#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "menu.h"
#include "Histograma.h"
//#include "operacionesArchivo.h"

int main(){
    printf("HISTOGRAMA");
    //Creacion de matriz simulación imagen
    srand( time(NULL) );
    for(int i = 0; i <N; i++)
        for(int j = 0; j < N; j++)
            IMAGEN[i][j] = rand() % 256;

    //Calculo del histograma con tiempo
    double TimeIni,TimeEnd,TimeEnd2,TimeEnd3;

    TimeIni = omp_get_wtime();
    histo_secuencial();
    TimeEnd = omp_get_wtime() - TimeIni;

    TimeIni = omp_get_wtime();
    histo_paralelo();
    TimeEnd2 = omp_get_wtime() - TimeIni;

    TimeIni = omp_get_wtime();
    histo_reduction();
    TimeEnd3 = omp_get_wtime() - TimeIni;

    //Comprobar Histogramas
    /*
    for(size_t i = 0; i < NG; i++)
        printf("%d - %d , %d , %d [%s , %s]\n", i, histo_sec[i], histo_par[i], histo_redu[i], histo_sec[i] == histo_par[i]? "OK":"Error",histo_sec[i] == histo_redu[i]? "OK":"Error" )
    */

    //Calculo de Métricas
    printf("\n** Metricas **");
    printf("\n  *Times: \n     Secuencial: %f \n     Paralelo: %f \n     Reduction: %f",TimeEnd, TimeEnd2, TimeEnd3);
    printf("\n  *SpeedUps: %f | %f ",TimeEnd/TimeEnd2 , TimeEnd/TimeEnd3);
    printf("\n  *Eficiencia: %f | %f ",(TimeEnd/TimeEnd2)/numProces, (TimeEnd/TimeEnd3)/numProces);
    printf("\n  *Overhead: %f | %f \n",TimeEnd2-(TimeEnd/numProces), TimeEnd3-(TimeEnd/numProces));

    return 0;
}
