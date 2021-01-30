#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>

#include "Histograma.h"
#include "operacionesArchivo.h"
#include "menu.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"

#define L 256
#define MAXTEXTO 50

int main(int argc, char *argv[]){
    int opcion;
    do
    {
      opcion = desplegarMenu("MENU:\nSeleccione la opcion que desea ejecutar\n1) Ecualizacion del Histograma (Serial / Paralelo)\n2) Convertir imagen de color a escala de grises\n3) Salir del programa\nOpcion: ",3);
      system("cls");
      switch (opcion)
      {
        case 1: ;
            /* PARTE SECUENCIAL */
            //Obtener ruta de la imagen
            char nombreIma[MAXTEXTO],nombreImaExtend[MAXTEXTO];
            char rutaImagen[MAXTEXTO] = "../ImagenesTest/";

            printf("Ingrese el nombre de la imagen jpg con la que se desea trabajar (sin la extensi%cn .jpg): ",162);
            scanf(" %[^\n]",&nombreIma);
            strcpy(nombreImaExtend,nombreIma);
            strcat(nombreImaExtend,".jpg");
            strcat(rutaImagen,nombreImaExtend);
            if (access(rutaImagen,F_OK)!= 0){
                printf("Lo sentimos, no existe la imagen %s :c",nombreImaExtend);
                return 0;
            }

            printf("=== Version Secuencial ===\n");

            //Carga de la imagen con la libreria stb
            int ancho, alto, nCanales, resolucion;
            double timeStartCarga,timeEndCarga;

            timeStartCarga = omp_get_wtime();
            unsigned char *imaOriginal = stbi_load(rutaImagen, &ancho, &alto, &nCanales, 0);
            timeEndCarga = omp_get_wtime()-timeStartCarga;

            if(imaOriginal == NULL){
                printf("La imagen %s no pudo ser cargada.\n",nombreImaExtend);
                return 0;
            }
            else{
                printf("La imagen %s se ha cargado correctamente.\n", nombreImaExtend);
                printf("Datos de la imagen:\n");
                resolucion = ancho*alto;
                printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d MegaPixeles\n",ancho,alto,nCanales,resolucion);
            }

            if(nCanales == 3){
                unsigned char *imaTemp = malloc(resolucion);
                for(int i=0; i<resolucion; i++)
                    imaTemp[i] = imaOriginal[i*nCanales];
                imaOriginal = imaTemp;
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
            char nombreImaSec[MAXTEXTO];
            strcpy(nombreImaSec,nombreIma);
            strcat(nombreImaSec,"_Sec.jpg");
            stbi_write_jpg(nombreImaSec, ancho, alto, 1, imaEc, 100);
            printf("\nImagen guardada correctamente.\n");

            //Liberar Memoria de la imagen.
            stbi_image_free(imaEc);

            //Generacion de archivo csv.
            char ArchivoNombreSec[MAXTEXTO];
            strcpy(ArchivoNombreSec,nombreIma);
            strcat(ArchivoNombreSec,"_Histo_Sec.csv");
            GuardarCSV(histoImaO,nuevoHisto,ArchivoNombreSec);
            printf("El archivo se ha creado correctamente.");


            /* PARTE PARALELA */

            //Se ocupa la ruta y nombre de secuencial :D
            printf("\n\n=== Version Paralelo ===\n");

            //Carga de imagen ocupada de secuencial
            printf("La imagen %s se ha cargado correctamente.\n", nombreImaExtend);
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
                    for(int i=0; i<L; i++){
                        histoImaOPara[i] = 0;
                        nuevoHistoPara[i] = 0;
                    }

                #pragma omp barrier

                //Obtener histograma usando descomposición de dominio/num. de procesadores
                #pragma omp for reduction(+ : histoImaOPara)
                    for(int i=0; i<resolucion; i++)
                        histoImaOPara[imaOriginal[i]]++;

                //Cumulative Distributive Function y CDF min
                #pragma omp single
                    cdfPara[0] = histoImaOPara[0];
                    for(int i=1; i<L; i++){
                        cdfPara[i] = histoImaOPara[i]+cdfPara[i-1];
                        if((cdfPara[i] < cdfPara_min)&&(cdfPara[i] != 0))
                            cdfPara_min = cdfPara[i];
                    }
                    double denominador = resolucion-cdfPara_min;
                    printf("\nValor minimo del cdf: %d\n",cdfPara_min); //Comprobación

                //Formula de ecualización
                #pragma omp for
                    for(int i=0; i<L; i++){
                        double numerador = cdfPara[i]-cdfPara_min;
                        int hv = numerador / denominador * (L-2) + 1;
                        histoEcPara[i] = round(hv);
                    }

                #pragma omp barrier

                //Generar la nueva imagen
                #pragma omp for reduction(+ : nuevoHistoPara)
                    for(int i=0; i<resolucion; i++){
                        imaEcPara[i] = histoEcPara[imaOriginal[i]];
                        nuevoHistoPara[imaEcPara[i]]++;
                    }

            }
            timeEndPara = omp_get_wtime()-timeStartPara;

            //Guardar imagen generada.
            char nombreImaPara[MAXTEXTO];
            strcpy(nombreImaPara,nombreIma);
            strcat(nombreImaPara,"_Para.jpg");

            double timeStartGenerada,timeEndGenerada;
            timeStartGenerada = omp_get_wtime();

            stbi_write_jpg(nombreImaPara, ancho, alto, 1, imaEcPara, 100);

            timeEndGenerada = omp_get_wtime()-timeStartGenerada;
            printf("\nImagen guardada correctamente.\n");

            //Liberar Memoria de la imagen.
            stbi_image_free(imaEcPara);

            //Generacion de archivo csv.
            char ArchivoNombrePara[MAXTEXTO];
            strcpy(ArchivoNombrePara,nombreIma);
            strcat(ArchivoNombrePara,"_Histo_Para.csv");

            double timeStartArchivo,timeEndArchivo;
            timeStartArchivo = omp_get_wtime();

            GuardarCSV(histoImaOPara,nuevoHistoPara,ArchivoNombrePara);

            timeEndArchivo = omp_get_wtime()-timeStartArchivo;
            printf("El archivo se ha creado correctamente.\n");

            //Calculo de métricas
            Metricas(timeEndSec,timeEndPara,numProces);
            OtrosTiempos(timeEndCarga,timeEndGenerada,timeEndArchivo);
            printf("\n\n");
            break;

        case 2: ;
            //Imagen de color a escala de grises.
            char nombreImaColor[MAXTEXTO],nombreImaColorExtend[MAXTEXTO];
            char rutaImagenColor[MAXTEXTO] = "../ImagenesTest/";

            printf("Ingrese el nombre de la imagen jpg con la que se desea trabajar (sin la extensi%cn .jpg): ",162);
            scanf(" %[^\n]",&nombreImaColor);
            strcpy(nombreImaColorExtend,nombreImaColor);
            strcat(nombreImaColorExtend,".jpg");
            strcat(rutaImagenColor,nombreImaColorExtend);
            if (access(rutaImagenColor,F_OK)!= 0){
                printf("Lo sentimos, no existe la imagen %s :c",nombreImaColor);
                return 0;
            }

            //Carga de la imagen con la libreria stb
            int width, height, channels, resolution;

            unsigned char *image = stbi_load(rutaImagenColor, &width, &height, &channels, 0);

            if(image == NULL){
                printf("La imagen %s no pudo ser cargada.\n",nombreImaColorExtend);
                return 0;
            }
            else{
                printf("La imagen %s se ha cargado correctamente.\n", nombreImaColorExtend);
                printf("Datos de la imagen:\n");
                resolution = width*height;
                printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d MegaPixeles\n",width,height,channels,resolution);
            }

            //Unicamente trabaja con imagenes a color de 3 canales.
            if(channels != 3){
                printf("Lo sentimos, necesitamos una imagen de 3 canales para trabajar. :c");
                return 0;
            }

            //Reserva de memoria para la nueva imagen.
            unsigned char *newImage = malloc(resolution);

            //Valor de cada pixel en escala de grises.
            for(int i=0; i<resolution; i++){
                unsigned char red = image[i * channels];
                unsigned char green = image[i * channels+1];
                unsigned char blue = image[i * channels+2];

                newImage[i] = (unsigned char)(0.3 * (float)red + 0.59 * (float)green + 0.11 * (float)blue);
            }

            //Guardar imagen generada.
            char nombreImaGris[MAXTEXTO];
            strcpy(nombreImaGris,nombreImaColor);
            strcat(nombreImaGris,"_Gris.jpg");

            stbi_write_jpg(nombreImaGris, width, height, 1, newImage, 100);
            printf("\nImagen guardada correctamente.\n");

            //Liberar Memoria de la imagen.
            stbi_image_free(newImage);

            printf("\n\n")    ;
            break;
      }
    }while (opcion != 3);
    return 0;
}

/* NOTA
    - Gráficas de histogramas [Original,Ecualizada]
    - Sufijos de las imagenes y CVS generados con el nombre original y Para,Sec
*/
