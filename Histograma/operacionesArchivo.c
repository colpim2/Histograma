#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "operacionesArchivo.h"

#define L 256
#define MAXTEXTO 500

//Lectura del nombre del archivo
int VerificarRuta(char *nombreIma, char *nombreImaExtend, char *rutaImagen){
    strcpy(nombreImaExtend,nombreIma);
    strcat(nombreImaExtend,".jpg");
    strcat(rutaImagen,nombreImaExtend);

    return access(rutaImagen,F_OK);
}

void GuardarCSV(int *histoImaO, int *nuevoHisto,char *archivoNombre){
    FILE *csv_secuencial = fopen(archivoNombre,"w+");

    //Datos del Header.
    fprintf(csv_secuencial, "%s,%s,%s \n", "Valor", "Histo", "HistoEc");

    //Contenido del archivo.
    for(int i=0; i<L; i++)
        fprintf(csv_secuencial,"%d,%d,%d \n",i,histoImaO[i],nuevoHisto[i]);

    fclose(csv_secuencial);
}
