#ifndef _FILTRE_
#define _FILTRE_

#include "../bmp/t_bmp8.h"

void bmp8_negative(t_bmp8 * img);

void bmp8_brightness(t_bmp8 * img, int value);

void bmp8_threshold(t_bmp8 *img, int value);

#endif