#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#include"operacionesArchivo.h"

#define MAXTEXTO 50

int EstadoArchivo(char *archivoNombre[MAXTEXTO]){
FILE *Archivo;
    if( (Archivo = fopen(archivoNombre,"r")) == NULL)
	{
        printf("No existe el archivo %s\n",archivoNombre);
        system("pause");
        return 0;
    }
    else{
        printf("DATOS GUARDADOS...\n");
        system("pause");
        return 1;
    }
}

/*
void LeerArchivo(char *archivoNombre[MAXTEXTO],Empresa *empresa){
  FILE *Archivo;
    Archivo = fopen(archivoNombre,"r");
    if(guardarMemoria(empresa) != 0){

        fgets(empresa->nombre,50,Archivo);
        empresa->nombre[strlen(empresa->nombre) - 1] = '\0';
        fscanf (Archivo,"%lf",&*empresa->mensual->ProduccionM);
        fscanf (Archivo,"%lf",&*empresa->mensual->VentaM);
        fscanf (Archivo,"%lf",&*empresa->anual->ProduccionA);
        fscanf (Archivo,"%lf",&*empresa->anual->VentaA);
        fscanf (Archivo,"%f",&empresa->PorcentajeReciclado);
    }
    fclose(Archivo);
    system("cls");
}

void GuardarArchivo(Empresa empresa){
  FILE *Nuevo;
  char archivoNuevo[MAXTEXTO],Respuesta,Test[MAXTEXTO];
  int i;
    printf("%cCon qu%c nombre quiere guardar sus resultados%c (Incluya la extensi%cn .txt):",168,130,63,162);
    scanf(" %[^\n]",&archivoNuevo);
    if( (Nuevo=fopen(archivoNuevo,"r")) == NULL ){
        Nuevo=fopen(archivoNuevo,"w+t");
        char a[100];
        time_t rawtime;
	   	tm *info;
	   	time( &rawtime );
	   	info = localtime( &rawtime );
	   	strcpy(a,asctime(info));
        fprintf(Nuevo,"%s\n%lf\n%lf\n%lf\n%lf\n%f",empresa.nombre,*empresa.mensual->ProduccionM,*empresa.mensual->VentaM,*empresa.anual->ProduccionA,*empresa.anual->VentaA,empresa.PorcentajeReciclado);
	   	fprintf(Nuevo, "\n\n\nUltima modificacion: %s", a);
        fclose(Nuevo);
        printf("%s Creado \n",archivoNuevo);
    }
    else
    {
        for(i=0;i<MAXTEXTO;i++)
            Test[i] = archivoNuevo[i];
        printf("\n%cDesea sobrescribirlo%c (s/n)",168,63);
        scanf("%s",&Respuesta);
        if(Respuesta == 's')
        {
            Nuevo = fopen(Test,"w+t");
            time_t rawtime;
		   	tm *info;
		   	time( &rawtime );
		   	info = localtime( &rawtime );
       		fprintf(Nuevo,"%s\n%lf\n%lf\n%lf\n%lf\n%f",empresa.nombre,*empresa.mensual->ProduccionM,*empresa.mensual->VentaM,*empresa.anual->ProduccionA,*empresa.anual->VentaA,empresa.PorcentajeReciclado);
	   		fprintf(Nuevo, "\n\n\nUltima modificacion: %s", asctime(info));
			fclose(Nuevo);
            printf("%s Actualizado \n",Test);
            system("pause");
        }
    }
}

void GuardarArchivoUsuario(Empresa empresa){
  FILE *Nuevo;
  char archivoNuevo[MAXTEXTO],Respuesta,Test[MAXTEXTO];
  int i;
    strcpy(archivoNuevo,empresa.nombre);
    strcat(archivoNuevo,"usuario.txt");
    if( (Nuevo=fopen(archivoNuevo,"r")) == NULL ){
        Nuevo=fopen(archivoNuevo,"w+t");
        char a[100];
        time_t rawtime;
	   	tm *info;
	   	time( &rawtime );
	   	info = localtime( &rawtime );
	   	strcpy(a,asctime(info));
        fprintf(Nuevo,"Nombre empresa:%s\n\nDATOS MENSUALES\nCantidad de plastico producido al mes [TONELADAS]: %lf\n\nCantidad de plastico vendido al mes [TONELADAS]: %lf\n\n\nDATOS ANUALES\nCantidad de plastico producido al ano [TONELADAS]: %lf\n\nCantidad de plastico vendido al ano [TONELADAS]: %lf\n\nPorcentaje de plastico reciclado al ano: %f",empresa.nombre,*empresa.mensual->ProduccionM,*empresa.mensual->VentaM,*empresa.anual->ProduccionA,*empresa.anual->VentaA,empresa.PorcentajeReciclado);
	   	fprintf(Nuevo, "\n\n\nUltima modificacion: %s", a);
        fclose(Nuevo);
        printf("%s Creado \n",archivoNuevo);
        system("pause");
    }
    else
    {
        for(i=0;i<MAXTEXTO;i++)
            Test[i] = archivoNuevo[i];
        printf("\n%cDesea sobrescribirlo%c (s/n)",168,63);
        scanf("%s",&Respuesta);
        if(Respuesta == 's')
        {
            Nuevo = fopen(Test,"w+t");
            time_t rawtime;
		   	tm *info;
		   	time( &rawtime );
		   	info = localtime( &rawtime );
       		fprintf(Nuevo,"Nombre empresa:%s\n\nDATOS MENSUALES\nCantidad de plastico producido al mes [TONELADAS]: %lf\n\nCantidad de plastico vendido al mes [TONELADAS]: %lf\n\n\nDATOS ANUALES\nCantidad de plastico producido al ano [TONELADAS]: %lf\n\nCantidad de plastico vendido al ano [TONELADAS]: %lf\n\nPorcentaje de plastico reciclado al ano: %f",empresa.nombre,*empresa.mensual->ProduccionM,*empresa.mensual->VentaM,*empresa.anual->ProduccionA,*empresa.anual->VentaA,empresa.PorcentajeReciclado);
	   		fprintf(Nuevo, "\n\n\nUltima modificacion: %s", asctime(info));
			fclose(Nuevo);
            printf("%s Actualizado \n",Test);
            system("pause");
        }
    }
    system("cls");
}

*/
