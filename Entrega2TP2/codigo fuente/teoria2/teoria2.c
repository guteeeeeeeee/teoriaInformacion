#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "teoria2.h"

void procesar(){
  int i,indice,cont,largoPalabra,acumLargo,p;
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
    largoMedio += palabras[k].largo * palabras[k].probabilidad;
    //largoMedio += palabras[k].largo; CON LOS CODIGOS!!
    entropia += palabras[k].probabilidad * (log(1/palabras[k].probabilidad) / log(p));
    entropia2 += palabras[k].probabilidad * (log(1/palabras[k].probabilidad) / log(2));
    //printf("%s %d %f %d\n",palabras[k].palabra,palabras[k].frecuencia,palabras[k].probabilidad,palabras[k].largo); //PARA MOSTRAR
  }
  //fprintf(salida,"CANTIDAD DE CARACTERES DISTINTOS: %d\n",p);
  //printf("CANTIDAD DE PALABRAS LEIDAS TOTAL: %d\n",cont);
  fprintf(salida,"CANTIDAD DE PALABRAS LEIDAS TOTAL: %d\n",cont);
  //printf("CANTIDAD DE PALABRAS SIN REPETIR: %d\n",i);
  fprintf(salida,"CANTIDAD DE PALABRAS SIN REPETIR: %d\n",i);
  fprintf(salida,"LARGO MEDIO DE LAS PALABRAS LEIDAS: %f\n",largoMedio);
  //fprintf(salida,"entropia fuente: %f\n",entropia);
  //fprintf(salida,"SIN CODIFICAR rendimiento: %f redundancia: %f\n",entropia/largoMedio,1-(entropia/largoMedio));
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
      codigo = code & 0x00000001; //REVISAR
    else if(i==5)
      codigo = code & 0x0000001F;
    else if(i==9)
      codigo = code & 0x000001FF;
    else
      codigo = code & 0x00001FFF;
  }else if(i==2 || i==6 || i==10 || i==14){
    if(i==2)
      codigo = code & 0x00000003;
    else if(i==6)
      codigo = code & 0x0000003F;
    else if(i==10)
      codigo = code & 0x000003FF;
    else
      codigo = code & 0x00003FFF;
  }else if(i==3 || i==7 || i==11 || i==15){
    if(i==3)
      codigo = code & 0x00000007;
    else if(i==7)
      codigo = code & 0x0000007F;
    else if(i==11)
      codigo = code & 0x000007FF;
    else
      codigo = code & 0x00007FFF;
  }else
    if(i==4)
      codigo = code & 0x0000000F;
    else if(i==8)
      codigo = code & 0x000000FF;
    else if(i==12)
      codigo = code & 0x00000FFF;
    else
      codigo = code & 0x0000FFFF;
  return codigo;
}

int codePostMascaraMejorado(int codigoLargo, int i){
  int aux,code,codigo;
  code = codigoLargo;
  codigo = 0;
  if(i==1 || i==5 || i==9 || i == 13){
    if(i==1)
      codigo = code & 0x00000001; //REVISAR
    else if(i==5)
      codigo = code & 0x0000001F;
    else if(i==9)
      codigo = code & 0x000001FF;
    else
      codigo = code & 0x00001FFF;
  }else if(i==2 || i==6 || i==10 || i==14){
    if(i==2)
      codigo = code & 0x00000003;
    else if(i==6)
      codigo = code & 0x0000003F;
    else if(i==10)
      codigo = code & 0x000003FF;
    else
      codigo = code & 0x00003FFF;
  }else if(i==3 || i==7 || i==11 || i==15){
    if(i==3)
      codigo = code & 0x00000007;
    else if(i==7)
      codigo = code & 0x0000007F;
    else if(i==11)
      codigo = code & 0x000007FF;
    else
      codigo = code & 0x00007FFF;
  }else if(i==4 || i==8 || i==12 || i==16){
    if(i==4)
      codigo = code & 0x0000000F;
    else if(i==8)
      codigo = code & 0x000000FF;
    else if(i==12)
      codigo = code & 0x00000FFF;
    else
      codigo = code & 0x0000FFFF;
  }else if(i==17 || i==21 || i==25 || i==29){
    if(i==17)
      codigo = code & 0x0001FFFF;
    else if(i==21)
      codigo = code & 0x001FFFFF;
    else if(i==25)
      codigo = code & 0x01FFFFFF;
    else
      codigo = code & 0x1FFFFFFF;
  }else if(i==18 || i==22 || i==26 || i==30){
    if(i==18)
      codigo = code & 0x0003FFFF;
    else if(i==22)
      codigo = code & 0x003FFFFF;
    else if(i==26)
      codigo = code & 0x03FFFFFF;
    else
      codigo = code & 0x3FFFFFFF;
  }else if(i==19 || i==23 || i==27 || i==31){
    if(i==19)
      codigo = code & 0x0007FFFF;
    else if(i==23)
      codigo = code & 0x007FFFFF;
    else if(i==27)
      codigo = code & 0x07FFFFFF;
    else
      codigo = code & 0x7FFFFFFF;
  }else if(i==20 || i==24 || i==28 || i==32){
    if(i==20)
      codigo = code & 0xFFF00000;
    else if(i==24)
      codigo = code & 0xFF000000;
    else if(i==28)
      codigo = code & 0xF0000000;
    else
      codigo = code & 0x00000000;
  }
  return codigo;
}

