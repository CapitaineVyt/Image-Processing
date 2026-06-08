#include <stdio.h>
#include <stdlib.h>
#include "t_bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filepath){

    FILE * file = fopen(filepath, "rb");

    if(file ==NULL){
        fprintf(stderr,"Memory allocation failed for the structure.");
    }

}

void bmp8_free(t_bmp8 *img) {
    if (img != NULL) {
        if (img->data != NULL) {
            free(img->data);
        }
        free(img);
    }
}