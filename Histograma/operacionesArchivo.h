#ifndef OPERACIONESARCHIVO_H_INCLUDED
#define OPERACIONESARCHIVO_H_INCLUDED

#define MAXTEXTO 50

int EstadoArchivo(char *archivoNombre[MAXTEXTO]);
void GuardarCSV(int *histoImaO, int *nuevoHisto,char *archivoNombre[MAXTEXTO]);


#endif // OPERACIONESARCHIVO_H_INCLUDED
