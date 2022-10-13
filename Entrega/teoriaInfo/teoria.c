#include <stdio.h>
#include <stdlib.h>
#include "teoria.h"
#include <math.h>
#include <string.h>

void procesarArchivo(){
  double prob[CANTALFABETO],mat[CANTALFABETO][CANTALFABETO];
  FILE *arch = fopen("code.txt","rt");
  int i,j,cont;
  char carAct,carAnt;
  inicializarVecDouble(prob,CANTALFABETO);
  inicializarMat(mat,CANTALFABETO);
  cont = 0;
  if(!feof(arch)){
    cont++;
    fscanf(arch,"%c",&carAnt);
    j = carAnt-'A';
    prob[j] += 1;
  } //prob[0] = prob[A] // prob[1] = prob[B] // prob[2] = prob[C]
  while(fscanf(arch,"%c",&carAct)==1){
    cont++;
    i = carAct-'A';
    j = carAnt-'A';
    mat[i][j] += 1; //mat[i][j] frecuencia de i despues de j
    prob[i] += 1; //prob[i] frecuencia de i
    carAnt = carAct;
  }
  //mostrar
  printf("frecuencia de cada simbolo de la fuente: \n");
  for(i=0;i<CANTALFABETO;i++)
    printf("%c %f ",'A'+i,prob[i]);
  printf("\nfrecuencia de cada i si salio el simbolo j:\n");
  for(i=0;i<CANTALFABETO;i++){
    for(j=0;j<CANTALFABETO;j++)
      printf("%c%c %f | ",'A'+i,'A'+j,mat[i][j]);
    printf("\n");
  }
  //operar
  for(i=0;i<CANTALFABETO;i++)
    for(j=0;j<CANTALFABETO;j++)
      mat[i][j] = (double) mat[i][j] / prob[j];//(cont - 1); //matriz con probabilidades condicionales MAT[A][B] = P(A/B)
  for(i=0;i<CANTALFABETO;i++)
    prob[i] = (double) prob[i] / cont; //probabilidad de cada simbolo de manera independiente
  //mostrar
  printf("probabilidad de cada simbolo de manera independiente:\n");
  for(i=0;i<CANTALFABETO;i++)
    printf("%c %f ",'A'+i,prob[i]);
  printf("\nprobabilidades condicionales:\n");
  for(i=0;i<CANTALFABETO;i++){
    for(j=0;j<CANTALFABETO;j++)
      printf("%c%c %f | ",'A'+i,'A'+j,mat[i][j]);
    printf("\n");
  }
  //memoria nula
  if(!esMemoriaNula(prob,mat,CANTALFABETO)){
    printf("La fuente no es de memoria nula\n");
    if(esErgodica(mat,CANTALFABETO)){
      printf("La fuente es ergodica\n");
      printf("Entropia de la fuente: %f bits\n",entropiaFuente(mat,CANTALFABETO));
    }
  }
  fclose(arch);
}

//mat = matriz con probabilidades condicionales MAT[A][B] = P(A/B)
//prob = vector con la probabilidad de cada simbolo de manera independiente
//n cant de caracteres alfabeto original
int esMemoriaNula(double prob[],double mat[][CANTALFABETO],int n){
  int i,j;
  i = j = 0;
  while(j<n && fabs(mat[i][j] - prob[i]*prob[j]) <= 0.001){
    //si son estadisticamente independientes => P(A/B) = P(A)*P(B) (se le da cierto margen al ser iteraciones finitas)
    i++;
    if(i>=n){
      i = 0;
      j++;
    }
  }
  return !(j<n);
}

double entropiaFuente(double mat[][CANTALFABETO],int n){
  double acumTotal,acumParcial,vecEstacionario[CANTALFABETO];
  vecEstacionario[0]= 0.5160244;
  vecEstacionario[1]= 0.2869584;
  vecEstacionario[2]= 0.1773729;
  /*for(int i =0;i<n;i++){
    for(int j=0;j<n;j++){
      printf("%f ",mat[i][j]);
    }
    printf("\n");
  }*/
  acumTotal = 0;
  for(int j=0;j<n;j++){
    acumParcial = 0;
    for(int i=0;i<n;i++){
     if(mat[i][j]!=0)
      acumParcial += mat[i][j]*(log(1/mat[i][j]) / log(2));
    }
    acumTotal += acumParcial * vecEstacionario[j];
  }
  return acumTotal;
}

