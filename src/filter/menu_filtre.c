#include <stdio.h>
#include "menu_filtre.h"
#include "filtre.h"





static void choix_1(img){
    bmp8_negative(img);
}

static void choix_2(){

}

static void choix_3(){

}

static void choix_4(){

}

static void choix_5(){

}

static void choix_6(){

}

static void choix_7(){

}

static void choix_8(){

}

static void choix_9(){

}


void choix_menu(void){

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
            choix_1();
        }else if(choix==2){
            choix_2();
        }else if(choix ==3){
            choix_3();
        }else if(choix ==4){
            choix_4();
        }else if(choix ==5){
            choix_5();
        }else if(choix ==6){
            choix_6();
        }else if(choix ==7){
            choix_7();
        }else if(choix ==8){
            choix_8();
        }else if(choix ==9){
            printf("Exiting");
        }
    }

}

void filtre_menu(void){
    int choix;
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