int codePostMascaraInverso(int codigoLargo, int i){//i cuantos 0 se necesitan
  int aux,code,codigo;
  code = codigoLargo;
  codigo = 0;
  if(i==1 || i==5 || i==9 || i == 13){
    if(i==1)
      codigo = code & 0xFFFFFFFE;
    else if(i==5)
      codigo = code & 0xFFFFFFE0;
    else if(i==9)
      codigo = code & 0xFFFFFE00;
    else
      codigo = code & 0xFFFFE000;
  }else if(i==2 || i==6 || i==10 || i==14){
    if(i==2)
      codigo = code & 0xFFFFFFFC;
    else if(i==6)
      codigo = code & 0xFFFFFFC0;
    else if(i==10)
      codigo = code & 0xFFFFFC00;
    else
      codigo = code & 0xFFFFC000;
  }else if(i==3 || i==7 || i==11 || i==15){
    if(i==3)
      codigo = code & 0xFFFF0008;
    else if(i==7)
      codigo = code & 0xFFFFFF80;
    else if(i==11)
      codigo = code & 0xFFFFF800;
    else
      codigo = code & 0xFFFF8000;
  }else
    if(i==4)
      codigo = code & 0xFFFFFFF0;
    else if(i==8)
      codigo = code & 0xFFFFFF00;
    else if(i==12)
      codigo = code & 0xFFFFF000;
    else
      codigo = code & 0xFFFF0000;
  return codigo;
}

int codePostMascaraInversoMejorado(int codigoLargo, int i){//i cuantos 0 se necesitan HASTA 32
  int aux,code,codigo;
  code = codigoLargo;
  codigo = 0;
  if(i==1 || i==5 || i==9 || i == 13){
    if(i==1)
      codigo = code & 0xFFFFFFFE;
    else if(i==5)
      codigo = code & 0xFFFFFFE0;
    else if(i==9)
      codigo = code & 0xFFFFFE00;
    else
      codigo = code & 0xFFFFE000;
  }else if(i==2 || i==6 || i==10 || i==14){
    if(i==2)
      codigo = code & 0xFFFFFFFC;
    else if(i==6)
      codigo = code & 0xFFFFFFC0;
    else if(i==10)
      codigo = code & 0xFFFFFC00;
    else
      codigo = code & 0xFFFFC000;
  }else if(i==3 || i==7 || i==11 || i==15){
    if(i==3)
      codigo = code & 0xFFFFFFF8;
    else if(i==7)
      codigo = code & 0xFFFFFF80;
    else if(i==11)
      codigo = code & 0xFFFFF800;
    else
      codigo = code & 0xFFFF8000;
  }else if(i==4 || i==8 || i==12 || i==16){
    if(i==4)
      codigo = code & 0xFFFFFFF0;
    else if(i==8)
      codigo = code & 0xFFFFFF00;
    else if(i==12)
      codigo = code & 0xFFFFF000;
    else
      codigo = code & 0xFFFF0000;
  }else if(i==17 || i==21 || i==25 || i==29){
    if(i==17)
      codigo = code & 0xFFFE0000;
    else if(i==21)
      codigo = code & 0xFFE00000;
    else if(i==25)
      codigo = code & 0xFE000000;
    else
      codigo = code & 0xE0000000;
  }else if(i==18 || i==22 || i==26 || i==30){
    if(i==18)
      codigo = code & 0xFFFC0000;
    else if(i==22)
      codigo = code & 0xFFC00000;
    else if(i==26)
      codigo = code & 0xFC000000;
    else
      codigo = code & 0xC0000000;
  }else if(i==19 || i==23 || i==27 || i==31){
    if(i==19)
      codigo = code & 0xFFF80000;
    else if(i==23)
      codigo = code & 0xFF800000;
    else if(i==27)
      codigo = code & 0xF8000000;
    else
      codigo = code & 0x80000000;
  }else if(i==20 || i==24 || i==28 || i==32){
    if(i==20)
      codigo = code & 0xFFF00000;
    else if(i==24)
      codigo = code & 0xFF000000;
    else if(i==28)
      codigo = code & 0xF0000000;
    else
      codigo = code & 0x00000000;
  }
  return codigo;
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
