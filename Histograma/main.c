#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
            //Obtener ruta de la imagen
            char nombreIma[MAXTEXTO],nombreImaExtend[MAXTEXTO];
            char rutaImagen[MAXTEXTO] = "../ImagenesTest/";

            printf("=== Ecualizacion del Histograma ===\n");
            printf("Ingrese el nombre de la imagen jpg con la que se desea trabajar (sin la extensi%cn .jpg): ",162);
            scanf(" %[^\n]",&nombreIma);
            if(VerificarRuta(nombreIma,nombreImaExtend,rutaImagen) != 0){
                printf("Lo sentimos, no existe la imagen %s :c",nombreImaExtend);
                break;
            }

            //Carga de la imagen con la libreria stb
            int ancho, alto, nCanales, resolucion;
            double timeStartCarga,timeEndCarga;

            //Obtener el tiempo de carga
            timeStartCarga = omp_get_wtime();
            unsigned char *imaOriginal = stbi_load(rutaImagen, &ancho, &alto, &nCanales, 0);
            timeEndCarga = omp_get_wtime()-timeStartCarga;

            if(imaOriginal == NULL){
                printf("La imagen %s no pudo ser cargada.\n",nombreImaExtend);
                break;
            }

            resolucion = ancho*alto;

            //Arreglo que almacena la informacion de un solo canal.
            if(nCanales == 3){
                unsigned char *imaTemp = malloc(resolucion);
                for(int i=0; i<resolucion; i++){
                    imaTemp[i] = imaOriginal[i*nCanales];
                }
                imaOriginal = imaTemp;
            }

            printf("La imagen %s se ha cargado correctamente.\n", nombreImaExtend);
            printf("Datos de la imagen:\n");
            printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d Pixeles\n",ancho,alto,nCanales,resolucion);


            /* PARTE SECUENCIAL */
            printf("\n=== Version Secuencial ===\n");

            //Histograma generado de manera Secuencial.
            double timeStartSec,timeEndSec;

            timeStartSec = omp_get_wtime();

            int histoImaO[L];
            int nuevoHisto[L];

            //Inicialización del los histogramas de la imagen original y de la imagen que se generará
            for(int i=0; i<L; i++){
                histoImaO[i] = 0;
                nuevoHisto[i] = 0;
            }

            //Cálculo del histograma de la imagen original.
            for(int i=0; i<resolucion; i++)
                histoImaO[imaOriginal[i]]++;

            //Generacion el cdf (Cumulative Distributive Function)
            int cdf[L];
            DistriAcumulada(histoImaO,cdf);

            //Busqueda del valor minimo
            int cdf_min = Minimo(cdf);
            printf("\nValor minimo del cdf: %d\n",cdf_min);

            //Generacion  del histograma ecualizado.
            int histoEc[L];
            for(int i=0; i<L; i++){
                double numerador = cdf[i]-cdf_min;
                double denominador = resolucion-cdf_min;
                int hv = numerador / denominador * (L-2) + 1;
                histoEc[i] = round(hv);
            }

            //Generacion del arreglo de Pixeles para la nueva imagen.
            unsigned char *imaEc = malloc(resolucion * sizeof(unsigned char));

            for(int i=0; i<resolucion; i++){
                imaEc[i] = histoEc[imaOriginal[i]];
                nuevoHisto[imaEc[i]]++;
            }

            timeEndSec = omp_get_wtime()-timeStartSec;

            //Guardar imagen generada.
            char nombreImaSec[MAXTEXTO];
            strcpy(nombreImaSec,nombreIma);
            strcat(nombreImaSec,"_Sec.jpg");

            printf("Guardando nueva imagen - Secuencial...\n");
            stbi_write_jpg(nombreImaSec, ancho, alto, 1, imaEc, 100);
            printf("\nImagen guardada correctamente.\n");

            //Liberar Memoria de la imagen.
            stbi_image_free(imaEc);

            //Generacion de archivo csv.
            char ArchivoNombreSec[MAXTEXTO];
            strcpy(ArchivoNombreSec,nombreIma);
            strcat(ArchivoNombreSec,"_Histo_Sec.csv");

            GuardarCSV(histoImaO,nuevoHisto,ArchivoNombreSec);
            printf("El archivo .csv con la informacion de los Histogramas se ha creado correctamente.\n");


            /* PARTE PARALELA */

            //Se ocupa la ruta y nombre de secuencial
            printf("\n\n=== Version Paralelo ===\n");

            //Variables compartidas
            int histoImaOPara[L],cdfPara[L],histoEcPara[L],nuevoHistoPara[L];
            int cdfPara_min = 100000;

            //Imagen Ecualizada
            unsigned char *imaEcPara = malloc(resolucion * sizeof(unsigned char));

            //Obtener numero de procesadores
            int numProces = omp_get_num_procs();

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

                //Obtener histograma usando descomposición de dominio
                #pragma omp for reduction(+ : histoImaOPara)
                    for(int i=0; i<resolucion; i++)
                        histoImaOPara[imaOriginal[i]]++;

                //Cumulative Distributive Function y CDF min
                #pragma omp single
                {
                    cdfPara[0] = histoImaOPara[0];
                    for(int i=1; i<L; i++)
                    {
                        cdfPara[i] = histoImaOPara[i]+cdfPara[i-1];
                        if((cdfPara[i] < cdfPara_min)&&(cdfPara[i] != 0))
                            cdfPara_min = cdfPara[i];
                    }
                    printf("\nValor minimo del cdf: %d\n",cdfPara_min); //Comprobación
                 }

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

            printf("Guardando nueva imagen - Paralelo...\n");

            //Obtener tiempo de generación
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

            //Obtener tiempo de archivo
            double timeStartArchivo,timeEndArchivo;
            timeStartArchivo = omp_get_wtime();

            GuardarCSV(histoImaOPara,nuevoHistoPara,ArchivoNombrePara);

            timeEndArchivo = omp_get_wtime()-timeStartArchivo;
            printf("El archivo .csv con la informacion de los Histogramas se ha creado correctamente.\n");

            //Calculo de métricas
            Metricas(timeEndSec,timeEndPara,numProces);
            OtrosTiempos(timeEndCarga,timeEndGenerada,timeEndArchivo);
            printf("\n\n");
            break;

        case 2: ;
            //Imagen de color a escala de grises.
            char nombreImaColor[MAXTEXTO],nombreImaColorExtend[MAXTEXTO];
            char rutaImagenColor[MAXTEXTO] = "../ImagenesTest/";

            printf("=== Imagen Color a Gris ===\n");
            printf("Ingrese el nombre de la imagen jpg con la que se desea trabajar (sin la extensi%cn .jpg): ",162);
            scanf(" %[^\n]",&nombreImaColor);
            if(VerificarRuta(nombreImaColor,nombreImaColorExtend,rutaImagenColor)!= 0){
                printf("Lo sentimos, no existe la imagen %s :c",nombreImaExtend);
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
                printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d Pixeles\n",width,height,channels,resolution);
            }

            //Unicamente trabaja con imagenes a color de 3 canales.
            if(channels != 3){
                printf("Lo sentimos, necesitamos una imagen de 3 canales para trabajar. :c");
                break;
            }

            //Reserva de memoria para la nueva imagen.
            unsigned char *newImage = malloc(resolution);

            double timeStartImaColor = omp_get_wtime();

            //Valor de cada pixel en escala de grises.
            for(int i=0; i<resolution; i++){
                unsigned char red = image[i * channels];
                unsigned char green = image[i * channels+1];
                unsigned char blue = image[i * channels+2];

                newImage[i] = (unsigned char)(0.299 * (float)red + 0.587 * (float)green + 0.114 * (float)blue);
            }

            double timeEndImaColor = omp_get_wtime()-timeStartImaColor;

            //Guardar imagen generada.
            char nombreImaGris[MAXTEXTO];
            strcpy(nombreImaGris,nombreImaColor);
            strcat(nombreImaGris,"_Gris.jpg");

            printf("\nGuardando imagen...\n");
            stbi_write_jpg(nombreImaGris, width, height, 1, newImage, 100);
            printf("Imagen guardada correctamente.\n");

            printf("\n\tTiempo de ejecucion: %f segundos.",timeEndImaColor);

            //Liberar Memoria de la imagen.
            stbi_image_free(newImage);

            printf("\n\n")    ;
            break;
      }
    }while (opcion != 3);
    return 0;
}

