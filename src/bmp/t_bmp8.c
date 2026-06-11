#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t_bmp8.h"

t_bmp8 *bmp8_loadImage(char *filepath) {
    // Ouverture du fichier
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'.\n", filepath);
        return NULL;
    }

    // Allocation de la structure principale
    t_bmp8 *img = (t_bmp8 *)calloc(1, sizeof(t_bmp8));
    if (img == NULL) {
        fprintf(stderr, "Critical Error: Memory allocation failed for image struct.\n");
        fclose(file);
        return NULL;
    }

    // Lecture de l'en-tête (les 54 premiers octets)
    fread(img->header, sizeof(unsigned char), 54, file);

    // 4Vérification de la signature 'BM'
    if (img->header[0] != 'B' || img->header[1] != 'M') {
        fprintf(stderr, "Error: Invalid format. '%s' is not a standard BMP file.\n", filepath);
        free(img);
        fclose(file);
        return NULL;
    }

    // Affiliation des différentes données du fichier grâce au header
    int pixelDataOffset = *(int *)&img->header[10];
    int width           = *(int *)&img->header[18];
    int height          = *(int *)&img->header[22];
    short colorDepth    = *(short *)&img->header[28];
    int dataSize        = *(int *)&img->header[34];

    // Vérification de sécurité (optionnelle mais recommandée : 8 bits)
    if (colorDepth != 8) {
        fprintf(stderr, "Error: Only 8-bit grayscale BMP images are supported.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Allocation du tableau de pixels pour les datas
    img->data = (unsigned char *)malloc(dataSize * sizeof(unsigned char));
    if (img->data == NULL) {
        fprintf(stderr, "Critical Error: Memory allocation failed for pixel data.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Positionnement au début des pixels et lecture
    fseek(file, pixelDataOffset, SEEK_SET);
    size_t bytesRead = fread(img->data, sizeof(unsigned char), dataSize, file);
    
    if (bytesRead != dataSize) {
        fprintf(stderr, "Warning: Expected %d bytes, but read %zu bytes.\n", dataSize, bytesRead);
    }

    // Fermeture du fichier et retour du pointeur
    fclose(file);
    return img;
}

void bmp8_free(t_bmp8 *img) {
    if (img != NULL) {
        if (img->data != NULL) {
            free(img->data);
        }
        free(img);
    }
}

void bmp8_saveImage(const char *filename, t_bmp8 *img){
    if (img ==NULL || img->data ==NULL){
        fprintf(stderr,"Erreur ! L'image est vide.");
        return;
    }
    FILE *file = fopen(filename, "wb");
    if (file ==NULL){
        fprintf(stderr, "Erreur! Impossible de creer le fichier %s", filename);
        return;
    }

    size_t headerWritten = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (headerWritten !=54){
        fprintf(stderr, "Erreur lors de l'ecriture du header du fichier. \n");
        fclose(file);
        return;
    }

    int pixelsDataOffset = *(int *)&img->header[10];

    fseek(file, pixelsDataOffset, SEEK_SET);

    size_t pixelsWritten = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    if (pixelsWritten != img->dataSize) {
        fprintf(stderr, "Attention : Erreur lors de l'écriture des pixels (%zu octets écrits sur %d).\n", pixelsWritten, img->dataSize);
    } else {
        printf("Image sauvegardée avec succès sous le nom : '%s'\n", filename);
    }

}

void bmp8_saveWithFilterName(const char *original_path, const char *filter_name, t_bmp8 *img){
    if (img ==NULL){
        fprintf(stderr,"Erreur ! L'image est vide.");
        return;
    }
    char new_filename[512] = "";

    const char *file_name =strrchr(original_path, '\\');
    if(file_name ==NULL){
        file_name = strrchr(original_path, '/');
    }

    if(file_name != NULL){
        file_name++;
    }else{
        file_name = original_path;
    }

    sprintf(new_filename, "output/%S_%s", file_name, new_filename);

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
