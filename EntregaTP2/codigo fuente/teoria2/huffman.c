#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "teoria2.h"

tListaCodificacion huff(palFrec palabras[],int cantPalabras,double *largoMedio){
  tListaHuffman auxHuffman,nuevoNodo,lista[cantPalabras];
  tLista aux,primerCode;
  int i;

  for(i=0;i<cantPalabras;i++){
    nuevoNodo = (tListaHuffman) malloc(sizeof(nodoClave));
    strcpy(nuevoNodo->palabra,palabras[i].palabra);
    nuevoNodo->prob = palabras[i].probabilidad;
    nuevoNodo->largo = palabras[i].largo;
    nuevoNodo->izq = NULL;
    nuevoNodo->der = NULL;
    nuevoNodo->codificacion = NULL;
    lista[i] = nuevoNodo;
  }
  //printf("\n\n\n");
  //tListaCodificacion codificacion = NULL;

  huffman(lista,cantPalabras);

  primerCode = (tLista)malloc(sizeof(nodo));
  primerCode = NULL;
  codificar(lista[0],primerCode);
  tListaCodificacion codificacion,auxCodificacion;
  codificacion = NULL;
  limpiarVector(lista[0],&codificacion);
  auxCodificacion = codificacion;
  *largoMedio = 0;
  while(auxCodificacion!=NULL){
    *largoMedio += calcularLargo(auxCodificacion->codificacion,auxCodificacion->probabilidad);
    //printf("%f ",(double)calcularLargo(auxCodificacion->codificacion,auxCodificacion->probabilidad));
    auxCodificacion = auxCodificacion->sig;
  }
  return codificacion;
}

void huffman(tListaHuffman nodos[],int n){
  tListaHuffman nuevoNodo;
  if(n>1){
    nuevoNodo = (tListaHuffman) malloc(sizeof(nodoClave));
    quicksortHuffman(nodos,0,n-1); //ordeno probabilidades con sus claves
    strcpy(nuevoNodo->palabra,"");
    nuevoNodo->izq = nodos[0];
    nuevoNodo->der = nodos[1];
    nuevoNodo->prob = nodos[0]->prob + nodos[1]->prob; //fusiono a los de menor probabilidad
    nuevoNodo->codificacion = NULL;
    nodos[0] = nuevoNodo;
    for(int i=1;i<n-1;i++){ //acomodo la lista
      nodos[i] = nodos[i+1];
    }
    n--;
    huffman(nodos,n);
  }
}

void quicksortHuffman(tListaHuffman lista[],int limite_izq,int limite_der){
    int izq,der;
    tListaHuffman pivote,temporal;
    izq=limite_izq;
    der = limite_der;
    pivote = lista[(izq+der)/2];
    do{
        while((lista[izq]->prob)<(pivote->prob) && (izq < limite_der))izq++;
        while((pivote->prob)<(lista[der]->prob) && (der > limite_izq))der--;
        if(izq <= der)
        {
            temporal = lista[izq];
            lista[izq]=lista[der];
            lista[der]=temporal;
            izq++;
            der--;
        }
    }while(izq<=der);
    if(limite_izq<der){quicksortHuffman(lista,limite_izq,der);}
    if(limite_der>izq){quicksortHuffman(lista,izq,limite_der);}
}

//CODIFICAR
void codificar(tListaHuffman nodo,tLista code){
  tLista auxDer,auxIzq;
  if(nodo!=NULL){
    if(strcmp(nodo->palabra,"")==0){
      //if(code!=NULL)
        //printf("%d ",code->code);
      auxDer = copiarCode(code);
      //if(code!=NULL)
        //printf("copiado: %d\n",code->k);
      auxIzq = copiarCode(code);
      agregarBitHuffman(&auxDer,1);
      agregarBitHuffman(&auxIzq,0);
      codificar(nodo->der,auxDer);
      codificar(nodo->izq,auxIzq);
    }else{
      nodo->codificacion = code;
      //printf("A");
      //codificaciones[nodo->clave] = code;
    }
  }
}

