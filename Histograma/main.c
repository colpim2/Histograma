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
    //Cambiar a ingresado por el usuario usar arg
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
        resolucion = ancho*alto;
        printf("Ancho: %d\nAlto: %d\nNumero de Canales: %d\nResolucion %d MegaPixeles\n",ancho,alto,nCanales,resolucion);
    }

    //Histograma generado de manera Secuencial.
    int histoImaO[L];
    histo_secuencial(histoImaO,imaOriginal,resolucion);

    //Verificación de pixeles
    printf("\nHistograma de la Imagen Original:\n");
    printf("\nContador: %d\n",VerificarPixeles(histoImaO));

    //Generacion el cdf (Cumulative Distributive Function)
    int cdf[L];
    DistriAcumulada_secuencial(histoImaO,cdf);

    //Busqueda del valor minimo
    int cdf_min = Minimo_secuencial(cdf);
    printf("\nValor minimo del cdf: %d\n",cdf_min);

    //Generacion  del histograma ecualizado.
    //Función?
    int histoEc[L];
    for(int i=0; i<L; i++){
        double numerador = cdf[i]-cdf_min;
        double denominador = resolucion-cdf_min;
        int hv = numerador / denominador * (L-2) + 1;
        histoEc[i] = round(hv);
    }

    //Comprobación
    printf("\nHISTOGRAMA ECUALIZADO:\n");
    ImprimirMatriz(histoEc);

    //Generacion del arreglo de Pixeles para la nueva imagen.
    unsigned char *imaEc = malloc(resolucion * sizeof(unsigned char));

    for(int i=0; i<resolucion; i++){
        imaEc[i] = histoEc[imaOriginal[i]];
    }

    int nuevoHisto[L];
    histo_secuencial(nuevoHisto,imaEc,resolucion);

    //Verificar pixeles nuevo Histograma
    printf("\n\nHistograma de la nueva imagen:\n");
    printf("\nContador: %d\n",VerificarPixeles(nuevoHisto));

    //Guardar imagen generada.
    stbi_write_jpg("nuevaImagen.jpg", ancho, alto, 1, imaEc, 100);
    printf("\nImagen guardada correctamente.\n");

    //Liberar Memoria de la imagen.
    stbi_image_free(imaEc);

    //Generacion de archivo csv.
    GuardarCSV(histoImaO,nuevoHisto);
    printf("El archivo se ha creado correctamente.");

    return 0;

}
