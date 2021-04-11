#include "opus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define AUDIO_INBUF_SIZE 204800
#define AUDIO_REFILL_THRESH 4096
#define AV_INPUT_BUFFER_PADDING_SIZE 64
int main(){
    const char *outfilename, *filename;
    uint8_t *data;
    uint8_t inbuf[AUDIO_INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    size_t   data_size;
    size_t out_size;
    filename = "../test.opus";
    outfilename = "out.opus";
    FILE *f, *outfile;
    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    outfile = fopen(outfilename, "wb");
    if (!outfile) {
        exit(1);
    }
    size_t num = 0;
    OpusDecoder* dec;
    int error;
    opus_int32 sampleRate = 8000;
    int channels = 1;
    int frameSizeMs = 60;
    int frameSize = channels * sampleRate * frameSizeMs/ 1000;
    opus_int16 *pcm = (opus_int16) malloc(frameSize);
    dec = opus_decoder_create(sampleRate, channels,&error);
    if (opus_decoder_init(dec, sampleRate,channels) != OPUS_OK)
    {

    }
    
    data      = inbuf;
    data_size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);
    // data = data + 6;
    while(data_size > 0){
        opus_decode(dec, data, data_size, pcm,frameSize,0);
        fwrite(data+num,1,data_size,outfile);
        num += data_size;
        data_size = fread(data+num, 1, AUDIO_INBUF_SIZE-num, f);
    }

    fclose(outfile);
    fclose(f);
    return 0;
}