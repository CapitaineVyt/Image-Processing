#include <stdio.h>
#include "..\bmp\t_bmp8.h"  
#include "menu_filtre.h"    
#include "filtre.h"

static void choix_1(t_bmp8 *img, const char *path){
    bmp8_negative(img, path);
}

static void choix_2(t_bmp8 *img, const char * path){
    int intensite = 0;
    printf("Entrez une valeur valeur d'intensite : ");
    if (scanf("%d", &intensite) != 1) {
        while(getchar() != '\n');
    }
    getchar();
    bmp8_brightness(img, intensite, path);
}

static void choix_3(t_bmp8 *img, const char * path){
    int seuil = 128;
    bmp8_threshold(img, seuil, path);
}

static void choix_4(t_bmp8 *img, const char * path){

}

static void choix_5(t_bmp8 *img, const char * path){

}

static void choix_6(t_bmp8 *img, const char * path){

}

static void choix_7(t_bmp8 *img, const char * path){

}

static void choix_8(t_bmp8 *img, const char * path){

}

void choix_menu(t_bmp8 *img, const char *path){

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
            while(getchar() != '\n'); 
            continue;
        }
    
        getchar();

        if (choix == 1){
            choix_1(img, path);
        }else if(choix==2){
            choix_2(img, path);
        }else if(choix ==3){
            choix_3(img, path);
        }else if(choix ==4){
            choix_4(img, path);
        }else if(choix ==5){
            choix_5(img, path);
        }else if(choix ==6){
            choix_6(img, path);
        }else if(choix ==7){
            choix_7(img, path);
        }else if(choix ==8){
            choix_8(img, path);
        }else if(choix ==9){
            printf("Exiting\n");
        }
    }

}

void filtre_menu(void){
    printf("--Veuillez choisit un filtre-- \n");
    printf("\t1. Negatif\n");
    printf("\t2. Luminosite\n");
    printf("\t3. Binarisation\n");
    printf("\t4. Flou\n");
    printf("\t5. Flou gaussien\n");
    printf("\t6. Nettete\n");
    printf("\t7. Contours\n");
    printf("\t8. Relief\n");
    printf("\t9. Retourner au menu principal\n");

}