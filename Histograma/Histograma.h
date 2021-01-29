#ifndef HISTOGRAMA_H_INCLUDED
#define HISTOGRAMA_H_INCLUDED


//Prototipos Secuencial
void histo_secuencial(int *histo, unsigned char *IMAGEN, int resolucion);
void DistriAcumulada_secuencial(int* histoImaO,int *cdf);
int Minimo_secuencial(int* cdf);

//Prototipos Paralelo

//Prototipos Comprobación
int VerificarPixeles(int *histoIma);
void ImprimirMatriz(int *HistoEc);

#endif // HISTOGRAMA_H_INCLUDED
