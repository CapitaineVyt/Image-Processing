#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // 
#include <string.h>
#include "t_bmp8.h"


t_bmp8 *bmp8_loadImage(char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) return NULL;

    t_bmp8 *img = (t_bmp8 *)calloc(1, sizeof(t_bmp8));
    if (img == NULL) { fclose(file); return NULL; }

    // 1. Lire le header
    fread(img->header, sizeof(unsigned char), 54, file);

    int pixelDataOffset = *(int *)&img->header[10];
    img->width          = *(int *)&img->header[18];
    img->height         = *(int *)&img->header[22];
    img->colorDepth     = *(short *)&img->header[28];

    // FORCE LE CALCUL : N'écoute pas le header pour dataSize, calcule-le toi-même !
    int rowSize = ((img->width * img->colorDepth + 31) / 32) * 4;
    img->dataSize = rowSize * abs(img->height);

    // 2. Allouer les pixels
    img->data = (unsigned char *)malloc(img->dataSize * sizeof(unsigned char));
    if (img->data == NULL) { free(img); fclose(file); return NULL; }

    // 3. Lire les pixels tout à la fin du fichier
    fseek(file, pixelDataOffset, SEEK_SET);
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    
    fclose(file);
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img){
    if (img == NULL || img->data == NULL) return;
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return;

    // 1. Écrire le header original (54 octets)
    fwrite(img->header, sizeof(unsigned char), 54, file);

    // 2. Gestion de la palette : On remplit le "trou" entre le header (54) et les pixels (ex: 1078)
    // avec des niveaux de gris progressifs si la palette d'origine n'a pas été stockée.
    int pixelsDataOffset = *(int *)&img->header[10];
    if (pixelsDataOffset > 54) {
        int paletteSize = pixelsDataOffset - 54;
        unsigned char *dummyPalette = (unsigned char *)calloc(paletteSize, 1);
        
        // Crée une palette de gris standard par défaut (R=G=B)
        for (int i = 0; i < paletteSize / 4 && i < 256; i++) {
            dummyPalette[i * 4 + 0] = i; // Bleu
            dummyPalette[i * 4 + 1] = i; // Vert
            dummyPalette[i * 4 + 2] = i; // Rouge
            dummyPalette[i * 4 + 3] = 0; // Réservé
        }
        fwrite(dummyPalette, sizeof(unsigned char), paletteSize, file);
        free(dummyPalette);
    }

    // 3. Écrire les pixels
    size_t pixelsWritten = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    printf("Image sauvegardee : '%s' (%d octets de pixels ecrits, total: %d octets)\n", 
            filename, img->dataSize, pixelsDataOffset + img->dataSize);

    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    if (img != NULL) {
        if (img->data != NULL) {
            free(img->data);
        }
        free(img);
    }
}



void bmp8_saveWithFilterName(const char *original_path, const char *filter_name, t_bmp8 *img) {
    if (img == NULL) {
        printf("Erreur : Aucune image en memoire a sauvegarder.\n");
        return;
    }

    if (original_path == NULL || strlen(original_path) == 0) {
        original_path = "image.bmp";
    }

    char new_filename[512] = "";

    const char *file_name = strrchr(original_path, '\\');
    if (file_name == NULL) {
        file_name = strrchr(original_path, '/');
    }

    if (file_name != NULL) {
        file_name++; 
    } else {
        file_name = original_path; 
    }

    if (strlen(file_name) == 0) {
        file_name = "default.bmp";
    }

    sprintf(new_filename, "output/%s_%s", filter_name, file_name);

    printf("Nom du fichier dans le dossier output: %s\n", new_filename);

    bmp8_saveImage(new_filename, img);
}


void bmp8_printInfo(t_bmp8 * img){
    printf("Image Info\n");
    printf("\tWidth : %d\n", img->width);
    printf("\tHeight : %d\n", img->height);
    printf("\tColor Depth : %d\n", img->colorDepth);
    printf("\tData Size : %d", img->dataSize);
}
