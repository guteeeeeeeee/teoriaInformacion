#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "teoria2.h"

//parte 2
void calcularEquivocacion(int canal,int cantEntrada,int cantSalida,double probEntrada[],double mat[][cantSalida]){
  int k;
  double probSalida[cantSalida],probCondEntrada[cantSalida][cantEntrada],entropiaPosteriori[cantSalida],
    equivocacion,entropiaEntrada,informacionMutua,entropia;
  //mostrarMatriz(cantEntrada,cantSalida,mat);
  char anexo[5],nombreArch[20];
  strcpy(nombreArch,"parte2salidaCanal");
  anexo[0] = canal + 48;
  anexo[1] = '\0';
  strcat(nombreArch,anexo);
  strcpy(anexo,".txt");
  strcat(nombreArch,anexo);

  FILE *arch = fopen(nombreArch,"wt");
  calcularProbSalida(cantEntrada,cantSalida,probEntrada,mat,probSalida); //calcula probabilidades de salida

  calcularCondicionales(cantEntrada,cantSalida,probEntrada,mat,probCondEntrada,probSalida); //calcular probabilidades condicionales

  calcularEntropiaPosteriori(cantEntrada,cantSalida,probCondEntrada,entropiaPosteriori); //calcula las entropias a posteriori

  k = equivocacion = 0;
  while(k<cantSalida){
    equivocacion += entropiaPosteriori[k] * probSalida[k];
    k++;
  }

  k = entropiaEntrada = 0;
  while(k<cantEntrada){
    entropiaEntrada += probEntrada[k] * (log(1/probEntrada[k]) / log(2));
    k++;
  }

  informacionMutua = entropiaEntrada - equivocacion;

  fprintf(arch,"entropia entrada: %f\n",calcularEntropia(probEntrada,cantEntrada));
  fprintf(arch,"prob salida: \n");
  char palabra[20],agregado[3];
  agregado[2] = '\0';
  for(int h=1;h<=cantSalida;h++){
    strcpy(palabra,"PROB(B");
    agregado[0] = h + 48;
    agregado[1] = ')';
    strcat(palabra,agregado);
    fprintf(arch,"%s ",palabra);
  }
  fprintf(arch,"\n");
  //mostrarVector(probSalida,cantSalida);
  //
  for(int i=0;i<cantSalida;i++)
    fprintf(arch,"%f ",probSalida[i]);
  //
  fprintf(arch,"\n");fprintf(arch,"entropia salida: %f\n",calcularEntropia(probSalida,cantSalida));
  fprintf(arch,"\n");fprintf(arch,"matriz cond: \n");
  //mostrarMatriz(cantSalida,cantEntrada,probCondEntrada);
  //
  char pal[5];
  pal[0] = 'S';
  pal[2] = '\0';
  fprintf(arch,"    ");
  for(int h=0;h<cantSalida;h++){
    pal[1] = h + 48;
    fprintf(arch,"%s        ",pal);
  }
  fprintf(arch,"\n");
  pal[0] = 'B';
  for(int n=0;n<cantEntrada;n++){
    pal[1] = n + 48;
    fprintf(arch,"%s ",pal);
    for(int m=0;m<cantSalida;m++){
      fprintf(arch,"%f ",mat[n][m]);
    }
    fprintf(arch,"\n");
  }
  //
  fprintf(arch,"entropia post: \n");
  for(int h=1;h<=cantSalida;h++){
    strcpy(palabra,"H(A/B");
    agregado[0] = h + 48;
    agregado[1] = ')';
    agregado[2] = '\0';
    strcat(palabra,agregado);
    fprintf(arch,"%s  ",palabra);
  }
  fprintf(arch,"\n");
  //mostrarVector(entropiaPosteriori,cantSalida);
  //
  for(int i=0;i<cantSalida;i++)
    fprintf(arch,"%f ",entropiaPosteriori[i]);
  //
  fprintf(arch,"\nequivocacion: \n%f",equivocacion); //UNIDADES!!!
  fprintf(arch,"\ninformacion mutua: \n%f",informacionMutua);
  fclose(arch);
}

double calcularEntropia(double vector[],int n){
  double acum = 0;
  for(int i=0;i<n;i++){
    acum += vector[i]*(log(1/vector[i])/log(2));
  }
  return acum;
}

void calcularProbSalida(int cantEntrada,int cantSalida,double probEntrada[],double mat[][cantSalida],double probSalida[cantSalida]){
  double acum;
  int i,j;
  j = 0;
  while(j<cantSalida){
    i = acum = 0;
    while(i<cantEntrada){
      acum += mat[i][j] * probEntrada[i];
      i++;
    }
    probSalida[j] = acum;
    j++;
  }
  //mostrarVector(probSalida,cantSalida);
}

void calcularCondicionales(int cantEntrada,int cantSalida,double probEntrada[],
                           double mat[][cantSalida],double probCondEntrada[][cantEntrada],double probSalida[cantSalida]){
  int i,j;
  j = 0;
  while(j<cantSalida){
    i = 0;
    while(i<cantEntrada){
      if(probSalida[j]!=0)
        probCondEntrada[j][i] = (mat[i][j] * probEntrada[i]) / probSalida[j];
      else
        probCondEntrada[j][i] = 0;
      i++;
    }
    j++;
  }
  //printf("\n");printf("\n");
  //mostrarMatriz(cantSalida,cantEntrada,probCondEntrada);
}

void calcularEntropiaPosteriori(int cantEntrada,int cantSalida,
                                double probCondEntrada[][cantEntrada],double entropiaPost[cantSalida]){
  int i,j;
  inicializarVector(entropiaPost,cantSalida);
  i = 0;
  while(i<cantSalida){
    j = 0;
    while(j<cantEntrada){
      if(probCondEntrada[i][j]!=0)
        entropiaPost[i] += probCondEntrada[i][j] * (log(1/probCondEntrada[i][j]) / log(2));
      j++;
    }
    i++;
  }
}

void mostrarVector(double vec[],int n){
  for(int i=0;i<n;i++)
    printf("%f ",vec[i]);
}

void mostrarMatriz(int i,int j,double mat[][j]){
  char palabra[5];
  palabra[0] = 'S';
  palabra[2] = '\0';
  printf("    ");
  for(int h=0;h<j;h++){
    palabra[1] = h + 48;
    printf("%s        ",palabra);
  }
  printf("\n");
  palabra[0] = 'B';
  for(int n=0;n<i;n++){
    palabra[1] = n + 48;
    printf("%s ",palabra);
    for(int m=0;m<j;m++){
      printf("%f ",mat[n][m]);
    }
    printf("\n");
  }
}

void inicializarVector(double vec[],int n){
  for(int i=0;i<n;i++)
    vec[i] = 0;
}

