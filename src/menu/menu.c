#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "../bmp/t_bmp8.h"
#include <string.h>
#include "../filter/menu_filtre.h"

t_bmp8 *image_actuelle = NULL;

void main_menu(void) {
    printf("\n--Veuillez choisir une option--\n");
    printf("\t1. Ouvrir une image \n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4. Afficher les informations de l'image\n");
    printf("\t5. Quitter\n");
    
}

static void choice_1(void){
    char access_path[256] = "";
    printf("Entrez le chemin d'acces de l'image : ");

    fgets(access_path, sizeof(access_path), stdin);
    access_path[strcspn(access_path, "\n")] = '\0';

//Enlever les guillemets
    if (access_path[0] == '"') {
    memmove(access_path, access_path + 1, strlen(access_path));
    }

    int len = strlen(access_path);
    if (len > 0 && access_path[len - 1] == '"') {
        access_path[len - 1] = '\0';
    }

    FILE* img = fopen(access_path, "rb"); 
    
    if (img == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'. No such file or directory.\n", access_path);
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
        fprintf(stderr, "Critical Error: Memory allocation failed for image struct.\n");
        fclose(img);
        return;
    }

    fread(image_actuelle->header, sizeof(unsigned char), 54, img);

    if (image_actuelle->header[0]=='B' && image_actuelle->header[1]=='M'){
        printf("Le fichier charge est bien un .bmp\n");
    } else {
        fprintf(stderr, "Error: Invalid format. '%s' is not a standard BMP file.\n", access_path);
    };
    

    fclose(img);
}

static void choice_2(){

}

static void choice_3(){
    choix_menu();

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