#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "teoria2.h"

void procesar(){
  int i,indice,cont,largoPalabra,acumLargo,p; //bajar mayusculas?
  palFrec palabras[10000],nuevaPalabra;
  char car,nueva[2],palabra[MAXPALABRA],caracteres[1000];
  double entropia,largoMedio,entropia2;
  FILE *arch = fopen("texto12.txt","rt");
  FILE *salida = fopen("parte1.txt","wt");
  i = p = cont = acumLargo = 0;
  nueva[1] = '\0';
  while(fscanf(arch,"%c",&car)==1){
    strcpy(palabra,"");
    largoPalabra = 0;
    while(!feof(arch) && car!=32 && car!=10){ //32
      //printf("CHAR: %c %d",car,car);
      if(!estaCaracter(caracteres,p,car))
        caracteres[p++] = car;
      largoPalabra++;
      nueva[0] = car;
      strcat(palabra,nueva);
      fscanf(arch,"%c",&car);
    }
    cont++; //contador de palabras leidas
    acumLargo += largoPalabra;
    indice = estaPalabra(palabras,i,palabra);
    if(indice == -1){ //si no esta la palabra la agrego
      palabras[i].frecuencia = 1;
      palabras[i].largo = largoPalabra;
      strcpy(palabras[i].palabra,palabra);
      //printf("%d ",nuevaPalabra.largo);
      //palabras[i] = nuevaPalabra;
      i++; //i es cantidad de palabras distintas
    }else //si esta aumento frecuencia
      palabras[indice].frecuencia += 1;
    //printf("%s ",palabra);
  }

  largoMedio = entropia = entropia2 = 0;
  for(int k=0;k<i;k++){
    palabras[k].probabilidad = (double)palabras[k].frecuencia / cont;
    largoMedio += (palabras[k].largo * 8) * palabras[k].probabilidad; //para tener en cuenta longitud en bits
    //largoMedio += palabras[k].largo; CON LOS CODIGOS!!
    entropia += palabras[k].probabilidad * (log(1/palabras[k].probabilidad) / log(p));
    entropia2 += palabras[k].probabilidad * (log(1/palabras[k].probabilidad) / log(2));
    //printf("%s %d %f %d\n",palabras[k].palabra,palabras[k].frecuencia,palabras[k].probabilidad,palabras[k].largo); //PARA MOSTRAR
  }
  fprintf(salida,"CANTIDAD DE CARACTERES DISTINTOS: %d\n",p);
  fprintf(salida,"CANTIDAD DE PALABRAS LEIDAS TOTAL: %d\n",cont);
  fprintf(salida,"CANTIDAD DE PALABRAS SIN REPETIR: %d\n",i);
  fprintf(salida,"LARGO MEDIO DE LAS PALABRAS LEIDAS: %f\n",largoMedio);
  fprintf(salida,"entropia fuente: %f\n",entropia);
  fprintf(salida,"SIN CODIFICAR rendimiento: %f redundancia: %f\n",entropia/largoMedio,1-(entropia/largoMedio));
  fprintf(salida,"entropia binaria: %f\n",entropia2);
  largoMedio = 0;
  tListaCodificacion codeHuff = huff(palabras,i,&largoMedio);
  fprintf(salida,"largo medio huffman: %f\n",largoMedio);
  fprintf(salida,"HUFFMAN rendimiento: %f redundancia: %f\n",entropia2/largoMedio,1-(entropia2/largoMedio));
  fclose(arch);
  codificarArchivoHuffman(codeHuff,i);
  largoMedio = 0;
  shannon(palabras,i,&largoMedio);
  fprintf(salida,"largo medio shannon: %f\n",largoMedio);
  fprintf(salida,"SHANNON rendimiento: %f redundancia: %f\n",entropia2/largoMedio,1-(entropia2/largoMedio));
  /*char papa[MAXPALABRA];
  strcpy(papa,"Quijote-;"); //Quijote de
  int a = estaPalabra(palabras,i,papa);
  if(a!=-1){
    printf("\n\n%s %d\n",palabras[a].palabra,palabras[a].frecuencia);
  }else
    printf("no esta");*/
  fclose(salida);
}

