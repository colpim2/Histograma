#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "Histograma.h"

#define L 256

//Métodos Secuenciales
void histo_secuencial(int *histo, unsigned char *IMAGEN, int resolucion){
    //Inicializar con 0
    for(int i = 0; i < L; i++)
        histo[i] = 0;

    //Calculo del histograma
    for(int i = 0; i < resolucion; i++)
        histo[IMAGEN[i]]++;
}

void DistriAcumulada_secuencial(int* histoImaO,int *cdf){
    cdf[0] = histoImaO[0];

    for(int i=1; i<L; i++)
        cdf[i] = histoImaO[i]+cdf[i-1];

}

int Minimo_secuencial(int* cdf){
    int cdf_min = 1000000;

    //Valor minimo en cdf diferente de 0.
    for(int i=L-1; i>=0; i--){
        if(cdf[i] == 0)
            break;
        if(cdf[i] < cdf_min)
            cdf_min = cdf[i];
    }
    return cdf_min;
}

//Métodos Paralelos


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
    printf("\n** Metricas **");
    printf("\n  *Tiempos: \n     Secuencial: %f \n     Paralelo: %f",timeEndSec, timeEndPara);
    printf("\n  *SpeedUp: %f",timeEndSec/timeEndPara);
    printf("\n  *Eficiencia: %f",(timeEndSec/timeEndPara)/numProces);
    printf("\n  *Overhead: %f\n",timeEndPara-(timeEndSec/numProces));
}

void OtrosTiempos(double timeEndCarga,double timeEndGenerada,double timeEndArchivo){
    printf("\n** Otros tiempos **");
    printf("\n  *Carga imagen: %f",timeEndCarga);
    printf("\n  *Generar imagen: %f",timeEndGenerada);
    printf("\n  *Generar archivo: %f\n",timeEndArchivo);
}
