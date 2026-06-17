#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\bmp\t_bmp8.h"  
#include "..\bmp\t_bmp24.h"
#include "menu_filtre.h"    
#include "filtre.h"

// --- SOUS-FONCTIONS DE GESTION DU CHOIX ---

static void choix_1(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        bmp24_negative((t_bmp24 *)img, path); // Version couleur
    } else {
        bmp8_negative((t_bmp8 *)img, path);   // Version niveau de gris
    }
}

static void choix_2(void *img, const char *path, int is_24bit){
    int intensite = 0;
    printf("Entrez une valeur d'intensite (-255 a 255) : ");
    if (scanf("%d", &intensite) != 1) {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Nettoyage propre du buffer

    if (is_24bit) {
        bmp24_brightness((t_bmp24 *)img, intensite, path); // Version couleur
    } else {
        bmp8_brightness((t_bmp8 *)img, intensite, path);   // Version niveau de gris
    }
}

static void choix_3(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        // En 24 bits, la binarisation n'a pas de sens direct, 
        // le sujet demande une conversion en niveaux de gris (Grayscale)
        bmp24_grayscale((t_bmp24 *)img, path);
    } else {
        int seuil = 128;
        bmp8_threshold((t_bmp8 *)img, seuil, path); // Version niveau de gris
    }
}

static void choix_4(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        bmp24_boxBlur((t_bmp24 *)img, path); // Version couleur
    } else {
        bmp8_boxBlur((t_bmp8 *)img, path);   // Version niveau de gris
    }
}

static void choix_5(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        bmp24_gaussianBlur((t_bmp24 *)img, path); // Version couleur
    } else {
        bmp8_gaussianBlur((t_bmp8 *)img, path);   // Version niveau de gris
    }
}

static void choix_6(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        bmp24_sharpen((t_bmp24 *)img, path); // Version couleur
    } else {
        bmp8_sharpen((t_bmp8 *)img, path);   // Version niveau de gris
    }
}

static void choix_7(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        bmp24_outline((t_bmp24 *)img, path); // Version couleur
    } else {
        bmp8_outline((t_bmp8 *)img, path);   // Version niveau de gris
    }
}

static void choix_8(void *img, const char *path, int is_24bit){
    if (is_24bit) {
        bmp24_emboss((t_bmp24 *)img, path); // Version couleur
    } else {
        bmp8_emboss((t_bmp8 *)img, path);   // Version niveau de gris
    }
}

// --- FONCTION PRINCIPALE APPELÉE PAR LE MAIN MENU ---

void choix_menu(void *img, const char *path, int is_24bit){

    if (img == NULL || path == NULL) {
        printf("Erreur : Veuillez d'abord charger une image dans le menu principal.\n");
        return;
    }

    int choix = 0;
    while (choix != 9) {
        filtre_menu();
        printf("Votre choix : ");
        
        if (scanf("%d", &choix) != 1) {
            printf("Veuillez entrer un nombre valide.\n");
            int c;
            while((c = getchar()) != '\n' && c != EOF); 
            continue;
        }
    
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // Nettoyage systématique

        if (choix == 1){
            choix_1(img, path, is_24bit);
            return; 
        }else if(choix == 2){
            choix_2(img, path, is_24bit);
        }else if(choix == 3){
            choix_3(img, path, is_24bit);
        }else if(choix == 4){
            choix_4(img, path, is_24bit);
        }else if(choix == 5){
            choix_5(img, path, is_24bit);
        }else if(choix == 6){
            choix_6(img, path, is_24bit);
        }else if(choix == 7){
            choix_7(img, path, is_24bit);
        }else if(choix == 8){
            choix_8(img, path, is_24bit);
        }else if(choix == 9){
            printf("Retour au menu principal.\n");
        }
    }
}

void filtre_menu(void){
    printf("\n--Veuillez choisir un filtre-- \n");
    printf("\t1. Negatif\n");
    printf("\t2. Luminosite\n");
    printf("\t3. Binarisation / Niveaux de gris (24b)\n");
    printf("\t4. Flou\n");
    printf("\t5. Flou gaussien\n");
    printf("\t6. Nettete\n");
    printf("\t7. Contours\n");
    printf("\t8. Relief\n");
    printf("\t9. Retourner au menu principal\n");
}