"""
Created on Fri 30  2021

@author: Castillo Montes Pamela
@author: Curiel Juarez Josue Adan
"""

import os
import matplotlib.pyplot as plt


def GraficarTiempos(NumDatos, TiemposBusquedaLineal, NombreIma):
    plt.bar(NumDatos, TiemposBusquedaLineal,
            label=NombreIma ,color="b")
    plt.xlabel("Pixel")
    plt.ylabel("Valor del Pixel")
    plt.title("Histograma")
    plt.legend()
    
    #Se eliminan los valores de los ejes dejando solo las etiquetas
    plt.xticks([])
    plt.yticks([])
    plt.show()


def LeerCVS(Archivo):
    if os.path.exists(Archivo):
        
        print("Leyendo archivo: ",Archivo)   #Comprobación
        
        """ Abir Archivo """
        with open(Archivo, 'r') as file:
            lineas = []
            ValorPixel = []
            HistogramaOriginal = []
            HistogramaEcualizado = []
            
            lineas = file.readlines()

            """ Lectura de datos """
            for i in range(1, len(lineas)):
                line = lineas[i]
                Registro = line.split(",")

                ValorPixel.append(Registro[0])
                HistogramaOriginal.append(Registro[1])
                HistogramaEcualizado.append(Registro[2]) 
                
        return HistogramaEcualizado, HistogramaOriginal, ValorPixel
    else:
        print("No existe el archivo ",Archivo)
        return 0,0,0

def Main():
    """ Obtener nombre de la imagen """
    NombreIma = input("Ingrese el nombre de la imagen jpg con la que se desea trabajar (sin la extensión .jpg): ")
    CSVSec = "./" + NombreIma + "_Histo_Sec.csv"
    CSVPara = "./" + NombreIma + "_Histo_Para.csv"
    
    ValorPixel = []
    HistogramaOriginal = []
    HistogramaSecuencial = []
    HistogramaParalelo = []
    
    """ Obtener informacion del archivo .cvs (secuencial) """
    HistogramaSecuencial, HistogramaOriginal, ValorPixel = LeerCVS(CSVSec)
    
    
    """ Obtener informacion del archivo .cvs (paralelo) """
    HistogramaParalelo, Basura, Basura = LeerCVS(CSVPara)
    
    if (HistogramaParalelo != 0):
        """ Grafica de tiempos """
        GraficarTiempos(ValorPixel, HistogramaOriginal,"ORIGINAL")
        GraficarTiempos(ValorPixel, HistogramaSecuencial,"SECUENCIAL")
        GraficarTiempos(ValorPixel, HistogramaParalelo,"PARALELO")
    else:
        if (HistogramaSecuencial!= 0):
            """ Grafica de tiempos """
            GraficarTiempos(ValorPixel, HistogramaOriginal,"ORIGINAL")
            GraficarTiempos(ValorPixel, HistogramaSecuencial,"SECUENCIAL")
    

Main()

