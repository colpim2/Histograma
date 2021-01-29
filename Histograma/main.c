#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#include "Histograma.h"
#include "operacionesArchivo.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"

#define L 256

int main(){
    /* PARTE SECUENCIAL */

    //Cambiar a ingresado por el usuario usar arg
    char *rutaImagen = "../ImagenesTest/imaGray_1.jpg";
    char *nombreIma = "imaGray_1.jpg";

    //Carga de la imagen con la libreria stb
    int ancho, alto, nCanales, resolucion;
    double timeStartCarga,timeEndCarga;
    timeStartCarga = omp_get_wtime();

    unsigned char *imaOriginal = stbi_load(rutaImagen, &ancho, &alto, &nCanales, 0);
    timeEndCarga = omp_get_wtime()-timeStartCarga;

    if(imaOriginal == NULL){
        printf("La imagen %s no pudo ser cargada.\n",nombreIma);
        return 0;
    }
    else{
        printf("La imagen %s se ha cargado correctamente.\n", nombreIma);
        printf("Datos de la imagen:\n");
        resolucion = ancho*alto;
        printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d MegaPixeles\n",ancho,alto,nCanales,resolucion);
    }

    //Histograma generado de manera Secuencial.
    double timeStartSec,timeEndSec;
    timeStartSec = omp_get_wtime();

    int histoImaO[L];
    histo_secuencial(histoImaO,imaOriginal,resolucion);

    //Verificación de pixeles
    //printf("\nHistograma de la Imagen Original:\n");
    //printf("\nContador: %d\n",VerificarPixeles(histoImaO));

    //Generacion el cdf (Cumulative Distributive Function)
    int cdf[L];
    DistriAcumulada_secuencial(histoImaO,cdf);

    //Comprobación
    //printf("\nCDF:\n");
    //ImprimirMatriz(cdf);

    //Busqueda del valor minimo
    int cdf_min = Minimo_secuencial(cdf);
    printf("\nValor minimo del cdf: %d\n",cdf_min);

    //Generacion  del histograma ecualizado.
    int histoEc[L];
    for(int i=0; i<L; i++){
        double numerador = cdf[i]-cdf_min;
        double denominador = resolucion-cdf_min;
        int hv = numerador / denominador * (L-2) + 1;
        histoEc[i] = round(hv);
    }

    //Comprobación
    //printf("\nHISTOGRAMA ECUALIZADO:\n");
    //ImprimirMatriz(histoEc);

    //Generacion del arreglo de Pixeles para la nueva imagen.
    unsigned char *imaEc = malloc(resolucion * sizeof(unsigned char));

    for(int i=0; i<resolucion; i++){
        imaEc[i] = histoEc[imaOriginal[i]];
    }

    int nuevoHisto[L];
    histo_secuencial(nuevoHisto,imaEc,resolucion);

    timeEndSec = omp_get_wtime()-timeStartSec;

    //Verificar pixeles nuevo Histograma
    //printf("\n\nHistograma de la nueva imagen:\n");
    //printf("\nContador: %d\n",VerificarPixeles(nuevoHisto));

    //Guardar imagen generada.
    stbi_write_jpg("nuevaImagen.jpg", ancho, alto, 1, imaEc, 100);
    printf("\nImagen guardada correctamente.\n");

    //Liberar Memoria de la imagen.
    stbi_image_free(imaEc);

    //Generacion de archivo csv.
    char *ArchivoNombre = "histo_secuencial.csv";
    GuardarCSV(histoImaO,nuevoHisto,ArchivoNombre);
    printf("El archivo se ha creado correctamente.");


    /* PARTE PARALELA */

    //Se ocupa la ruta y nombre de secuencial :D
    printf("\n\n=== Version Paralelo ===\n");

    //Carga de imagen ocupada de secuencial
    printf("La imagen %s se ha cargado correctamente.\n", nombreIma);
    printf("Datos de la imagen:\n");
    printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d MegaPixeles\n",ancho,alto,nCanales,resolucion);

    //Variables compartidas -> Fuera del pragma
    int histoImaOPara[L];
    int cdfPara[L];
    int cdfPara_min = 1000000;
    int histoEcPara[L];
    int nuevoHistoPara[L];

    //Imagen Ecualizada
    unsigned char *imaEcPara = malloc(resolucion * sizeof(unsigned char));

    //Obtener numero de procesadores
    int numProces = omp_get_num_threads();

    double timeStartPara,timeEndPara;
    timeStartPara = omp_get_wtime();

    #pragma omp parallel num_threads(numProces)
    {
        //Inicializar con 0
        #pragma omp for
            for(int i=0; i<L; i++)
                histoImaOPara[i] = 0;

        #pragma omp barrier

        //Obtener histograma usando descomposición de dominio/num. de procesadores
        #pragma omp for reduction(+ : histoImaOPara)
            for(int i=0; i<resolucion; i++)
                histoImaOPara[imaOriginal[i]]++;

        //Cumulative Distributive Function y CDF min
        #pragma omp single
            cdfPara[0] = histoImaOPara[0];
            for(int i=1; i<L; i++)
            {
                cdfPara[i] = histoImaOPara[i]+cdfPara[i-1];
                if((cdfPara[i] < cdfPara_min)&&(cdfPara[i] != 0))
                    cdfPara_min = cdfPara[i];
            }
            printf("\nValor minimo del cdf: %d\n",cdfPara_min); //Comprobación

        #pragma omp barrier //???

        //Formula de ecualización
        #pragma omp for
            for(int i=0; i<L; i++){
                double numerador = cdfPara[i]-cdfPara_min;
                double denominador = resolucion-cdfPara_min;
                int hv = numerador / denominador * (L-2) + 1;
                histoEcPara[i] = round(hv);
            }

        #pragma omp barrier

        //Generar la nueva imagen
        #pragma omp for
            for(int i=0; i<resolucion; i++)
                imaEcPara[i] = histoEcPara[imaOriginal[i]];

        #pragma omp barrier

        #pragma omp for reduction(+ : nuevoHistoPara)
            for(int i=0; i<resolucion; i++)
                nuevoHistoPara[imaEcPara[i]]++;
    }
    timeEndPara = omp_get_wtime()-timeStartPara;

    //Guardar imagen generada.
    double timeStartGenerada,timeEndGenerada;
    timeStartGenerada = omp_get_wtime();

    stbi_write_jpg("nuevaImagenParalelo.jpg", ancho, alto, 1, imaEcPara, 100);

    timeEndGenerada = omp_get_wtime()-timeStartGenerada;
    printf("\nImagen guardada correctamente.\n");

    //Liberar Memoria de la imagen.
    stbi_image_free(imaEcPara);

    //Generacion de archivo csv.
    char *ArchivoNombrePara = "histo_paralelo.csv";
    double timeStartArchivo,timeEndArchivo;
    timeStartArchivo = omp_get_wtime();

    GuardarCSV(histoImaOPara,nuevoHistoPara,ArchivoNombrePara);

    timeEndArchivo = omp_get_wtime()-timeStartArchivo;
    printf("El archivo se ha creado correctamente.\n");

    //Calculo de métricas
    Metricas(timeEndSec,timeEndPara,numProces);
    OtrosTiempos(timeEndCarga,timeEndGenerada,timeEndArchivo);
    return 0;
}

/* NOTA
    - Funcionar con 3 canales [Tomar el rojo]
    - Pasar la ruta mediante linea de comando usando argc y argv
    Si no existe el argumento, arrojar un error y terminar el programa
    - Gráficas de histogramas [Original,Ecualizada]
    - Sufijos de las imagenes y CVS generados con el nombre original y Para,Sec
*/
