#ifndef T_BMP8
#define T_BMP8

#include <stdio.h>

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;

t_bmp8 * bmp8_loadImage(const char * filepath);

void bmp8_saveImage(const char * filepath, t_bmp8 * img);

void bmp8_free(t_bmp8 * img);

void bmp8_printInfo(t_bmp8 * img);



#endif