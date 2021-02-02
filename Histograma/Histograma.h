#ifndef HISTOGRAMA_H_INCLUDED
#define HISTOGRAMA_H_INCLUDED

#define MAXTEXTO 50


void DistriAcumulada(int* histoImaO,int *cdf);
int Minimo(int* cdf);

//Prototipos Comprobación
void Metricas(double timeEndSec,double timeEndPara,int numProces);
void OtrosTiempos(double timeEndCarga,double timeEndGenerada,double timeEndArchivo);

#endif // HISTOGRAMA_H_INCLUDED
