#!/bin/sh
gcc -c cmdutils.c -I./ -I/usr/local/Cellar/ffmpeg/4.2.2_2/include -I/usr/local/Cellar/ffmpeg/4.2.2_2/include/libavutil -o cmdutil.o
# gcc -c ffplay.c -I../ -I/usr/local/Cellar/sdl2/2.0.12_1/include/SDL2 -I./ -o fplay.o
# gcc  fplay.o cmdutils.o -o fplay -L/usr/local/Cellar/ffmpeg/4.2.2_2/lib -lavutil -lavformat -lavdevice -lswscale -lavcodec -lswresample -lavfilter -lpostproc -lSDL2 
# ./configure --enable-shared --prefix=./out