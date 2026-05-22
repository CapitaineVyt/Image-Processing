#include <stdio.h>
#include "menu.h"
#include <string.h>

void main_menu(void) {
    printf("--Veuillez choisir une option--\n");
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
    scanf("%d", &choice);
    if (choice == 1){
        choice_1();
    }else if(choice==2){
        choice_2();
    }else if(choice ==3){
        choice_3();
    }else if(choice ==4){
        choice_4();
    }else if(choice ==5){
        
    }
}