//SEGUNDA PARTE
void procesarPalabras(int orden){
  double prob[(int)pow(CANTALFABETO,orden)];
  int i,ind,cont,n;
  n = (int)pow(CANTALFABETO,orden);
  int largo[n];
  char pal[orden + 1];
  double cantInfo,entropia;
  cont = cantInfo = entropia = 0;
  inicializarVecInt(largo,n);
  inicializarVecDouble(prob,n);
  FILE *arch = fopen("code.txt","rt");
  printf("\norden %d\n",orden);
  while(!feof(arch)){
    i = 0;
    strcpy(pal,"");
    while(!feof(arch) && i<orden)//lee las palabras
      fscanf(arch,"%c",&pal[i++]);
    pal[i] = '\0';
    if(i==orden){ //si son del largo deseado
      cont++; //cant de palabras
      ind = 0; //indice vector prob y largo
      //printf("%s ",pal);
      for(i=0;i<orden;i++)
       ind += (pal[i]-'A') * pow(3,orden-i-1); //de trinario a decimal
      prob[ind] += 1; //frecuencia
    }
  }
  for(i=0;i<n;i++){
    largo[i] = orden; //largo de las palabras
    prob[i] = prob[i] / cont;
    if(prob[i]!=0){
      cantInfo += log(1/prob[i]) / log(2);
      entropia += prob[i] * (log(1/prob[i]) / log(2));
    }
  }/*
  for(int k=0;k<n;k++){
    decimalToTrinario(k,orden,trinario);
    printf(" %f ",prob[k]);
  }*/
  printf("\ncantidad informacion: %f bits entropia: %f bits\n",cantInfo,entropia);
  if(cumpleKraft(largo,n))
    printf("cumple Kraft y McMillan\n");
  else
    printf("no cumple Kraft y McMillan\n");
  printf("longitud media: %f\n",longitudMedia(largo,prob,n));
  if(esCompacto(largo,prob,n))
    printf("es compacto\n");
  else
    printf("no es compacto\n");
  //printf("compacto: %d\n",esCompacto(largo,prob,n));
  printf("rendimiento: %f redundancia: %f\n",calcularRendimiento(entropia,longitudMedia(largo,prob,n)),
         1-calcularRendimiento(entropia,longitudMedia(largo,prob,n)));
  shannon(prob,orden);
  fclose(arch);
}

int cumpleKraft(int largo[],int k){ //calcula si es posible un codigo instantaneo con esas longitudes de codigo
  double acum = 0;
  for(int i=0;i<k;i++)
    acum += pow(CANTALFABETO,(-1)*largo[i]);
  return acum<=1;
}

double longitudMedia(int largo[],double prob[],int k){ //k es cant de posibles palabras distintas
  double acum = 0;
  for(int i=0;i<k;i++)
    acum += prob[i] * largo[i];
  return acum;
}

int esCompacto(int largo[],double prob[],int k){ //log(1/prob[i]) / log(CANTALFABETO)
  int i = 0;
  while(i<k && largo[i] == (int)ceil(log(prob[i]) / log(1/CANTALFABETO)))//redondea para arriba
    i++;
  return !(i<k);
}

double calcularRendimiento(double entropia,double largoMedio){
  return entropia / largoMedio;
}

void shannon(double prob[],int orden){
  char nombreArch[20],ordenChar[10];
  int tri[orden],n;
  n = (int)pow(3,orden);
  strcpy(nombreArch,"codificacionOrden");
  ordenChar[0] = orden+48;
  ordenChar[1] = '\0';

  strcat(nombreArch,ordenChar);
  strcpy(ordenChar,".txt");
  strcat(nombreArch,ordenChar);
  FILE *arch = fopen(nombreArch,"wt");
  int i,claves[n];
  codif codificaciones[n];
  for(i=0;i<n;i++)
    claves[i] = i;
  inicializarCodificaciones(codificaciones,n);
  quicksort(prob,0,n-1,claves); //ordena las probabilidades con sus claves
  //for(i=0;i<n;i++)
    //printf("%d %f\n",claves[i],prob[i]);
  shannonFano(prob,0,n-1,codificaciones);
  for(i=0;i<n;i++){
    decimalToTrinario(claves[i],orden,tri);
    for(int k=0;k<orden;k++)
      fprintf(arch,"%c",tri[k]+65);
    fprintf(arch," %s\n",codificaciones[i]);
    //printf("%d %f ",claves[i],prob[i]);
    //printf("%s\n",codificaciones[i]);
  }
  fclose(arch);
}

void shannonFano(double prob[],int i,int j,codif codificaciones[]){//i donde arranca j donde termina
  int x,h;
  double acum,suma;
  char pal[2];
  pal[1] = '\0';
  acum = suma = 0;
  if(i!=j){
    if(i+1 == j){
      pal[0] = '0';
      strcat(codificaciones[i],pal);
      pal[0] = '1';
      strcat(codificaciones[j],pal);
    }else{
      for(h=i;h<=j;h++)
        acum += prob[h];
      h = i;
      pal[0] = '0'; //sqrt(pow(suma+prob[h]-(acum-suma-prob[h]),2))-sqrt(pow(suma-prob[h]-(acum-suma-prob[h]),2))
      while(h<j && fabs(suma+prob[h]-(acum-suma-prob[h]))-fabs(suma-prob[h]-(acum-suma-prob[h])) < 0.000001){ //<=
        strcat(codificaciones[h],pal);
        suma += prob[h++];
      }
      x = h - 1;
      pal[0] = '1';
      while(h<=j)
        strcat(codificaciones[h++],pal);
    shannonFano(prob,i,x,codificaciones);
    shannonFano(prob,x+1,j,codificaciones);
    }
  }
}
