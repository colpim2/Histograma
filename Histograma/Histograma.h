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

//Prototipos
void histo_secuencial(int *histo, unsigned char *IMAGEN, int resolucion);
void histo_paralelo();
void histo_reduction();

#endif // HISTOGRAMA_H_INCLUDED
