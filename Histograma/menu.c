#include <stdio.h>
#include "menu.h"

int desplegarMenu (char texto[], int n) {
   int opcion;
   do {
       printf("%s ",texto);
       scanf("%d", &opcion);
       if (opcion < 1 || opcion > n)
       {
        printf("Error: opcion no valida...\n");
        system("pause");
        system("cls");
       }
   } while (opcion < 1 || opcion > n);
   return opcion;
}
