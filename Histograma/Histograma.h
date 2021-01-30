#ifndef HISTOGRAMA_H_INCLUDED
#define HISTOGRAMA_H_INCLUDED

#define MAXTEXTO 50

//Lectura de datos desde el teclado.
int manejoCadenas(char *nombreIma, char *nombreImaExtend, char *rutaImagen);

//Prototipos Secuencial
void histo_secuencial(int *histo, unsigned char *IMAGEN, int resolucion);
void DistriAcumulada_secuencial(int* histoImaO,int *cdf);
int Minimo_secuencial(int* cdf);

//Prototipos Paralelo

//Prototipos Comprobación
int VerificarPixeles(int *histoIma);
void ImprimirMatriz(int *HistoEc);
void Metricas(double timeEndSec,double timeEndPara,int numProces);
void OtrosTiempos(double timeEndCarga,double timeEndGenerada,double timeEndArchivo);

#endif // HISTOGRAMA_H_INCLUDED
