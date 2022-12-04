#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "teoria2.h"

void shannon(palFrec palabras[],int cantPalabras,double *largoMedio){
  int i;
  nodoShannon lista[cantPalabras];

  for(i=0;i<cantPalabras;i++){
    strcpy(lista[i].palabra,palabras[i].palabra);
    lista[i].probabilidad = palabras[i].probabilidad;
    lista[i].largo = palabras[i].largo;
    lista[i].codificacion = NULL;
  }

  quicksortShannon(lista,0,cantPalabras-1);
  shannonFano(lista,0,cantPalabras-1);

  *largoMedio = 0;
  for(i=0;i<cantPalabras;i++){
    *largoMedio += calcularLargo(lista[i].codificacion,lista[i].probabilidad);
  }
  int codeado,largo,nuevoK,auxCode,insertar,acumulador;
  char palabra[MAXPALABRA],nueva[2],car,aber[MAXPALABRA];
  FILE *arch,*salida;
  //salida = fopen("salidaShannon.dat","wb");
  salida = fopen("salida.Fan","wb");
  //tabla
  char cantCar,cantCode;
  fwrite(&cantPalabras,sizeof(int),1,salida); //cant de palabras codificadas
  for(int i=0;i<cantPalabras;i++){
    cantCar = lista[i].largo;
    fwrite(&cantCar,sizeof(char),1,salida); //cant caracteres de la palabra
    fwrite(lista[i].palabra,(int)cantCar,1,salida); //palabra
    //printf("%s",lista[i].palabra);
    cantCode = (char)lista[i].codificacion->k;
    //printf("%d ",cantCode);
    fwrite(&cantCode,sizeof(char),1,salida); //cant de bits del code
    fwrite(&(lista[i].codificacion->code),2,1,salida); //code
    //bitsToString(lista[i].codificacion->k,lista[i].codificacion->code,aber);
    //printf("%s %d %s\n",lista[i].palabra,lista[i].codificacion->k,aber); //cantCar
  }
  tLista auxNodo;
  insertar = codeado = largo = 0;
  acumulador = 0;
  nueva[1] = '\0';
  int l,kodi;
  arch = fopen("texto12.txt","rt");
  while(fscanf(arch,"%s",palabra)==1){
    auxNodo = encontrarCode(lista,palabra); //buscar codificacion de la palabra
    if(largo+(auxNodo->k) <= 32){ //no se pasa de bits
      codeado = codeado<<(auxNodo->k);
      codeado = codePostMascaraInversoMejorado(codeado,auxNodo->k);
      insertar = codePostMascaraMejorado(auxNodo->code,auxNodo->k); //a insertar
      //printf("insertar primero: %d\n",insertar);
      codeado = codeado | insertar;
      //printf("codeado primero: %d\n",codeado);
      if(largo+(auxNodo->k) < 32)
        largo = largo + (auxNodo->k);
      else{
        //printf("PRIMERO: %d\n",codeado);
        fwrite(&codeado,sizeof(int),1,salida);
        codeado = largo = 0;
      }
    }else{ //se pasa de bits / copio lo que puedo en los bits que hay
      nuevoK = 32 - largo;
      auxCode = (auxNodo->code)>>(auxNodo->k - nuevoK);
      auxCode = codePostMascaraMejorado(auxCode,nuevoK);
      codeado = codeado<<nuevoK;
      codeado = codePostMascaraInversoMejorado(codeado,nuevoK);
      codeado = codeado | auxCode;
      //escribir en el archivo
      //printf("PRIMERO: %d\n",codeado);
      fwrite(&codeado,sizeof(int),1,salida);
      codeado = largo = 0;
      //guardo lo sobrante para despues
      largo = auxNodo->k - nuevoK;
      codeado = auxNodo->code;
      codeado = codePostMascaraMejorado(codeado,largo);
      //printf("codeado post: %d %d\n",codeado,largo);
    }
  }
  codeado = codeado << (32-largo);
  codeado = codePostMascaraInverso(codeado,32-largo);
  fwrite(&codeado,sizeof(int),1,salida);
  fclose(arch);
  fclose(salida);

  /*char pepe[MAXPALABRA];
  FILE *cap = fopen("cap.txt","rt");
  fscanf(cap,"%s",pepe);
  auxNodo = encontrarCode(lista,pepe); //buscar codificacion de la palabra
  printf("PEPE: %s %d %d\n",pepe,auxNodo->k,auxNodo->code);
  bitsToString(auxNodo->k,auxNodo->code,pepe);
  printf("PEPE: code: %s\n",pepe);
  //En 0111000001s
  //en 110000
  //EN 010101111001
  fclose(cap);*/
}

tLista encontrarCode(nodoShannon lista[],char x[MAXPALABRA]){
  int i = 0;
  while(strcmp(lista[i].palabra,x)!=0)
    i++;
  return lista[i].codificacion;
}

void quicksortShannon(nodoShannon lista[],int limite_izq,int limite_der){
    int izq,der;
    nodoShannon temporal,pivote;
    izq = limite_izq;
    der = limite_der;
    pivote = lista[(izq+der)/2];
    do{
        while(lista[izq].probabilidad<pivote.probabilidad && izq<limite_der)izq++;
        while(pivote.probabilidad<lista[der].probabilidad && der > limite_izq)der--;
        if(izq <=der)
        {
            temporal = lista[izq];
            lista[izq] = lista[der];
            lista[der] = temporal;
            izq++;
            der--;
        }
    }while(izq<=der);
    if(limite_izq<der){quicksortShannon(lista,limite_izq,der);}
    if(limite_der>izq){quicksortShannon(lista,izq,limite_der);}
}

void agregarBit(int i,int bit,nodoShannon codi[]){
  tLista aux,nuevo;
  aux = codi[i].codificacion;
  if(aux!=NULL){
    while(aux->sig != NULL) //recorre la lista del codigo correspondiente
      aux = aux->sig;
    if(aux->k<16){ //si hay espacio
      aux->code = ((aux->code << 1) & 0xFFFE) + bit; //agrego
      aux->k += 1; //actualizo longitud
    }else{ //creo nodo nuevo y anexo
      nuevo = (tLista) malloc(sizeof(nodo));
      nuevo->code = (nuevo->code & 0x0000) + bit;
      nuevo->k = 1;
      nuevo->sig = NULL;
      aux->sig = nuevo;
    }
  }else{
    nuevo = (tLista) malloc(sizeof(nodo));
    nuevo->code = 0x0000 + bit;
    nuevo->k = 1;
    nuevo->sig = NULL;
    codi[i].codificacion = nuevo;
  }
}

void shannonFano(nodoShannon vector[],int i,int j){//i donde arranca j donde termina
  int x,h;
  double acum,suma;
  acum = suma = 0;
  if(i!=j){
    if(i+1 == j){
      agregarBit(i,0,vector);
      agregarBit(j,1,vector);
    }else{
      for(h=i;h<=j;h++)
        acum += vector[h].probabilidad;
      h = i;
      while(h<j && fabs(suma+vector[h].probabilidad-(acum-suma-vector[h].probabilidad))
            -fabs(suma-vector[h].probabilidad-(acum-suma-vector[h].probabilidad)) <= 0.000001){ //<=
        agregarBit(h,0,vector);
        suma += vector[h++].probabilidad;
      }
      x = h - 1;
      while(h<=j){
        agregarBit(h,1,vector);
        h++;
      }
    shannonFano(vector,i,x);
    shannonFano(vector,x+1,j);
    }
  }
}

