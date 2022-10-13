#include <stdio.h>
#include <stdlib.h>
#include "teoria.h"
#include <math.h>
#include <string.h>

int esErgodica(double mat[][CANTALFABETO],int n){
  int i,j,k,aux[CANTALFABETO][CANTALFABETO];
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      if(i!=j){
        if(mat[i][j]==0)
          aux[i][j] = 99; //no hay camino
        else
          aux[i][j] = 1; //si hay camino
      }else
        aux[i][j] = 0;
    }
  }/*
  for(i=0;i<n;i++){
    for(j=0;j<n;j++)
      printf("%f ",mat[i][j]);
    printf("\n");
  }
  printf("\n");
  printf("\n");
  for(i=0;i<n;i++){
    for(j=0;j<n;j++)
      printf("%d ",aux[i][j]);
    printf("\n");
  }
  printf("\n");
  printf("\n");
  */
  for(k=0;k<n;k++){
    for(i=0;i<n;i++){
      for(j=0;j<n;j++){
        if(aux[i][k] + aux[k][j] < aux[i][j])
          aux[i][j] = 1;
      }
    }
  }
  /*
  for(i=0;i<n;i++){
    for(j=0;j<n;j++)
      printf("%d ",aux[i][j]);
    printf("\n");
  }*/
  i = 0;
  j = 0;
  while(j<n){
    while(i<n){
      if(i!=j && aux[i][j] == 99)
        return 0;
      i++;
    }
    i = 0;
    j++;
  }
  return 1;
}

//CAMBIAR EL 4
void inicializarMatInt(int mat[][4],int n){
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

void decimalToTrinario(int decimal,int orden,int tri[]){
  int k;
  tri[0] = tri[1] = tri[2] = tri[3] = tri[4] = tri[5] = tri[6] = 0;
  k = orden;
  while(decimal>0){
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
  for(k--;k>=0;k--)
    printf("%d",bi[k]);
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

int busquedaBinariaRecursiva( int arreglo[], int busqueda,int izquierda,int derecha){
    if (izquierda > derecha) return -1;

    int indiceDeLaMitad = floor((izquierda + derecha) / 2);

     int valorQueEstaEnElMedio = arreglo[indiceDeLaMitad];
    if (busqueda == valorQueEstaEnElMedio){
        return indiceDeLaMitad;
    }

    if (busqueda < valorQueEstaEnElMedio){
        // Entonces está hacia la izquierda
        derecha = indiceDeLaMitad - 1;
    }else{
        // Está hacia la derecha
        izquierda = indiceDeLaMitad + 1;
    }
    return busquedaBinariaRecursiva(arreglo, busqueda, izquierda, derecha);
}