int codePostMascara(int codigoLargo, int i){
  int aux,code,codigo;
  code = codigoLargo;
  codigo = 0;
  if(i==1 || i==5 || i==9 || i == 13){
    if(i==1)
      codigo = code & 0x0001; //REVISAR
    else if(i==5)
      codigo = code & 0x001F;
    else if(i==9)
      codigo = code & 0x01FF;
    else
      codigo = code & 0x1FFF;
  }else if(i==2 || i==6 || i==10 || i==14){
    if(i==2)
      codigo = code & 0x0003;
    else if(i==6)
      codigo = code & 0x003F;
    else if(i==10)
      codigo = code & 0x03FF;
    else
      codigo = code & 0x3FFF;
  }else if(i==3 || i==7 || i==11 || i==15){
    if(i==3)
      codigo = code & 0x0007;
    else if(i==7)
      codigo = code & 0x007F;
    else if(i==11)
      codigo = code & 0x07FF;
    else
      codigo = code & 0x7FFF;
  }else
    if(i==4)
      codigo = code & 0x000F;
    else if(i==8)
      codigo = code & 0x00FF;
    else if(i==12)
      codigo = code & 0x0FFF;
    else
      codigo = code & 0xFFFF;
  return codigo;
}

int codePostMascaraInverso(int codigoLargo, int i){//i cuantos 0 se necesitan
  int aux,code,codigo;
  code = codigoLargo;
  codigo = 0;
  if(i==1 || i==5 || i==9 || i == 13){
    if(i==1)
      codigo = code & 0xFFFE;
    else if(i==5)
      codigo = code & 0xFFE0;
    else if(i==9)
      codigo = code & 0xFE00;
    else
      codigo = code & 0xE000;
  }else if(i==2 || i==6 || i==10 || i==14){
    if(i==2)
      codigo = code & 0xFFFC;
    else if(i==6)
      codigo = code & 0xFFC0;
    else if(i==10)
      codigo = code & 0xFC00;
    else
      codigo = code & 0xC000;
  }else if(i==3 || i==7 || i==11 || i==15){
    if(i==3)
      codigo = code & 0x0008;
    else if(i==7)
      codigo = code & 0xFF80;
    else if(i==11)
      codigo = code & 0xF800;
    else
      codigo = code & 0x8000;
  }else
    if(i==4)
      codigo = code & 0xFFF0;
    else if(i==8)
      codigo = code & 0xFF00;
    else if(i==12)
      codigo = code & 0xF000;
    else
      codigo = code & 0x0000;
  return codigo;
}

tLista codePalabra(tListaCodificacion codificacion,char palabra[MAXPALABRA]){
  tListaCodificacion aux = codificacion;
  while(aux!=NULL && strcasecmp(aux->palabra,palabra)!=0){
    aux = aux->sig;
  }
  return aux->codificacion;
}

double calcularLargo(tLista codificacion,double prob){
  double acum = 0;
  tLista aux = codificacion;
  while(aux!=NULL){
    acum += aux->k;
    //printf("%d ",aux->k);
    aux = aux->sig;
  }
  acum = acum * prob;
  //printf("\n");
  return acum;
}

int estaPalabra(palFrec palabras[10000],int n,char x[MAXPALABRA]){ //devuelve indice y -1 si no esta
  int i = 0;
  while(i<n && strcmp(palabras[i].palabra,x)!=0){
    i++;
  }
  if(i>=n)
    i = -1;
  return i;
}

int estaCaracter(char caracteres[],int n,char x){
  int i = 0;
  while(i<n && caracteres[i]!=x){
    i++;
  }
  return i<n;
}
