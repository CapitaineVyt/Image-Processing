#include <stdio.h>
#include <stdlib.h>
#include "../bmp/t_bmp8.h"
#include <string.h>
#include "../filter/menu_filtre.h"
#include"../filter/filtre.h"


void bmp8_negative(t_bmp8 * img, const char *original_path){
    if(img ==NULL || img->data ==NULL){
        fprintf(stderr, "Erreur, l'image est vide\n");
    }

    for(int i=0; i < img->dataSize; i++){
        img->data[i] = 255 - img->data[i];
    }

    printf("Filtre negatif applique avec succes !");
    bmp8_saveWithFilterName(original_path, "negatif", img);
}

void bmp8_brightness(t_bmp8 *img, int value, const char *original_path) {
    if (img == NULL || img->data == NULL) {
        fprintf(stderr, "Erreur, l'image est vide\n");
        return;
    }

    for (int i = 0; i < img->dataSize; i++) {
        int new_pixel_val = (int)img->data[i] + value;

        if (new_pixel_val < 0) {
            img->data[i] = 0;
        } 
        else if (new_pixel_val > 255) {
            img->data[i] = 255;
        } 
        else {
            img->data[i] = (unsigned char)new_pixel_val;
        }
    }

    printf("Filtre luminosite applique avec succes !\n");

    bmp8_saveWithFilterName(original_path, "luminosite", img);
}

void bmp8_threshold(t_bmp8 *img, int value, const char *original_path) {
    if (img == NULL || img->data == NULL) {
        fprintf(stderr, "Erreur, l'image est vide\n");
        return;
    }

    for (int i = 0; i < img->dataSize; i++) {
        if (img->data[i] < value) {
            img->data[i] = 0;
        } 
        else {
            img->data[i] = 255;
        }
    }

    printf("Filtre binarisation applique avec succes !\n");

    bmp8_saveWithFilterName(original_path, "binarisation", img);
}