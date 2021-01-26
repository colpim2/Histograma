#ifndef OPERACIONESARCHIVO_H_INCLUDED
#define OPERACIONESARCHIVO_H_INCLUDED

#define MAXTEXTO 50

typedef struct{
   int tm_sec;
   int tm_min;
   int tm_hour;
   int tm_mday;
   int tm_mon;
   int tm_year;
   int tm_wday;
   int tm_yday;
   int tm_isdst;
}tm;

int EstadoArchivo(char *archivoNombre[MAXTEXTO]);
//void LeerArchivo(char *archivoNombre[MAXTEXTO],Empresa *empresa);
//void GuardarArchivo(Empresa empresa);
//void GuardarArchivoUsuario(Empresa empresa);

#endif // OPERACIONESARCHIVO_H_INCLUDED