void agregarBitHuffman(tLista *codi,int bit){
  tLista aux,nuevo;
  aux = *codi;
  if(aux!=NULL){
    while(aux->sig != NULL) //recorre la lista del codigo correspondiente
      aux = aux->sig;
    if(aux->k<16){ //si hay espacio
      aux->code = ((aux->code << 1) & 0xFFFE) + bit; //agrego
      aux->k += 1; //actualizo longitud
    }else{ //creo nodo nuevo y anexo
      //printf("AAAAAA");
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
    //printf("MRS");
    *codi = nuevo;
  }
}

tLista copiarCode(tLista code){ //REVISAR
  tLista auxCode,aux,ant,alter;
  aux = NULL;
  if(code!=NULL){
    aux = (tLista)malloc(sizeof(nodo));
    aux->code = code->code;
    aux->k = code->k;
    aux->sig = NULL;
    ant = aux;
    auxCode = code->sig;
    while(auxCode!=NULL){
      alter = (tLista)malloc(sizeof(nodo));
      alter->code = auxCode->code;
      alter->k = auxCode->k;
      alter->sig = NULL;
      ant->sig = alter;
      ant = alter;
      auxCode = auxCode->sig;
    }
  }
  return aux;
}

void limpiarVector(tListaHuffman nodo,tListaCodificacion *codificaciones){
  tListaCodificacion nuevo;
  if(nodo!=NULL){
    if(strcmp(nodo->palabra,"")!=0){
      nuevo = (tListaCodificacion) malloc(sizeof(nodoCodificacion));
      strcpy(nuevo->palabra,nodo->palabra);
      nuevo->codificacion = nodo->codificacion;
      //printf("%d ",nodo->largo);
      nuevo->probabilidad = nodo->prob;
      nuevo->largo = nodo->largo;
      //printf("%f ",nuevo->probabilidad);
      nuevo->sig = *codificaciones;
      //printf("%s ",nuevo->palabra);
      *codificaciones = nuevo;
    }
    limpiarVector(nodo->izq,codificaciones);
    limpiarVector(nodo->der,codificaciones);
    free(nodo);
  }
}

void codificarArchivoHuffman(tListaCodificacion codificacion,int cantPalabras){
  int codeado,largo,nuevoK,auxCode,insertar;
  char palabra[MAXPALABRA],nueva[2],car;
  FILE *arch, *salida;
  arch = fopen("texto12.txt","rt");
  salida = fopen("salidaHuffman.dat","wb");
  tLista aux;
  nueva[1] = '\0';
  codeado = largo = 0;
  int acumUnico = 0;
  char cantCar,cantCode;
  tListaCodificacion auxCodificacion = codificacion;
  cantCar = cantPalabras;
  fwrite(&cantCar,sizeof(char),1,salida); //cant de palabras codificadas
  while(auxCodificacion!=NULL){
    cantCar = auxCodificacion->largo;
    //printf("%d ",auxCodificacion->largo);
    acumUnico += auxCodificacion->largo;
    fwrite(&cantCar,sizeof(char),1,salida); //cant caracteres de la palabra
    fwrite(auxCodificacion->palabra,(int)cantCar,1,salida); //palabra
    //printf("%s",auxCodificacion->palabra);
    cantCode = (char)auxCodificacion->codificacion->k;
    fwrite(&cantCode,sizeof(char),1,salida); //cant de bits del code
    fwrite(&(auxCodificacion->codificacion->code),2,1,salida); //code
    auxCodificacion = auxCodificacion->sig;
  }
  //printf("CANT UNICO: %f\n",(double)acumUnico/cantPalabras);
  while(fscanf(arch,"%c",&car)==1){
    strcpy(palabra,"");
    while(!feof(arch) && car!=32 && car!=10){ //no espacio ni salto de linea
      nueva[0] = car;
      strcat(palabra,nueva);
      fscanf(arch,"%c",&car);
    }
    aux = codePalabra(codificacion,palabra);//buscar codificacion de la palabra
    if(largo+(aux->k) <= 16){ //no se pasa de bits
      codeado = codeado<<(aux->k);
      codeado = codePostMascaraInverso(codeado,aux->k);
      insertar = codePostMascara(aux->code,aux->k); //a insertar mascara
      //printf("insertar primero: %d\n",insertar);
      codeado = codeado | insertar;
      //printf("codeado primero: %d\n",codeado);
      if(largo+(aux->k) < 16)
        largo = largo + (aux->k);
      else{
        fwrite(&codeado,2,1,salida);
        //printf("codeado: %d\n",codeado);
        codeado = largo = 0;
      }
    }else{ //se pasa de bits / copio lo que puedo en los bits que hay
      nuevoK = 16 - largo;
      auxCode = (aux->code)>>(aux->k - nuevoK);
      auxCode = codePostMascara(auxCode,nuevoK);
      codeado = codeado<<nuevoK;
      codeado = codePostMascaraInverso(codeado,nuevoK);
      codeado = codeado | auxCode;
      //printf("codeado primero: %d\n",codeado);
      //escribir en el archivo
      fwrite(&codeado,2,1,salida);
      //printf("codeado: %d\n",codeado);
      codeado = largo = 0;
      //guardo lo sobrante para despues
      largo = aux->k - nuevoK;
      codeado = aux->code;
      codeado = codePostMascara(codeado,largo);
    }
  }
  fwrite(&codeado,2,1,salida);
  fclose(arch);
  fclose(salida);
}

void mostrarArbol(tListaHuffman nodo){
  if(nodo!=NULL){
    if(strcmp(nodo->palabra,"")!=0)
      printf("%s %f %d| ",nodo->palabra,nodo->prob,nodo->codificacion->code);
    mostrarArbol(nodo->izq);
    mostrarArbol(nodo->der);
  }
}

void recorrerLista(tListaCodificacion codificaciones){
  tListaCodificacion aux = codificaciones;
  while(aux!=NULL){
    printf("%s %d",aux->palabra,aux->codificacion->code);
    aux = aux->sig;
  }
}
