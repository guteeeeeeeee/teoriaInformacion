#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "teoria2.h"

void descomprimir(char metodo[MAXPALABRA]){
  char nombre[MAXPALABRA];
  strcpy(nombre,"salida.");
  if(strcmp(metodo,"Shannon")==0)
    strcat(nombre,"Fan");
  else
    strcat(nombre,"Huf");
  FILE *arch = fopen(nombre,"rb");
  //FILE *arch = fopen("salidaHuffman.dat","rb");
  //FILE *arch = fopen("salidaShannon.dat","rb");
  char caracter,palabra[MAXPALABRA],aux[2],codi[MAXPALABRA];
  int cantPalabras,entero,k;
  aux[1] = '\0';
  tDiccionario diccionario,nuevo;
  diccionario = NULL;
  strcpy(palabra,"");
  short codiiii,codigoLeido;
  //leo tabla
  fread(&cantPalabras,sizeof(int),1,arch);//cantidad de palabras
  //printf("cant: %d\n",cantPalabras);
  for(int i=0;i<cantPalabras;i++){//armo diccionario
    fread(&caracter,sizeof(char),1,arch);//cant caracteres de palabra
    //printf("largo palabra %d\n",(int)caracter);
    entero = (int)caracter;
    for(int b=0;b<entero;b++){//cantidad de caracteres
      fread(&caracter,sizeof(char),1,arch);
      aux[0] = caracter;
      strcat(palabra,aux);
      //printf("%c",caracter);
    }
    //printf("palabra: %s\n",palabra);
    fread(&caracter,sizeof(char),1,arch);//cant de bits del code
    k = (int)caracter;
    //printf("cantidad code: %d\n",k);
    fread(&codiiii,2,1,arch); //codigo
    nuevo = (tDiccionario)malloc(sizeof(nodoDicc));//nuevo nodo de diccionario
    //printf("numero:        %d\n",codiiii);
    strcpy(nuevo->palabra,palabra);
    //printf("%s ",palabra);
    strcpy(palabra,"");
    bitsToString(k,codiiii,codi);
    strcpy(nuevo->codificacion,codi);
    //printf("%s\n",nuevo->codificacion);
    //printf("palabra: %s codif: %s",nuevo->palabra,nuevo->codificacion);
    if(i==0)
      nuevo->sig = NULL;
    else
      nuevo->sig = diccionario;
    diccionario = nuevo;
    //de bits a string
  }
  strcpy(nombre,"descompresion");
  strcat(nombre,metodo);
  strcat(nombre,".txt");
  FILE *salida = fopen(nombre,"wt");
  char leida[MAXPALABRA];
  int cantidad,titular,suplente,acum,leo;
  short primera,segunda;
  acum = 0;
  fread(&titular,sizeof(int),1,arch); //relleno titular
  fread(&suplente,sizeof(int),1,arch); //relleno suplente
  fread(&leo,sizeof(int),1,arch); //tiene que leer (int)
  primera = (short) codePostMascaraMejorado(leo>>16,16);
  segunda = (short) codePostMascaraMejorado(leo,16);
  //printf("titular: %d suplente: %d leo: %d\n",titular,suplente,leo);
  int band = 0;
  while(!feof(arch)){
    while(acum<=16){
      quePalabra(&titular,diccionario,leida,&cantidad);//cantidad te dice cuantos bits son el largo del codigo leido
      //printf("titular: %d\n",titular);
      //printf("%s ",leida);
      fprintf(salida,"%s ",leida);
      rellenarTitular(&titular,cantidad,&suplente); //suplente queda << cantidad
      //printf("titular rellenado: %d suplente post: %d\n",titular,suplente);
      acum += cantidad;
      //printf("acum: %d\n",acum);
    }
    //printf("fin primer while\n");
    //printf("primera: %d\n",primera);
    rellenarSuplente(primera,&acum,&suplente); //rellenar suplente NUNCA RELLENA CON LA SEGUNDA
    primera = 0;
    //printf("suplente rellenado: %d\n",suplente);
    //printf("acum: %d\n",acum);
    while(acum<=16){
      quePalabra(&titular,diccionario,leida,&cantidad);//cantidad te dice cuantos bits son el largo del codigo leido
      //printf("titular: %d\n",titular);
      //printf("leida: %s\n",leida);
      fprintf(salida,"%s ",leida);
      rellenarTitular(&titular,cantidad,&suplente); //suplente queda << cantidad
      //printf("titular rellenado: %d suplente post: %d\n",titular,suplente);
      acum += cantidad;
      //printf("acum: %d\n",acum);
    }
    //printf("fin segundo while\n");
    if(band==0){
      rellenarSuplente(segunda,&acum,&suplente); //rellenar suplente
      segunda = 0;
      //printf("suplente rellenado: %d\n",suplente);
      while(acum<=16){
        quePalabra(&titular,diccionario,leida,&cantidad);//cantidad te dice cuantos bits son el largo del codigo leido
        //printf("titular: %d\n",titular);
        //printf("leida: %s\n",leida);
        fprintf(salida,"%s ",leida);
        rellenarTitular(&titular,cantidad,&suplente); //suplente queda << cantidad
        //printf("titular rellenado: %d suplente post: %d\n",titular,suplente);
        acum += cantidad;
      }
    }
    if(!feof(arch)){
      fread(&leo,sizeof(int),1,arch); //tiene que leer (int)
      //leo lo divido en 2 shorts
      primera = (short) codePostMascaraMejorado(leo>>16,16);
      segunda = (short) codePostMascaraMejorado(leo,16);
      rellenarSuplente(primera,&acum,&suplente);//relleno suplente
      //printf("suplente rellenado: %d\n",suplente);
      primera = segunda;
      band = 1;
    }
    //printf("CICLO\n");
  }
  //printf("despues\n");
  //printf("sale titular: %d suplente: %d\n",titular,suplente);
  while(acum<=16){
    //printf("ENTRAAAAA");
    quePalabra(&titular,diccionario,leida,&cantidad);//cantidad te dice cuantos bits son el largo del codigo leido
    //printf("titular: %d\n",titular);
    //printf("suplente: %d\n",suplente);
    //printf("leida: %s\n",leida);
    fprintf(salida,"%s ",leida);
    rellenarTitular(&titular,cantidad,&suplente); //suplente queda << cantidad
    //printf("titular relleno: %d suplente post: %d\n",titular,suplente);
    acum += cantidad;
  }
  quePalabra(&titular,diccionario,leida,&cantidad);//cantidad te dice cuantos bits son el largo del codigo leido
  fprintf(salida,"%s ",leida);
  fclose(arch);
  fclose(salida);
}

