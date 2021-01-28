#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "menu.h"
#include "Histograma.h"
//#include "operacionesArchivo.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"

#define L 256

int main(){
    char *rutaImagen = "../ImagenesTest/imaGray_1.jpg";
    char *nombreIma = "imaGray_1.jpg";

    //Carga de la imagen con la libreria stb
    int ancho, alto, nCanales, resolucion;

    unsigned char *imaOriginal = stbi_load(rutaImagen, &ancho, &alto, &nCanales, 0);
    if(imaOriginal == NULL){
        printf("La imagen %s no pudo ser cargada.\n",nombreIma);
        return 0;
    }
    else{
        printf("La imagen %s se ha cargado correctamente.\n", nombreIma);
        printf("Datos de la imgen:\n");
        resolucion=ancho*alto;
        printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d MegaPixeles\n",ancho,alto,nCanales,resolucion);
    }

    //Histograma generado de manera Secuencial.
    int histoImaO[L];
    histo_secuencial(histoImaO,imaOriginal,resolucion);

    printf("Histograma de la Imagen Original:\n");
    int contador=0;
    for(int i=0; i<L; i++){
        printf("[%d] ", histoImaO[i]);
        contador+=histoImaO[i];
    }
    printf("\nContador: %d\n",contador);

    //Generacion el cdf (Cumulative Distributive Function)
    int cdf[L];
    cdf[0] = histoImaO[0];

    for(int i=1; i<L; i++)
        cdf[i] = histoImaO[i]+cdf[i-1];

    printf("\nCDF:\n");
    for(int i=0; i<L; i++)
        printf("[%d] ",cdf[i]);

    //Busqueda del valor minimo en cdf diferente de 0.
    int cdf_min = 1000000;

    for(int i=L-1; i>=0;i--){
        if(cdf[i] == 0)
            break;
        if(cdf[i] < cdf_min)
            cdf_min = cdf[i];
    }
    printf("Valor minimo del cdf: %d\n",cdf_min);

    //Generacion  del histograma ecualizado.
    int histoEc[L];
    for(int i=0; i<L; i++){
        double numerador = cdf[i]-cdf_min;
        double denominador = resolucion-cdf_min;
        int hv = numerador / denominador * (L-2) + 1;
        histoEc[i] = round(hv);
    }

    printf("\n\nHISTOGRAMA ECUALIZADO:\n");
    for(int i=0; i<L; i++)
        printf("[%d] ",histoEc[i]);

    //Generacion del arreglo de Pixeles para la nueva imagen.
    unsigned char *imaEc = malloc(resolucion * sizeof(unsigned char));

    for(int i=0; i<resolucion; i++){
        imaEc[i] = histoEc[imaOriginal[i]];
        //printf("%d ",imaEc[i]);
    }

    int nuevoHisto[L];
    histo_secuencial(nuevoHisto,imaEc,resolucion);

    printf("\nHistograma de la nueva imagen:\n");
    contador=0;
    for(int i=0; i<L; i++){
        printf("[%d] ", nuevoHisto[i]);
        contador+=nuevoHisto[i];
    }
    printf("\nContador: %d\n",contador);

    //Guardar imagen generada.
    stbi_write_jpg("nuevaImagen.jpg", ancho, alto, 1, imaEc, 100);
    printf("Imagen guardada correctamente.\n");

    //Liberar Memoria
    stbi_image_free(imaEc);
    return 0;

    /*
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

    /*
    //Calculo de Métricas
    printf("\n** Metricas **");
    printf("\n  *Times: \n     Secuencial: %f \n     Paralelo: %f \n     Reduction: %f",TimeEnd, TimeEnd2, TimeEnd3);
    printf("\n  *SpeedUps: %f | %f ",TimeEnd/TimeEnd2 , TimeEnd/TimeEnd3);
    printf("\n  *Eficiencia: %f | %f ",(TimeEnd/TimeEnd2)/numProces, (TimeEnd/TimeEnd3)/numProces);
    printf("\n  *Overhead: %f | %f \n",TimeEnd2-(TimeEnd/numProces), TimeEnd3-(TimeEnd/numProces));

    return 0;
    */
}
