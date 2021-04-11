# 学习音视频编解码

分为几个部分：

- c-codec：一些ffmpeg和libopus的例子，使用原生c语言
- ffplay：计划中学习ffplay源码
- go-decode-opus：使用go包装的ffmepg或者libopus
- wrap：包装c语言的练习



#### [理解ffmpeg的编解码过程](https://github.com/leandromoreira/ffmpeg-libav-tutorial#audio---what-you-listen)

从tutorial中的理解和我一些例子的学习，总结出来（不一定对）：

编码和解码应该放在一起来理解。首先音视频的文件结构是，需要有个container容器来组织video和audio，甚至还有字幕。从container中拿到video和audio，此时称为Stream，video为Stream#0，audio为Stream#1，或者更多。这个取出stream的操作，在ffmpeg中，是由format模块来完成的，具体下来，就是AVFormatContext。得到了AVStream后，可以划分为packet，称为AVPacket，此时的packet就是输入codec模块的单位，之前不太明白也没有想过packet是怎么从stream得到的，现在看了ogg-opus的写文件过程后，才有点感觉。

在[ogg-opus](https://tools.ietf.org/html/rfc7845.html#page-6)中,opus是一种编码类型，ogg是一种文件容器，如何组织opus的，之前对组织只停留在就是header中加上一些信息。ogg文件把文件分成好几个page，这里的page，除开前面几个信息的字节（固定字节长度），剩下的就是数据已编码的音频数据，而前面的字节信息中会有这段已编码数据的长度，那么我猜测这里的page中的数据就是得到的packet。总的来看，就是编码的时候，把raw 未编码的音频数据PCM一段放进编码器，得到的编码后的数据（packet），加上他的长度作为一个page，也就是编解码是对应的。

A====》encodec====》B====》+len（B）====》page====》ogg file

ogg file====》page====》B+len（B）====》decode====》A

如何做到，就是因为每个page前面的字节数是固定的，每个page前面所带的信息有packet的长度，那么就可以分离出每个page，每个packet，把已知长度的packet送到decodec中，进行decode。

所以，容器container不仅仅是header，还有如何组织每个块，还有时间轴的组织等等应该也有讲究，现在就只看了ogg-opus的结构，其他的没看。应该原理差不多。

所以不是每个container都支持每个codec，remuxing的时候，也有可能需要transcode，还要重新计算时间。



#### c-codec

- 1.[decode_audio.c](https://ffmpeg.org/doxygen/trunk/decode_audio_8c-example.html) & [decode_vidoe.c](https://ffmpeg.org/doxygen/trunk/decode_video_8c-example.html)

这个例子应该只是作为解码数据的例子，没有涉及音视频文件的container（format），测试的时候没有比较好的文件输入，只是拿了encode_audio.c的例子把pcm文件进行编码，但是使用这个例子的时候，选择的codec只能选择```AV_CODEC_ID_MP2```，选择其他的会报错，暂时还没找到原因。使用这个编码后的文件，其实是带format，所以到这我就懵逼了，也搞不明白了。不过，暂时也没空去深挖了。

- 2.[demuxing_decoding.c](https://ffmpeg.org/doxygen/trunk/demuxing_decoding_8c-example.html) & codec.c & decode_audio_file.c

这三个例子大体上是一样的，都是从format中取出stream，stream取出packet，packet解码出frame。demuxing_decoding.c是ffmpeg例子，codec.c是我改造的tutorial中的例子，decode_audio_file.c是网上瞎找的例子，当时啥都不懂。。。这些例子就都比较好理解，输入的文件都是比较常用的，可以测试。

- 3.opus系列: [libopus](https://opus-codec.org/) [opus-tool](https://www.opus-codec.org/release/dev/2018/09/18/opus-tools-0_2.html) [opusfile](https://github.com/xiph/opusfile/tree/master/src)
  - libopus中的opus_demo.c例子，是直接encode&decode的例子，也就是说，他不能直接用于正常的有container结构的文件，看他的源码，encode读取每一段音频数据进行编码，当然前面会有一些设置，编码成功后，在已编码的数据的前面加上这个数据的长度在几个字节，这个自己根据长度的大小定，demo里好像放的是4个字节的长度，所以在decode中需要解析出前4个字节的非编码数据，输入decode function中需要data和len，这就都有了，还有输出的frame和frame size，这里的size还不知道如何确定，当时只是一个个的试，试到合适为止。如何frame size和data len不匹配的话，size太小，decode function 会报too small ，太大的话，得到的frame 数据后面会有很多0填充，可能可以设置，但是现在暂时还未知。
  - Opus-tool中的，opusdec.c & opusenc.c & opusrtp.c的例子，是有ogg container的demo，可以输入正常的opus或者ogg文件。当然他的底层使用的编解码函数也是libopus的。
  - opusfile是更高一层封装的API，具体怎么用还没了解。



- ffplay



- go-decode-opus
- wrap
