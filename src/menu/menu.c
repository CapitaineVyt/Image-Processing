#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bmp/t_bmp8.h"  
#include "menu.h"
#include "../filter/menu_filtre.h"
#include "../filter/filtre.h"

t_bmp8 *image_actuelle = NULL;
char chemin_origine[256] = "";

void main_menu(void) {
    printf("\n--Veuillez choisir une option--\n");
    printf("\t1. Ouvrir une image \n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4. Afficher les informations de l'image\n");
    printf("\t5. Quitter\n");
    
}

static void choice_1(void) {
    char access_path[256] = "";
    printf("Entrez le chemin d'acces de l'image : ");

    // Saisie et nettoyage du chemin 
    if (fgets(access_path, sizeof(access_path), stdin) == NULL) return;
    access_path[strcspn(access_path, "\n")] = '\0';

    // Enlever les guillemets 
    if (access_path[0] == '"') {
        memmove(access_path, access_path + 1, strlen(access_path));
    }
    int len = strlen(access_path);
    if (len > 0 && access_path[len - 1] == '"') {
        access_path[len - 1] = '\0';
    }

    // Libération de l'ancienne image 
    if (image_actuelle != NULL) {
        bmp8_free(image_actuelle);
        image_actuelle = NULL; 
    }

    // Chargement de la nouvelle image 
    image_actuelle = bmp8_loadImage(access_path);

    // Message de confirmation à l'utilisateur
    if (image_actuelle != NULL) {
        strcpy(chemin_origine, access_path);
        printf("Le fichier charge est bien un .bmp et les pixels sont en memoire.\n");
    }
}

static void choice_2(){
    if (image_actuelle == NULL) {
        printf("Erreur : Aucune image en memoire a sauvegarder.\n");
        return;
    }
    char save_path[256] = "";
    printf("Entrez le chemin et nom de la sauvegarde (ex: output/resultat.bmp) : ");
    if (fgets(save_path, sizeof(save_path), stdin) == NULL) return;
    save_path[strcspn(save_path, "\n")] = '\0';

    bmp8_saveImage(save_path, image_actuelle);
}

static void choice_3(t_bmp8 *img, const char *path){
    choix_menu(img, path);
}

static void choice_4(){
    bmp8_printInfo(image_actuelle);
}

static void choice_5(){
    if (image_actuelle != NULL) {
        bmp8_free(image_actuelle);
        image_actuelle = NULL;
    }
    printf("Exiting\n");
}

void run_main_menu(void){
    int choice = 0;
    while (choice != 5) {
        main_menu();
        printf("Votre choix : ");
        
        if (scanf("%d", &choice) != 1) {
            
            printf("Veuillez entrer un nombre valide.\n");
            while(getchar() != '\n'); 
            continue;
        }
    
        getchar();

        if (choice == 1){
            choice_1();
        }else if(choice==2){
            choice_2();
        }else if(choice ==3){
            choice_3(image_actuelle, chemin_origine);
        }else if(choice ==4){
            choice_4();
        }else if(choice ==5){
            choice_5();
        }
    }
}