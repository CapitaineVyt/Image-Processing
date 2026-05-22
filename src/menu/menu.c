#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "../bmp/t_bmp8.h"
#include <string.h>

t_bmp8 *image_actuelle = NULL;

void main_menu(void) {
    printf("\n--Veuillez choisir une option--\n");
    printf("\t1. Ouvrir une image \n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4.  Afficher les informations de l'image\n");
    printf("\t5. Quitter\n");
    
}

static void choice_1(void){
    char access_path[256] = "";
    printf("Entrez le chemin d'acces de l'image : ");

    fgets(access_path, sizeof(access_path), stdin);
    access_path[strcspn(access_path, "\n")] = '\0';

    FILE* img = fopen(access_path, "rb"); 
    
    if (img == NULL) {
        printf("No such file or directory\n");
        return;
    }
//Libère l'image si l'utilisateur en a déja mise une

    if (image_actuelle != NULL){
        if(image_actuelle->data != NULL){
            free(image_actuelle->data);
        }
        free(image_actuelle);
    }
    image_actuelle = (t_bmp8*) calloc(1, sizeof(t_bmp8));
    if (image_actuelle == NULL) {
        printf("Erreur d'allocation mémoire\n");
        fclose(img);
        return;
    }

    fread(image_actuelle->header, sizeof(unsigned char), 54, img);

    if (image_actuelle->header[0]=='B' && image_actuelle->header[1]=='M'){
        printf("Le fichier charge est bien un .bmp\n");
    } else {
        printf("ERROR! Le format charge n'est pas correct\n");
    };
    

    fclose(img);
}

static void choice_2(){

}

static void choice_3(){

}

static void choice_4(){

}

static void choice_5(){
    
}

void choice_menu(){
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
            choice_3();
        }else if(choice ==4){
            choice_4();
        }else if(choice ==5){
            printf("Exiting");
        }
    }
}