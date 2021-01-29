#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#include "operacionesArchivo.h"

#define L 256
#define MAXTEXTO 500

int EstadoArchivo(char *archivoNombre[MAXTEXTO]){
FILE *Archivo;
    if( (Archivo = fopen(archivoNombre,"r")) == NULL)
	{
        printf("No existe el archivo %s\n",archivoNombre);
        system("pause");
        return 0;
    }
    else{
        printf("DATOS GUARDADOS...\n");
        system("pause");
        return 1;
    }
}

void GuardarCSV(int *histoImaO, int *nuevoHisto,char *archivoNombre[MAXTEXTO]){
    FILE *csv_secuencial = fopen(archivoNombre,"w+");

    //Datos del Header.
    fprintf(csv_secuencial, "%s,%s,%s \n", "Valor", "Histo", "HistoEc");

    //Contenido del archivo.
    for(int i=0; i<L; i++)
        fprintf(csv_secuencial,"%d,%d,%d \n",i,histoImaO[i],nuevoHisto[i]);

    fclose(csv_secuencial);
}
