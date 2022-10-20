#include <stdio.h>
#include <stdlib.h>
#include "teoria.h"
#include <math.h>
#include <string.h>

void inicializarMatInt(int mat[][CANTALFABETO],int n){
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      mat[i][j] = 0;
}

void inicializarMat(double mat[][CANTALFABETO],int n){
  int i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      mat[i][j] = 0;
}

void inicializarVecDouble(double vec[],int n){
  for(int i=0;i<n;i++)
    vec[i] = 0;
}

void inicializarVecInt(int vec[],int n){
  for(int i=0;i<n;i++)
    vec[i] = 0;
}

void inicializarVecLong(long int vec[],int n){
  for(int i=0;i<n;i++)
    vec[i] = 0;
}

void decimalToTernario(int decimal,int orden,int tri[]){
  int k;
  tri[0] = tri[1] = tri[2] = tri[3] = tri[4] = tri[5] = tri[6] = 0;
  k = orden;
  while(k>=0 && decimal>0){
    tri[--k] = decimal % 3;
    decimal = decimal / 3;
  }
  //for(k=0;k<orden;k++)
    //printf("%c",tri[k]+'A');
}

void inicializarCodificaciones(codif codificaciones[],int n){
  for(int i=0;i<n;i++)
    strcpy(codificaciones[i],"");
}

void decimalToBinario(long int decimal){
  int k,bi[20];
  k = 0;
  while(decimal>0){
    bi[k++] = decimal % 2;
    decimal = decimal / 2;
  }
  //for(k--;k>=0;k--)
    //printf("%d",bi[k]);
}

void quicksort(double lista[],int limite_izq,int limite_der,int claves[]){
    int izq,der,auxClave;
    double temporal,pivote;
    izq=limite_izq;
    der = limite_der;
    pivote = lista[(izq+der)/2];
    do{
        while(lista[izq]<pivote && izq<limite_der)izq++;
        while(pivote<lista[der] && der > limite_izq)der--;
        if(izq <=der)
        {
            temporal= lista[izq];
            auxClave = claves[izq];
            lista[izq]=lista[der];
            claves[izq]=claves[der];
            lista[der]=temporal;
            claves[der]=auxClave;
            izq++;
            der--;
        }
    }while(izq<=der);
    if(limite_izq<der){quicksort(lista,limite_izq,der,claves);}
    if(limite_der>izq){quicksort(lista,izq,limite_der,claves);}
}
