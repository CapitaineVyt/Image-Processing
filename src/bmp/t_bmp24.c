#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "t_bmp24.h"

// 1. Alloue uniquement la matrice de pixels (width x height)
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    // Allocation du tableau de pointeurs de lignes
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (pixels == NULL) {
        fprintf(stderr, "Erreur : Allocation memoire echouee pour les lignes de la matrice.\n"); // [cite: 433]
        return NULL;
    }

    // Allocation de chaque ligne de pixels
    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (pixels[i] == NULL) {
            fprintf(stderr, "Erreur : Allocation memoire echouee pour la ligne %d.\n", i);
            // En cas d'échec sur une ligne, on libère ce qu'on a déjà alloué avant de quitter
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }

    return pixels; // [cite: 432]
}

// 2. Libère uniquement la matrice de pixels
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels == NULL) return;

    // On libère d'abord chaque ligne
    for (int i = 0; i < height; i++) {
        if (pixels[i] != NULL) {
            free(pixels[i]);
        }
    }
    // Puis on libère le tableau principal
    free(pixels);
}

// 3. Alloue la structure globale t_bmp24 et sa matrice
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    // Allocation de la structure principale
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24)); // [cite: 435]
    if (img == NULL) {
        fprintf(stderr, "Erreur : Allocation de la structure t_bmp24 echouee.\n");
        return NULL;
    }

    // Initialisation des champs basiques
    img->width = width; // [cite: 437]
    img->height = height; // [cite: 437]
    img->colorDepth = colorDepth; // [cite: 437]

    // Allocation de la matrice de pixels en appelant notre fonction précédente
    img->data = bmp24_allocateDataPixels(width, height); // [cite: 436]
    if (img->data == NULL) {
        // Si la matrice échoue, on nettoie la structure pour éviter les fuites
        free(img); // [cite: 439]
        return NULL; // [cite: 439]
    }

    return img; // [cite: 438]
}

// 4. Libère l'intégralité de l'image
void bmp24_free(t_bmp24 *img) {
    if (img == NULL) return;

    // 1. Libérer la matrice interne
    if (img->data != NULL) {
        bmp24_freeDataPixels(img->data, img->height);
    }
    // 2. Libérer la structure elle-même
    free(img); // [cite: 440]
}


// Lit n éléments de taille size à partir d'une position spécifique
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

// Écrit n éléments de taille size à une position spécifique
void file_rawWrite(uint32_t position, const void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

// Lit toutes les données de pixels du fichier en gérant le padding
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    fseek(file, image->header.offset, SEEK_SET);

    // Calcul du padding par ligne (chaque ligne binaire doit être multiple de 4 octets)
    int padding = (4 - (image->width * 3) % 4) % 4;

    // Boucle inversée pour les lignes
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
        // Sauter les octets de padding à la fin de la ligne lue
        if (padding > 0) {
            fseek(file, padding, SEEK_CUR);
        }
    }
}

// Fonction globale pour charger l'image complète
t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier '%s'.\n", filename);
        return NULL;
    }

    // 1. Déclarer les structures locales
    t_bmp_header header;
    t_bmp_info header_info;

    // --- LECTURE MANUELLE ET SÉCURISÉE DU PREMIER HEADER (14 octets pile) ---
    fseek(file, 0, SEEK_SET);
    fread(&header.type, sizeof(uint16_t), 1, file);
    fread(&header.size, sizeof(uint32_t), 1, file);
    fread(&header.reserved1, sizeof(uint16_t), 1, file);
    fread(&header.reserved2, sizeof(uint16_t), 1, file);
    fread(&header.offset, sizeof(uint32_t), 1, file);

    // --- LECTURE MANUELLE ET SÉCURISÉE DU SECOND HEADER (40 octets pile) ---
    fseek(file, 14, SEEK_SET); // L'en-tête info commence TOUJOURS à l'octet 14
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

    // Vérification de sécurité : est-ce bien un BMP 24 bits ?
    if (header.type != BMP_TYPE || header_info.bits != 24) {
        fprintf(stderr, "Erreur : Le fichier n'est pas un BMP 24 bits valide.\n");
        fclose(file);
        return NULL;
    }

    // 2. Allouer notre structure t_bmp24 avec la largeur et hauteur lues
    t_bmp24 *img = bmp24_allocate(header_info.width, header_info.height, header_info.bits);
    if (img == NULL) {
        fclose(file);
        return NULL;
    }

    // Copie des structures d'en-têtes bien remplies dans l'image
    img->header = header;
    img->header_info = header_info;

    // 3. Lire l'ensemble des pixels via notre fonction dédiée
    bmp24_readPixelData(img, file);

    fclose(file);
    return img;
}


// Écrit la matrice de pixels dans le fichier en rajoutant le padding requis
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
        // Écrire les octets de padding nécessaires pour que le BMP reste valide
        for (int p = 0; p < padding; p++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, file);
        }
    }
}

// Fonction globale pour sauvegarder l'image couleur complète
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    if (img == NULL || img->data == NULL) return;

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Erreur : Impossible de creer le fichier de sauvegarde '%s'.\n", filename);
        return;
    }

    // --- ÉCRITURE MANUELLE ET SÉCURISÉE DU PREMIER HEADER ---
    fseek(file, 0, SEEK_SET);
    fwrite(&img->header.type, sizeof(uint16_t), 1, file);
    fwrite(&img->header.size, sizeof(uint32_t), 1, file);
    fwrite(&img->header.reserved1, sizeof(uint16_t), 1, file);
    fwrite(&img->header.reserved2, sizeof(uint16_t), 1, file);
    fwrite(&img->header.offset, sizeof(uint32_t), 1, file);

    // --- ÉCRITURE MANUELLE ET SÉCURISÉE DU SECOND HEADER ---
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

    // 2. Écrire toutes les données de pixels
    bmp24_writePixelData(img, file);

    fclose(file);
    printf("Image 24 bits sauvegardee avec succes : '%s'\n", filename);
}


void bmp24_saveWithFilterName(const char *original_path, const char *filter_name, t_bmp24 *img) {
    if (img == NULL) {
        printf("Erreur : Aucune image en memoire a sauvegarder.\n");
        return;
    }

    // Sécurité si le chemin d'origine est vide
    if (original_path == NULL || strlen(original_path) == 0) {
        original_path = "image.bmp";
    }

    char new_filename[512] = "";

    // On cherche le dernier slash ou antislash pour isoler le nom du fichier (ex: flowers_color.bmp)
    const char *file_name = strrchr(original_path, '\\');
    if (file_name == NULL) {
        file_name = strrchr(original_path, '/');
    }

    if (file_name != NULL) {
        file_name++; // On avance d'un caractère pour dépasser le '/'
    } else {
        file_name = original_path; // Pas de slash, le chemin est juste le nom du fichier
    }

    if (strlen(file_name) == 0) {
        file_name = "default.bmp";
    }

    // On prépare le nouveau chemin dans le dossier output/
    sprintf(new_filename, "output/%s_%s", filter_name, file_name);

    printf("Nom du fichier de sortie genere : %s\n", new_filename);

    // On appelle ta fonction de sauvegarde 24 bits
    bmp24_saveImage(img, new_filename);
}