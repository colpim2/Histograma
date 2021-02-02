"""
Created on Fri 30  2021

@author: Castillo Montes Pamela
@author: Curiel Juarez Josue Adan
"""

import os
import matplotlib.pyplot as plt


def GraficarTiempos(Pixel, Valor, NombreIma, TipoHistograma):
    plt.bar(Pixel, Valor,
            label=TipoHistograma ,color="b")
    plt.xlabel("Pixel")
    plt.ylabel("Valor del Pixel")
    plt.title("Histograma " + NombreIma)
    plt.legend()
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
                
                #Convertir a enteros
                Pixel = int(Registro[0])
                Original = int(Registro[1])
                Ecualizado = int(Registro[2])
                
                ValorPixel.append(Pixel)
                HistogramaOriginal.append(Original)
                HistogramaEcualizado.append(Ecualizado) 
                
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
        GraficarTiempos(ValorPixel, HistogramaOriginal, NombreIma,"ORIGINAL")
        GraficarTiempos(ValorPixel, HistogramaSecuencial, NombreIma,"SECUENCIAL")
        GraficarTiempos(ValorPixel, HistogramaParalelo, NombreIma,"PARALELO")
    else:
        if (HistogramaSecuencial!= 0):
            """ Grafica de tiempos """
            GraficarTiempos(ValorPixel, HistogramaOriginal,NombreIma,"ORIGINAL")
            GraficarTiempos(ValorPixel, HistogramaSecuencial,NombreIma,"SECUENCIAL")
    

Main()

