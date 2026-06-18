#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../bmp/t_bmp8.h"  
#include "../bmp/t_bmp24.h" 
#include "menu.h"
#include "../filter/menu_filtre.h"
#include "../filter/filtre.h"

static void *image_actuelle = NULL; 
static char chemin_origine[256] = "";
static int mode_24bit = 0;          

void main_menu(void) {
    printf("\n--Veuillez choisir une option--\n");
    printf("\t1. Ouvrir une image \n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4. Afficher les informations de l'image\n");
    printf("\t5. Quitter\n");
}

void run_main_menu(void) {
    int choice = 0;
    while (choice != 5) {
        main_menu(); 
        printf("Votre choix : ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Veuillez entrer un nombre valide.\n");
            int c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }
    
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        if (choice == 1) {
            printf("Quel type d'image voulez-vous ouvrir ?\n");
            printf("  1. Image 8 bits (Niveaux de gris)\n");
            printf("  2. Image 24 bits (Couleur)\n");
            printf("Votre choix : ");
            
            int type_choice = 0;
            if (scanf("%d", &type_choice) == 1) {
                while ((c = getchar()) != '\n' && c != EOF);
                
                if (image_actuelle != NULL) {
                    if (mode_24bit) bmp24_free((t_bmp24 *)image_actuelle);
                    else bmp8_free((t_bmp8 *)image_actuelle);
                    image_actuelle = NULL;
                }

                char access_path[256] = "";
                if (type_choice == 2) {
                    mode_24bit = 1;
                    printf("Entrez le chemin d'acces de l'image 24 bits : ");
                    if (scanf("%255s", access_path) == 1) {
                        while ((c = getchar()) != '\n' && c != EOF);
                        
                        if (access_path[0] == '"') memmove(access_path, access_path + 1, strlen(access_path));
                        int len = strlen(access_path);
                        if (len > 0 && access_path[len - 1] == '"') access_path[len - 1] = '\0';

                        image_actuelle = bmp24_loadImage(access_path);
                        if (image_actuelle != NULL) {
                            strcpy(chemin_origine, access_path);
                            printf("-> SUCCES : Image 24 bits chargee.\n");
                        } else {
                            printf("-> ECHEC : Impossible de charger l'image 24 bits.\n");
                        }
                    }
                } else {
                    mode_24bit = 0;
                    printf("Entrez le chemin d'acces de l'image 8 bits : ");
                    if (scanf("%255s", access_path) == 1) {
                        while ((c = getchar()) != '\n' && c != EOF);
                        
                        if (access_path[0] == '"') memmove(access_path, access_path + 1, strlen(access_path));
                        int len = strlen(access_path);
                        if (len > 0 && access_path[len - 1] == '"') access_path[len - 1] = '\0';

                        image_actuelle = bmp8_loadImage(access_path);
                        if (image_actuelle != NULL) {
                            strcpy(chemin_origine, access_path);
                            printf("-> SUCCES : Image 8 bits chargee.\n");
                        } else {
                            printf("-> ECHEC : Impossible de charger l'image 8 bits.\n");
                        }
                    }
                }
            }
        } 
        else if (choice == 2) {
            if (image_actuelle == NULL) {
                printf("Erreur : Aucune image en memoire.\n");
                continue;
            }
            char save_path[256] = "";
            printf("Entrez le chemin et le nom de la sauvegarde (ex: output/resultat.bmp) : ");
            if (scanf("%255s", save_path) == 1) {
                while ((c = getchar()) != '\n' && c != EOF);
                if (mode_24bit) bmp24_saveImage((t_bmp24 *)image_actuelle, save_path);
                else bmp8_saveImage(save_path, (t_bmp8 *)image_actuelle);
            }
        } 
        else if (choice == 3) {
            if (image_actuelle == NULL) {
                printf("Erreur : Veuillez d'abord charger une image.\n");
                continue;
            }
            
            choix_menu(image_actuelle, chemin_origine, mode_24bit);
            choice = 0; 
        } 
        else if (choice == 4) {
            if (image_actuelle == NULL) {
                printf("Aucune image chargee.\n");
            } else {
                if (mode_24bit) {
                    t_bmp24 *img = (t_bmp24 *)image_actuelle;
                    printf("Image Info (24 bits):\nWidth: %d\nHeight: %d\nColor Depth: %d\n", img->width, img->height, img->colorDepth);
                } else {
                    bmp8_printInfo((t_bmp8 *)image_actuelle);
                }
            }
        } 
        else if (choice == 5) {
            if (image_actuelle != NULL) {
                if (mode_24bit) bmp24_free((t_bmp24 *)image_actuelle);
                else bmp8_free((t_bmp8 *)image_actuelle);
                image_actuelle = NULL;
            }
            printf("Au revoir !\n");
        }
    }
}