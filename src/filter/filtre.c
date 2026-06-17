#include <stdio.h>
#include <stdlib.h>
#include "../bmp/t_bmp8.h"
#include "../bmp/t_bmp24.h"
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


// 1. Filtre négatif avec sauvegarde automatique
void bmp24_negative(t_bmp24 *img, const char *original_path) {
    if (img == NULL || img->data == NULL) return;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red   = 255 - img->data[y][x].red; 
            img->data[y][x].green = 255 - img->data[y][x].green; 
            img->data[y][x].blue  = 255 - img->data[y][x].blue; 
        }
    }
    printf("Filtre negatif 24 bits applique avec succes !\n");
    bmp24_saveWithFilterName(original_path, "negatif", img);
}

// 2. Filtre niveaux de gris avec sauvegarde automatique
// 2. Filtre niveaux de gris avec sauvegarde automatique
void bmp24_grayscale(t_bmp24 *img, const char *original_path) {
    if (img == NULL || img->data == NULL) return;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t moy = (img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3;
            img->data[y][x].red   = moy;
            img->data[y][x].green = moy;
            img->data[y][x].blue  = moy;
        }
    }
    printf("Conversion en niveaux de gris (24 bits) reussie !\n");
    bmp24_saveWithFilterName(original_path, "grayscale", img);
}

// 3. Filtre luminosité avec sauvegarde automatique
void bmp24_brightness(t_bmp24 *img, int value, const char *original_path) {
    if (img == NULL || img->data == NULL) return;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int r = (int)img->data[y][x].red   + value; 
            int g = (int)img->data[y][x].green + value; 
            int b = (int)img->data[y][x].blue  + value; 

            img->data[y][x].red   = (r > 255) ? 255 : ((r < 0) ? 0 : r); 
            img->data[y][x].green = (g > 255) ? 255 : ((g < 0) ? 0 : g); 
            img->data[y][x].blue  = (b > 255) ? 255 : ((b < 0) ? 0 : b); 
        }
    }
    printf("Luminosite 24 bits ajustee avec succes !\n");
    bmp24_saveWithFilterName(original_path, "brightness", img);
}


t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float kernel[3][3], int kernelSize) {
    float sum_r = 0.0f;
    float sum_g = 0.0f;
    float sum_b = 0.0f;

    int n = kernelSize / 2; // Pour un masque 3x3, n = 1 [cite: 284, 306]

    // Parcours du masque de -1 à 1 [cite: 306]
    for (int ky = -n; ky <= n; ky++) {
        for (int kx = -n; kx <= n; kx++) {
            // Coordonnées du pixel voisin
            int px = x + kx;
            int py = y + ky;

            // Récupération du coefficient du filtre (on décale de +n pour aller de 0 à 2)
            float k_val = kernel[ky + n][kx + n];

            // Convolution indépendante sur les 3 canaux [cite: 527, 530]
            sum_r += (float)img->data[py][px].red   * k_val;
            sum_g += (float)img->data[py][px].green * k_val;
            sum_b += (float)img->data[py][px].blue  * k_val;
        }
    }

    // Clamping de sécurité pour chaque canal (0 à 255) [cite: 531]
    if (sum_r < 0.0f) sum_r = 0.0f; else if (sum_r > 255.0f) sum_r = 255.0f;
    if (sum_g < 0.0f) sum_g = 0.0f; else if (sum_g > 255.0f) sum_g = 255.0f;
    if (sum_b < 0.0f) sum_b = 0.0f; else if (sum_b > 255.0f) sum_b = 255.0f;

    // On retourne le pixel final calculé
    t_pixel result_pixel;
    result_pixel.red   = (uint8_t)sum_r;
    result_pixel.green = (uint8_t)sum_g;
    result_pixel.blue  = (uint8_t)sum_b;

    return result_pixel;
}


void bmp24_applyFilter(t_bmp24 *img, float kernel[3][3], int kernelSize, const char *original_path, const char *filter_name) {
    if (img == NULL || img->data == NULL) return;

    // 1. Allouer la matrice temporaire pour stocker le résultat
    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) return;

    // 2. Appliquer la convolution sur tous les pixels (sauf la bordure de 1 pixel) [cite: 308, 309]
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            // Si on est sur le bord, on garde le pixel d'origine intact [cite: 308]
            if (y == 0 || y == img->height - 1 || x == 0 || x == img->width - 1) {
                temp_data[y][x] = img->data[y][x];
            } else {
                // Sinon, on calcule la convolution [cite: 541]
                temp_data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
            }
        }
    }

    // 3. Remplacer l'ancienne matrice par la nouvelle filtrée
    bmp24_freeDataPixels(img->data, img->height);
    img->data = temp_data;

    // 4. Sauvegarde automatique avec le nom du filtre !
    bmp24_saveWithFilterName(original_path, filter_name, img);
}

// 4. Flou Couleur (Box Blur) [cite: 543]
void bmp24_boxBlur(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f}
    }; // [cite: 289, 290]
    bmp24_applyFilter(img, kernel, 3, path, "boxBlur");
}

// 5. Flou Gaussien Couleur [cite: 544]
void bmp24_gaussianBlur(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f},
        {2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f},
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f}
    }; // [cite: 290, 291]
    bmp24_applyFilter(img, kernel, 3, path, "gaussianBlur");
}

// 6. Netteté Couleur [cite: 546]
void bmp24_sharpen(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        { 0.0f, -1.0f,  0.0f},
        {-1.0f,  5.0f, -1.0f},
        { 0.0f, -1.0f,  0.0f}
    }; // [cite: 297, 298]
    bmp24_applyFilter(img, kernel, 3, path, "sharpen");
}

// 7. Détection de contours Couleur (Outline)
void bmp24_outline(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  8.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f}
    };
    bmp24_applyFilter(img, kernel, 3, path, "outline");
}

// 8. Relief Couleur (Emboss)
void bmp24_emboss(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {-2.0f, -1.0f,  0.0f},
        {-1.0f,  1.0f,  1.0f},
        { 0.0f,  1.0f,  2.0f}
    };
    bmp24_applyFilter(img, kernel, 3, path, "emboss");
}