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

    unsigned char *temp_data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));
    if (temp_data == NULL) return;

    memcpy(temp_data, img->data, img->dataSize);

    int w = img->width;
    int h = img->height;

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            float sum = 0.0f;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int pixel_index = (y + ky) * w + (x + kx);
                    
                    float kernel_val = kernel[ky + 1][kx + 1];

                    sum += (float)img->data[pixel_index] * kernel_val;
                }
            }

            if (sum < 0.0f) sum = 0.0f;
            if (sum > 255.0f) sum = 255.0f;

            temp_data[y * w + x] = (unsigned char)sum;
        }
    }

    free(img->data);
    img->data = temp_data;
}


// Flou Box Blur
void bmp8_boxBlur(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f}
    }; // [cite: 290]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "box_blur", img);
}

// Flou Gaussien
void bmp8_gaussianBlur(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f},
        {2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f},
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f}
    }; // [cite: 290]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "gaussian_blur", img);
}

// Netteté Sharpen
void bmp8_sharpen(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        { 0.0f, -1.0f,  0.0f},
        {-1.0f,  5.0f, -1.0f},
        { 0.0f, -1.0f,  0.0f}
    }; // [cite: 298]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "sharpen", img);
}

// Contours Outline
void bmp8_outline(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  8.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f}
    }; // [cite: 293]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "outline", img);
}

// Relief Emboss
void bmp8_emboss(t_bmp8 *img, const char *path) {
    float kernel[3][3] = {
        {-2.0f, -1.0f,  0.0f},
        {-1.0f,  1.0f,  1.0f},
        { 0.0f,  1.0f,  2.0f}
    }; // [cite: 298]
    bmp8_applyFilter(img, kernel, 3);
    bmp8_saveWithFilterName(path, "emboss", img);
}


// Filtre négatif avec sauvegarde automatique
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

// Filtre niveaux de gris avec sauvegarde automatique
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

// Filtre luminosité avec sauvegarde automatique
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

    for (int ky = -n; ky <= n; ky++) {
        for (int kx = -n; kx <= n; kx++) {
            int px = x + kx;
            int py = y + ky;

            float k_val = kernel[ky + n][kx + n];

            sum_r += (float)img->data[py][px].red   * k_val;
            sum_g += (float)img->data[py][px].green * k_val;
            sum_b += (float)img->data[py][px].blue  * k_val;
        }
    }

    if (sum_r < 0.0f) sum_r = 0.0f; else if (sum_r > 255.0f) sum_r = 255.0f;
    if (sum_g < 0.0f) sum_g = 0.0f; else if (sum_g > 255.0f) sum_g = 255.0f;
    if (sum_b < 0.0f) sum_b = 0.0f; else if (sum_b > 255.0f) sum_b = 255.0f;

    t_pixel result_pixel;
    result_pixel.red   = (uint8_t)sum_r;
    result_pixel.green = (uint8_t)sum_g;
    result_pixel.blue  = (uint8_t)sum_b;

    return result_pixel;
}


void bmp24_applyFilter(t_bmp24 *img, float kernel[3][3], int kernelSize, const char *original_path, const char *filter_name) {
    if (img == NULL || img->data == NULL) return;

    t_pixel **temp_data = bmp24_allocateDataPixels(img->width, img->height);
    if (temp_data == NULL) return;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            if (y == 0 || y == img->height - 1 || x == 0 || x == img->width - 1) {
                temp_data[y][x] = img->data[y][x];
            } else {
                temp_data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
            }
        }
    }

    // Remplacer l'ancienne matrice par la nouvelle filtrée
    bmp24_freeDataPixels(img->data, img->height);
    img->data = temp_data;

    // Sauvegarde automatique avec le nom du filtre !
    bmp24_saveWithFilterName(original_path, filter_name, img);
}

// Flou Couleur Box Blur
void bmp24_boxBlur(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f},
        {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f}
    }; // [cite: 289, 290]
    bmp24_applyFilter(img, kernel, 3, path, "boxBlur");
}

// Flou Gaussien Couleur
void bmp24_gaussianBlur(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f},
        {2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f},
        {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f}
    }; // [cite: 290, 291]
    bmp24_applyFilter(img, kernel, 3, path, "gaussianBlur");
}

// Netteté Couleur 
void bmp24_sharpen(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        { 0.0f, -1.0f,  0.0f},
        {-1.0f,  5.0f, -1.0f},
        { 0.0f, -1.0f,  0.0f}
    };
    bmp24_applyFilter(img, kernel, 3, path, "sharpen");
}

