#ifndef HISTOGRAMA_H_INCLUDED
#define HISTOGRAMA_H_INCLUDED

#define MAXTEXTO 50

//Lectura de datos desde el teclado.
int manejoCadenas(char *nombreIma, char *nombreImaExtend, char *rutaImagen);

int DistriAcumulada(int* histoImaO,int *cdf);

//Prototipos Comprobación
int VerificarPixeles(int *histoIma);
void ImprimirMatriz(int *HistoEc);
void Metricas(double timeEndSec,double timeEndPara,int numProces);
void OtrosTiempos(double timeEndCarga,double timeEndGenerada,double timeEndArchivo);

#endif // HISTOGRAMA_H_INCLUDED
