#include <stdio.h>
#include "menu.h"
#include <string.h>

void main_menu() {
    printf("--Veuillez choisir une option--\n");
    printf("\t1. Ouvrir une image \n");
    printf("\t2. Sauvegarder une image\n");
    printf("\t3. Appliquer un filtre\n");
    printf("\t4. Quitter\n");
    
}

void choice_1(){
    char access_path[256] = "";
    printf("Entrez le chemin d'acces de l'image : ");
    scanf("%s", access_path);
    FILE* img = fopen(access_path, "rb"); 
    
    if (img == NULL) {
        printf("No such file or directory\n");
    }

    fclose(img);
}

void choice_2(){

}

void choice_3(){

}

void choice_4(){

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
    }
}