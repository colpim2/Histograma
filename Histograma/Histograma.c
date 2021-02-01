#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>

#include "Histograma.h"

#define L 256
#define MAXTEXTO 50

//Lectura del nombre del archivo
int manejoCadenas(char *nombreIma, char *nombreImaExtend, char *rutaImagen){
    strcpy(nombreImaExtend,nombreIma);
    strcat(nombreImaExtend,".jpg");
    strcat(rutaImagen,nombreImaExtend);
    if (access(rutaImagen,F_OK)!= 0)
        return 0;
    return 1;
}

int DistriAcumulada(int* histoImaO,int *cdf){
    int cdf_min = 10000000;
    cdf[0] = histoImaO[0];

    for(int i=1; i<L; i++){
        cdf[i] = histoImaO[i]+cdf[i-1];
        if((cdf[i] < cdf_min)&&(cdf[i] != 0))
            cdf_min = cdf[i];
    }
    return cdf_min;
}

//Métodos Comprobación
int VerificarPixeles(int *histoIma){
    int contador = 0;
    for(int i=0; i<L; i++){
        printf("[%d] ", histoIma[i]);
        contador+= histoIma[i];
    }
    return contador;
}

void ImprimirMatriz(int *histoEc){
    for(int i=0; i<L; i++)
        printf("[%d] ",histoEc[i]);
}

void Metricas(double timeEndSec,double timeEndPara,int numProces){
    printf("\n=== Metricas ===");
    printf("\n  *Numero de procesadores: %d\n",numProces);
    printf("\n  *Tiempos: \n     Secuencial: %f \n     Paralelo: %f",timeEndSec, timeEndPara);
    printf("\n  *SpeedUp: %f",timeEndSec/timeEndPara);
    printf("\n  *Eficiencia: %f",(timeEndSec/timeEndPara)/numProces);
    printf("\n  *Overhead: %f\n",timeEndPara-(timeEndSec/numProces));
}

void OtrosTiempos(double timeEndCarga,double timeEndGenerada,double timeEndArchivo){
    printf("\n=== Otros tiempos ===");
    printf("\n  *Carga imagen: %f",timeEndCarga);
    printf("\n  *Generar imagen: %f",timeEndGenerada);
    printf("\n  *Generar archivo: %f\n",timeEndArchivo);
}