//  Détection de contours Couleur Outline
void bmp24_outline(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  8.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f}
    };
    bmp24_applyFilter(img, kernel, 3, path, "outline");
}

// Relief Couleur Emboss
void bmp24_emboss(t_bmp24 *img, const char *path) {
    float kernel[3][3] = {
        {-2.0f, -1.0f,  0.0f},
        {-1.0f,  1.0f,  1.0f},
        { 0.0f,  1.0f,  2.0f}
    };
    bmp24_applyFilter(img, kernel, 3, path, "emboss");
}

// Histogramme 8bits
void bmp8_histogramEqualization(t_bmp8 *img, const char *path) {
    if (img == NULL || img->data == NULL) return;

    int total_pixels = img->width * img->height;

    int histogram[256] = {0};
    for (int y = 0; y < img->height; y++) {
        for (int z = 0; z < img->width; z++) {
            uint8_t pixel_value = img->data[y * img->width + z];
            histogram[pixel_value]++;
        }
    }

    int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    if (total_pixels - cdf_min == 0) return;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t old_val = img->data[y * img->width + x];
            
            float equalized = ((float)(cdf[old_val] - cdf_min) / (total_pixels - cdf_min)) * 255.0f;
            
            if (equalized < 0.0f) equalized = 0.0f;
            if (equalized > 255.0f) equalized = 255.0f;
            
            img->data[y * img->width + x] = (uint8_t)(equalized + 0.5f);
        }
    }

    bmp8_saveImage("output/equalized_8bit.bmp", img);
    printf("Egalisation 8 bits appliquee avec succes.\n");
}

// Histogramme 24bits
void bmp24_histogramEqualization(t_bmp24 *img, const char *path) {
    if (img == NULL || img->data == NULL) return;
    int total = img->width * img->height;
    
    float **Y = (float **)malloc(img->height * sizeof(float *));
    float **U = (float **)malloc(img->height * sizeof(float *));
    float **V = (float **)malloc(img->height * sizeof(float *));
    for (int y = 0; y < img->height; y++) {
        Y[y] = (float *)malloc(img->width * sizeof(float));
        U[y] = (float *)malloc(img->width * sizeof(float));
        V[y] = (float *)malloc(img->width * sizeof(float));
    }

    int histogram[256] = {0};
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            float r = img->data[y][x].red;
            float g = img->data[y][x].green;
            float b = img->data[y][x].blue;

            float y_val = 0.299f * r + 0.587f * g + 0.114f * b;
            float u_val = -0.169f * r - 0.331f * g + 0.500f * b + 128.0f;
            float v_val = 0.500f * r - 0.419f * g - 0.081f * b + 128.0f;

            Y[y][x] = y_val;
            U[y][x] = u_val;
            V[y][x] = v_val;

            int y_int = (int)(y_val + 0.5f);
            if (y_int < 0) y_int = 0;
            if (y_int > 255) y_int = 255;
            histogram[y_int]++;
        }
    }

    int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    int cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    if (total - cdf_min > 0) {
        for (int y = 0; y < img->height; y++) {
            for (int x = 0; x < img->width; x++) {
                int old_y_int = (int)(Y[y][x] + 0.5f);
                if (old_y_int < 0) old_y_int = 0;
                if (old_y_int > 255) old_y_int = 255;

                float new_y = ((float)(cdf[old_y_int] - cdf_min) / (total - cdf_min)) * 255.0f;
                float u = U[y][x] ; 
                float v = V[y][x];

                float r_new = new_y + 1.403f * (v - 128.0f);
                float g_new = new_y - 0.344f * (u - 128.0f) - 0.714f * (v - 128.0f);
                float b_new = new_y + 1.773f * (u - 128.0f);

                if (r_new < 0) r_new = 0; if (r_new > 255) r_new = 255;
                if (g_new < 0) g_new = 0; if (g_new > 255) g_new = 255;
                if (b_new < 0) b_new = 0; if (b_new > 255) b_new = 255;

                img->data[y][x].red   = (uint8_t)(r_new + 0.5f);
                img->data[y][x].green = (uint8_t)(g_new + 0.5f);
                img->data[y][x].blue  = (uint8_t)(b_new + 0.5f);
            }
        }
    }

    for (int y = 0; y < img->height; y++) {
        free(Y[y]);
        free(U[y]);
        free(V[y]);
    }
    free(Y);
    free(U);
    free(V);

    bmp24_saveWithFilterName(path, "equalized", img);
}