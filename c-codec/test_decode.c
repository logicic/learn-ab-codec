#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <math.h>
 
 
#include    <libavutil/opt.h>
#include    <libavutil/mathematics.h>
#include    <libavutil/imgutils.h>
#include    <libavutil/samplefmt.h>
#include    <libavutil/timestamp.h>
#include    <libavformat/avformat.h>
#include    <libavcodec/avcodec.h>
#include    <libswscale/swscale.h>
#include    <libavutil/mathematics.h>
#include    <libswresample/swresample.h>
#include    <libavutil/channel_layout.h>
#include    <libavutil/common.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>

#define FF_INPUT_BUFFER_PADDING_SIZE 16
#define CODEC_CAP_TRUNCATED       0x0008

int main(int argc, char * argv[])
{
const char *input;
const char *outfilename = "test.pcm";
const char *filename = "out.mp3";
 
input = argv[1];
 
av_register_all();
avcodec_register_all();
 
AVFormatContext    *iFormatCtx        = NULL;
AVCodecContext     *iaCodecCtx        = NULL;
AVCodec            *iaCodec           = NULL;
AVFrame            *frame             = NULL;
AVPacket           ipacket;
av_init_packet(&ipacket);
AVDictionary       *optionsDict       = NULL;
 
struct SwsContext  *sws_ctx           = NULL;
 
int                AudioStreamIndex;
int                frameFinished;
int                numBytes;
int len;
int i;
FILE *f, *outfile;
uint8_t *outbuf;
uint8_t *outbuf3;
uint8_t *outbuf2;
uint8_t inbuf[1024+ FF_INPUT_BUFFER_PADDING_SIZE];
 
iFormatCtx = avformat_alloc_context();
if(avformat_open_input(&iFormatCtx, input, NULL, NULL)!=0)
{
    printf("could not open input_file\n"); 
    return -1;
}
printf("The url: %s\n", input);
 
if(avformat_find_stream_info(iFormatCtx,NULL) < 0)//获取文件内音视频流的信息
    {
            printf("find stream info failed\n");
            return -1 ;
    }     
av_dump_format(iFormatCtx, 0, input, 0);
 
AudioStreamIndex = -1;
for(i=0; i<iFormatCtx->nb_streams; i++)
{
    if(iFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && AudioStreamIndex <0 )
    {
        AudioStreamIndex =  i;
        break;
    }
}
 
if(AudioStreamIndex== -1)
{
    printf("Cannot find VideoStream\n");
    return -1;
}
 
iaCodecCtx    = iFormatCtx->streams[AudioStreamIndex]->codec;
iaCodec       = avcodec_find_decoder(iaCodecCtx->codec_id);
if(iaCodec == NULL)
{
    printf("unsupported codec!\n");
    return -1;
}
 
if(iaCodec->capabilities & CODEC_CAP_TRUNCATED)
    iaCodecCtx->flags |= CODEC_CAP_TRUNCATED;
 
 
if(avcodec_open2(iaCodecCtx, iaCodec, &optionsDict)<0)
{
    printf("could not open the codec\n");
    return -1;
}
 
outbuf = (uint8_t *)malloc(1024);
 
f = fopen(filename, "rb");
    if (f) {
        printf("could not open %s\n", filename);
        return -1;
    }
outfile = fopen(outfilename, "wb");
if (!outfile) {
        printf("open outfile failed\n");
        av_free(iaCodecCtx);
        return -1 ;
    }
 
frame = av_frame_alloc();
if (!frame) {
        fprintf(stderr, "Could not allocate audio frame\n");
        exit(1);
    }
 
    //frame->nb_samples     = iaCodecCtx->frame_size;
    //frame->format         = iaCodecCtx->sample_fmt;
    //frame->channel_layout = iaCodecCtx->channel_layout;
 
int pktsize = 0;
uint8_t *pktdata;
 
while(av_read_frame(iFormatCtx, &ipacket)>=0) 
{
    if(ipacket.stream_index == AudioStreamIndex)
    {
 
        while(ipacket.size>0)
        {    
        int out_size;
        len = avcodec_decode_audio4(iaCodecCtx, frame, &out_size, &ipacket);
        if (len < 0) 
        {
            pktsize = 0;
            printf("Error while decoding\n");
            continue;
        }
        if (out_size ) 
        {
        int data_size1 = av_samples_get_buffer_size(NULL, iaCodecCtx->channels, frame->nb_samples, iaCodecCtx->sample_fmt, 1);    
            //fwrite(frame->data[0], 1, data_size1, outfile);
            fwrite(frame->data[0], 1, frame->linesize[0], outfile);
            fflush(outfile);
        }
            ipacket.size -=len;
            ipacket.data +=len;
        } 
    }
}
 
return 0;

}