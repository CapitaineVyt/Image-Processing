#ifndef _FILTRE_
#define _FILTRE_

#include "../bmp/t_bmp8.h"
#include "../bmp/t_bmp24.h"

void bmp8_negative(t_bmp8 * img, const char *path);

void bmp8_brightness(t_bmp8 * img, int value, const char *path);

void bmp8_threshold(t_bmp8 *img, int value, const char *path);

void bmp8_applyFilter(t_bmp8 *img, float kernel[3][3], int kernelSize);

void bmp8_boxBlur(t_bmp8 *img, const char *path);

void bmp8_gaussianBlur(t_bmp8 *img, const char *path);

void bmp8_sharpen(t_bmp8 *img, const char *path);

void bmp8_outline(t_bmp8 *img, const char *path);

void bmp8_emboss(t_bmp8 *img, const char *path); 

// 24 bits filters
void bmp24_negative(t_bmp24 *img, const char *original_path);

void bmp24_grayscale(t_bmp24 *img, const char *original_path);

void bmp24_brightness(t_bmp24 *img, int value, const char *original_path);


t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float kernel[3][3], int kernelSize);

void bmp24_applyFilter(t_bmp24 *img, float kernel[3][3], int kernelSize, const char *original_path, const char *filter_name);

void bmp24_boxBlur(t_bmp24 *img, const char *path);

void bmp24_gaussianBlur(t_bmp24 *img, const char *path);

void bmp24_sharpen(t_bmp24 *img, const char *path);

#endif