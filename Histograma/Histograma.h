#ifndef HISTOGRAMA_H_INCLUDED
#define HISTOGRAMA_H_INCLUDED

#define numProces 4
#define N 1000
#define NG 256

//Variables Globales
int IMAGEN[N][N];
int histo_sec[NG];
int histo_para[NG];
int histo_redu[NG];

//Prototipos Secuencial
void histo_secuencial(int *histo, unsigned char *IMAGEN, int resolucion);
void DistriAcumulada_secuencial(int* histoImaO,int *cdf);
int Minimo_secuencial(int* cdf);

//Prototipos Paralelo
void histo_paralelo();
void histo_reduction();

//Prototipos Comprobaci�n
int VerificarPixeles(int *histoIma);
void ImprimirMatriz(int *HistoEc);

#endif // HISTOGRAMA_H_INCLUDED