void rellenarSuplente(short leido,int *cant,int *suplente){ //fijarme cant
  int aux = (int) leido;//<< suplente
  aux = codePostMascaraMejorado(aux,16); //mascara
  aux = aux << (*cant-16);
  aux = codePostMascaraInversoMejorado(aux,(*cant-16));
  //*suplente = *suplente << *cant; //reacomodar el suplente
  //*suplente = codePostMascaraInversoMejorado(*suplente,*cant);
  *suplente = *suplente | aux; //revisar OR o AND
  *cant -= 16;
}

void rellenarTitular(int *code,int cant,int *suplente){ //es rellenar el code con los bits suplentes
  //printf("codigo: %d\n",*code);
  int aux;
  *code = (*code) << cant; //el codigo ya usado lo elimino
  *code = codePostMascaraInversoMejorado(*code,cant);
  aux = *suplente >> (32 - cant);//OJO REVISAR //le agrego el codigo viejo
  aux = codePostMascaraMejorado(aux,cant);
  *code = *code | aux; //queda relleno de nuevo
  //reacomodar el suplente
  *suplente = *suplente<<cant; //reacomodar el suplente
  *suplente = codePostMascaraInversoMejorado(*suplente,cant);
}

void rellenarTitular22(int *code,int cant,int *suplente){ //es rellenar el code con los bits suplentes TITULAR POSTA
  //printf("codigo: %d\n",*code);
  int nuevo;
  *code = (*code) << cant; //el codigo ya usado lo elimino
  *code = codePostMascaraInversoMejorado(*code,cant);
  *suplente = *suplente >> cant; //le agrego el codigo viejo
  *suplente = codePostMascaraMejorado(*suplente,cant);
  *code = *code | *suplente; //queda relleno de nuevo
  nuevo = *suplente<<cant; //reacomodar el suplente
  nuevo = codePostMascaraInversoMejorado(nuevo,cant);
}

void quePalabra(int *code,tDiccionario diccionario,char palabra[MAXPALABRA],int *cant){
  tDiccionario aux = diccionario;
  char codigoLlega[MAXPALABRA];
  //coincida el code en alguna longitud
  bitsToStringInversa(strlen(aux->codificacion),*code,codigoLlega);
  //printf("llega: %s\n",codigoLlega);
  while(aux!=NULL && strcmp(codigoLlega,aux->codificacion)!=0){
    //printf("%s\n",aux->codificacion);
    aux = aux->sig;
    if(aux!=NULL)
      bitsToStringInversa(strlen(aux->codificacion),*code,codigoLlega);
  }
  //printf("codigo encontrada: %s\n",aux->codificacion);
  //printf("palabra: %s\n",aux->palabra);
  *cant = strlen(aux->codificacion);
  strcpy(palabra,aux->palabra);
}

void bitsToStringInversa(int cant,int codigo,char codi[MAXPALABRA]){
  char car,palabra[MAXPALABRA],palAux[2];
  int aux,k,ultimoBit;
  palAux[1] = '\0';
  strcpy(palabra,"");
  //printf("cant: %d codigo: %d",cant,codigo);
  k = 32-cant;
  aux = codigo;
  aux = aux >> k;
  for(int j=cant;j>0;j--){
    ultimoBit = aux >> (j-1);
    car = codePostMascara(ultimoBit,1);
    car = car & 0x0000FFFF;
    car = car + 48;
    //printf("%c",car);
    palAux[0] = car;
    strcat(palabra,palAux);
  }
  strcpy(codi,palabra);
  //printf("%s",palabra);
}

void bitsToString(int cant,int codigo,char codi[MAXPALABRA]){//palabra
  char car,palabra[MAXPALABRA],palAux[2];
  int aux,k,ultimoBit;
  palAux[1] = '\0';
  strcpy(palabra,"");
  //printf("cant: %d codigo: %d",cant,codigo);
  k = cant;
  while(k>0){
    aux = codigo;
    aux = aux >> (k - 1);
    ultimoBit = codePostMascara(aux,1);
    ultimoBit = ultimoBit & 0x0000FFFF;
    car = ultimoBit + 48;
    //printf("%c",car);
    palAux[0] = car;
    strcat(palabra,palAux);
    k--;
  }
  strcpy(codi,palabra);
  //printf("%s",palabra);
}
