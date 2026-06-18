#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "t_bmp24.h"

t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        fprintf(stderr, "Erreur : Allocation memoire echouee pour les lignes de la matrice.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            fprintf(stderr, "Erreur : Allocation memoire echouee pour la ligne %d.\n", i);
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels == NULL) return;

    for (int i = 0; i < height; i++) {
        if (pixels[i] != NULL) {
            free(pixels[i]);
        }
    }
    free(pixels);
}

t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));
    if (img == NULL) {
        fprintf(stderr, "Erreur : Allocation de la structure t_bmp24 echouee.\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    img->data = bmp24_allocateDataPixels(width, height);
    if (img->data == NULL) {
        free(img);
        return NULL;
    }

    return img;
}

void bmp24_free(t_bmp24 *img) {
    if (img == NULL) return;

    if (img->data != NULL) {
        bmp24_freeDataPixels(img->data, img->height);
    }
    free(img);
}

void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite(uint32_t position, const void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    fseek(file, image->header.offset, SEEK_SET);

    int padding = (4 - (image->width * 3) % 4) % 4;

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            uint8_t b, g, r;
            fread(&b, sizeof(uint8_t), 1, file);
            fread(&g, sizeof(uint8_t), 1, file);
            fread(&r, sizeof(uint8_t), 1, file);

            image->data[y][x].red   = r;
            image->data[y][x].green = g;
            image->data[y][x].blue  = b;
        }
        if (padding > 0) {
            fseek(file, padding, SEEK_CUR);
        }
    }
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s'.\n", filename);
        return NULL;
    }

    t_bmp_header header;
    t_bmp_info header_info;

    fseek(file, 0, SEEK_SET);
    fread(&header.type, sizeof(uint16_t), 1, file);
    fread(&header.size, sizeof(uint32_t), 1, file);
    fread(&header.reserved1, sizeof(uint16_t), 1, file);
    fread(&header.reserved2, sizeof(uint16_t), 1, file);
    fread(&header.offset, sizeof(uint32_t), 1, file);

    fseek(file, 14, SEEK_SET);
    fread(&header_info.size, sizeof(uint32_t), 1, file);
    fread(&header_info.width, sizeof(int32_t), 1, file);
    fread(&header_info.height, sizeof(int32_t), 1, file);
    fread(&header_info.planes, sizeof(uint16_t), 1, file);
    fread(&header_info.bits, sizeof(uint16_t), 1, file);
    fread(&header_info.compression, sizeof(uint32_t), 1, file);
    fread(&header_info.imagesize, sizeof(uint32_t), 1, file);
    fread(&header_info.xresolution, sizeof(int32_t), 1, file);
    fread(&header_info.yresolution, sizeof(int32_t), 1, file);
    fread(&header_info.ncolors, sizeof(uint32_t), 1, file);
    fread(&header_info.importantcolors, sizeof(uint32_t), 1, file);

    if (header.type != BMP_TYPE || header_info.bits != 24) {
        fprintf(stderr, "Erreur : Le fichier n'est pas un BMP 24 bits valide.\n");
        fclose(file);
        return NULL;
    }

    t_bmp24 *img = bmp24_allocate(header_info.width, header_info.height, header_info.bits);
    if (img == NULL) {
        fclose(file);
        return NULL;
    }

    img->header = header;
    img->header_info = header_info;

    bmp24_readPixelData(img, file);

    fclose(file);
    return img;
}

void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    fseek(file, image->header.offset, SEEK_SET);

    int padding = (4 - (image->width * 3) % 4) % 4;
    uint8_t padding_byte = 0x00;

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            uint8_t b = image->data[y][x].blue;
            uint8_t g = image->data[y][x].green;
            uint8_t r = image->data[y][x].red;

            fwrite(&b, sizeof(uint8_t), 1, file);
            fwrite(&g, sizeof(uint8_t), 1, file);
            fwrite(&r, sizeof(uint8_t), 1, file);
        }
        for (int p = 0; p < padding; p++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, file);
        }
    }
}

void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    if (img == NULL || img->data == NULL) return;

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible de creer le fichier de sauvegarde '%s'.\n", filename);
        return;
    }

    fseek(file, 0, SEEK_SET);
    fwrite(&img->header.type, sizeof(uint16_t), 1, file);
    fwrite(&img->header.size, sizeof(uint32_t), 1, file);
    fwrite(&img->header.reserved1, sizeof(uint16_t), 1, file);
    fwrite(&img->header.reserved2, sizeof(uint16_t), 1, file);
    fwrite(&img->header.offset, sizeof(uint32_t), 1, file);

    fseek(file, 14, SEEK_SET);
    fwrite(&img->header_info.size, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.width, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.height, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.planes, sizeof(uint16_t), 1, file);
    fwrite(&img->header_info.bits, sizeof(uint16_t), 1, file);
    fwrite(&img->header_info.compression, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.imagesize, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.xresolution, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.yresolution, sizeof(int32_t), 1, file);
    fwrite(&img->header_info.ncolors, sizeof(uint32_t), 1, file);
    fwrite(&img->header_info.importantcolors, sizeof(uint32_t), 1, file);

    bmp24_writePixelData(img, file);

    fclose(file);
    printf("Image 24 bits sauvegardee avec succes : '%s'\n", filename);
}

void bmp24_saveWithFilterName(const char *original_path, const char *filter_name, t_bmp24 *img) {
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

    printf("Nom du fichier de sortie genere : %s\n", new_filename);

    bmp24_saveImage(img, new_filename);
}