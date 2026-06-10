#include <stdio.h>
#include <stdlib.h>
#include "../bmp/t_bmp8.h"
#include <string.h>
#include "../filter/menu_filtre.h"
#include"../filter/filtre.h"


void bmp8_negative(t_bmp8 * img){
    if(img ==NULL || img->data ==NULL){
        fprintf(stderr, "Erreur, l'image est vide\n");
    }

    for(int i=0; i < img->dataSize; i++){
        img->data[i] = 255 - img->data[i];
    }

    printf("Filtre negatif applique avec succes !");
}

void bmp8_brightness(t_bmp8 * img, int value){
    if(img ==NULL || img->data ==NULL){
        fprintf(stderr, "Erreur, l'image est vide\n");
    }

    for(int i=0; i < img->dataSize; i++){
        if (img->data[i] + value <0){
            img->data[i] = 0;
        }else if (img->data[i] + value > 255){
            img->data[i] = 255;
        }
    }

    printf("Filtre luminosite applique avec succes !");
}

void bmp8_threshold(t_bmp8 *img, int value){
    if(img ==NULL || img->data ==NULL){
        fprintf(stderr, "Erreur, l'image est vide\n");
    }

    for(int i=0; i< img->dataSize; i++){
        if(img->data[i]<value){
            img->data[i] = 0;
        }else if (img->data[i]>value){
            img->data[i] = 255;
        }
    }

    printf("Filtre binarisation applique avec succes !");
}