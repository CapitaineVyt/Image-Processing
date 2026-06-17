#ifndef _FILTRE_
#define _FILTRE_

#include "../bmp/t_bmp8.h"

void bmp8_negative(t_bmp8 * img, const char *path);

void bmp8_brightness(t_bmp8 * img, int value, const char *path);

void bmp8_threshold(t_bmp8 *img, int value, const char *path);

void bmp8_applyFilter(t_bmp8 *img, float kernel[3][3], int kernelSize);

void bmp8_boxBlur(t_bmp8 *img, const char *path);

void bmp8_gaussianBlur(t_bmp8 *img, const char *path);

void bmp8_sharpen(t_bmp8 *img, const char *path);

void bmp8_outline(t_bmp8 *img, const char *path);

void bmp8_emboss(t_bmp8 *img, const char *path);

#endif