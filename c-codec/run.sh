#!/bin/sh

# codec
# gcc codec.c -o codec -lavcodec -lavformat -lavutil
# rm codec

# decode_audio_file
# gcc decode_audio_file.c -o decode_audio_file -lavcodec -lavformat -lswresample -lavutil
# rm decode_audio_file

# decode_audio
# gcc decode_audio.c -o decode_audio -lavcodec -lavutil
# rm decode_audio

# decode_video
# gcc decode_video.c -o decode_video -lavcodec -lavutil
# rm decode_video

# demuxing_decoding 
# gcc demuxing_decoding.c -o demuxing_decoding -lavformat -lavcodec -lavutil
# rm demuxing_decoding

# encode_audio
# gcc encode_audio.c -o encode_audio -lavcodec -lavutil
# rm encode_audio

# http_multiclient
# gcc http_multiclient.c -o http_multiclient -lavutil -lavformat
# rm http_multiclient

# metadata
# gcc metadata.c -o metadata -lavformat -lavutil
# rm metadata

# opus_tool
# gcc opus_tool.c -o opus_tool -lopus -Iopus.h
# rm opus_tool

# decode from opsu_demo.c
# gcc decode.c -o decode -lopus -L$opus-1.3.1/silk/
# rm decode

# opus_demo from libopus-1.3.1/src/opus_demo
# gcc opus_demo.c -o opus_demo -lopus
# rm opus_demo

# opusdec, opusenc, opusrtp from libopus-tool-0.19/src/
# make in the program