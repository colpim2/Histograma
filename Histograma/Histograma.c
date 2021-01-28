#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "Histograma.h"

#define L 256

void histo_secuencial(int *histo, unsigned char *IMAGEN, int resolucion){
    //Inicializar con 0
    for(int i = 0; i < L; i++)
        histo[i] = 0;

    //Calculo del histograma
    for(int i = 0; i < resolucion; i++)
        histo[IMAGEN[i]]++;
}

void histo_paralelo(){
    int histop[NG],i,j;

    //Inicializar con 0
    for(i = 0; i < NG; i++)
        histo_redu[i] = 0;

    //Calculo del histograma
    #pragma omp parallel private(histop) num_threads(2)
    {
        //Inicializar con 0
        for(i = 0; i < NG; i++)
            histop[i] = 0;

        //Calculo por renglon
        #pragma omp for private(j)
            for(i = 0; i < N; i++)
                for(j = 0; j < N; j++)
                    histop[IMAGEN[i][j]]++;

        //Suma de todos los histop
        #pragma omp critical
        {
            for(i = 0; i < NG; i++)
                histo_redu[i] += histop[i];
        }
    }
}

void histo_reduction(){
    #pragma omp parallel
    {
        //Inicializar con 0
        #pragma omp for
            for(int i = 0; i < NG; i++)
                histo_para[i] = 0;

        //Calculo del histograma
        #pragma omp for reduction(+ : histo_para)
            for(int i = 0; i < N; i++)
                for(int j = 0; j < N; j++)
                    histo_para[IMAGEN[i][j]]++;
    }
}


