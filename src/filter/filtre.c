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

void bmp8_applyFilter(t_bmp8 *img, float kernel[3][3], int kernelSize) {
    if (img == NULL || img->data == NULL) return;

    // 1. Allouer un tableau temporaire pour stocker le résultat de la convolution
    unsigned char *temp_data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));
    if (temp_data == NULL) return;

    // On recopie d'abord l'image de base (pour garder les bordures intactes)
    memcpy(temp_data, img->data, img->dataSize);

    int w = img->width;
    int h = img->height;

    // 2. Parcourir les pixels (on ignore la bordure de 1 pixel: de 1 à w-1 et 1 à h-1)
    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            float sum = 0.0f;

            // 3. Appliquer le noyau de convolution (de -n à n, ici de -1 à 1)
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    // Trouver la position du pixel voisin dans le tableau 1D
                    int pixel_index = (y + ky) * w + (x + kx);
                    
                    // Récupérer la valeur du filtre (ky+1 et kx+1 pour aller de 0 à 2)
                    float kernel_val = kernel[ky + 1][kx + 1];

                    sum += (float)img->data[pixel_index] * kernel_val;
                }
            }

            // 4. Clamer le résultat entre 0 et 255 (sécurité anti-débordement)
            if (sum < 0.0f) sum = 0.0f;
            if (sum > 255.0f) sum = 255.0f;

            // Stocker le pixel calculé dans le tableau temporaire
            temp_data[y * w + x] = (unsigned char)sum;
        }
    }

    // 5. Remplacer les anciennes données par les nouvelles filtrées
    free(img->data);
    img->data = temp_data;
}


// 4. Flou (Box Blur)
void bmp8_boxBlur(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f}
    }; // [cite: 290]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "box_blur", img);
}

// 5. Flou Gaussien
void bmp8_gaussianBlur(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f},
        {2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f},
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f}
    }; // [cite: 290]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "gaussian_blur", img);
}

// 6. Netteté (Sharpen)
void bmp8_sharpen(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        { 0.0f, -1.0f,  0.0f},
        {-1.0f,  5.0f, -1.0f},
        { 0.0f, -1.0f,  0.0f}
    }; // [cite: 298]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "sharpen", img);
}

// 7. Contours (Outline)
void bmp8_outline(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  8.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f}
    }; // [cite: 293]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "outline", img);
}

// 8. Relief (Emboss)
void bmp8_emboss(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {-2.0f, -1.0f,  0.0f},
        {-1.0f,  1.0f,  1.0f},
        { 0.0f,  1.0f,  2.0f}
    }; // [cite: 298]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "emboss", img);
}