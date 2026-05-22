#include <stdlib.h>
#include <stdio.h>
#include "menu/menu.h"
#include "bmp/t_bmp8.h"

int main(void) {

    t_bmp8 *ma_photo =(t_bmp8*) calloc(1, sizeof(t_bmp8));

    main_menu();
    choice_menu();

}