#include <stdio.h>
#include <stdlib.h>
#include "teoria.h"
#include <math.h>
#include <string.h>

void procesarArchivo(){
  double prob[CANTALFABETO],mat[CANTALFABETO][CANTALFABETO];
  FILE *arch,*ejercicio1a,*ejercicio1c;
  arch = fopen("code.txt","rt");//
  ejercicio1a = fopen("PrimeraParteEjercicio1A.txt","wt");
  ejercicio1c = fopen("PrimeraParteEjercicio1C.txt","wt");
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
  /*printf("frecuencia de cada simbolo de la fuente: \n");
  for(i=0;i<CANTALFABETO;i++)
    printf("%c %f ",'A'+i,prob[i]);
  printf("\nfrecuencia de cada i si salio el simbolo j:\n");
  for(i=0;i<CANTALFABETO;i++){
    for(j=0;j<CANTALFABETO;j++)
      printf("%c%c %f | ",'A'+i,'A'+j,mat[i][j]);
    printf("\n");
  }*/
  //operar
  for(i=0;i<CANTALFABETO;i++)
    for(j=0;j<CANTALFABETO;j++)
      mat[i][j] = (double) mat[i][j] / prob[j];//(cont - 1); //matriz con probabilidades condicionales MAT[A][B] = P(A/B)
  for(i=0;i<CANTALFABETO;i++)
    prob[i] = (double) prob[i] / cont; //probabilidad de cada simbolo de manera independiente
  //mostrar
  fprintf(ejercicio1a,"probabilidad de cada simbolo de manera independiente:\n");
  for(i=0;i<CANTALFABETO;i++)
    fprintf(ejercicio1a,"%c %f ",'A'+i,prob[i]);
  fprintf(ejercicio1a,"\nprobabilidades condicionales:\n");
  for(i=0;i<CANTALFABETO;i++){
    for(j=0;j<CANTALFABETO;j++)
      fprintf(ejercicio1a,"%c%c %f | ",'A'+i,'A'+j,mat[i][j]);
    fprintf(ejercicio1a,"\n");
  }
  //memoria nula
  if(!esMemoriaNula(prob,mat,CANTALFABETO)){
    fprintf(ejercicio1a,"La fuente no es de memoria nula\n");
    //printf("La fuente no es de memoria nula\n");
    if(esErgodica(mat,CANTALFABETO)){
      fprintf(ejercicio1c,"La fuente es ergodica\n");
      //printf("La fuente es ergodica\n");
      fprintf(ejercicio1c,"Entropia de la fuente: %f unidades trinarias/simbolo\n",entropiaFuente(mat,CANTALFABETO));
      //printf("Entropia de la fuente: %f unidades trinarias/simbolo\n",entropiaFuente(mat,CANTALFABETO));
    }
  }
  fclose(ejercicio1a);
  fclose(ejercicio1c);
  fclose(arch);
}

//mat = matriz con probabilidades condicionales MAT[A][B] = P(A/B)
//prob = vector con la probabilidad de cada simbolo de manera independiente
//n cant de caracteres alfabeto original
int esMemoriaNula(double prob[],double mat[][CANTALFABETO],int n){
  int i,j;
  i = j = 0;
  while(j<n && fabs(mat[i][j] - prob[i]) <= 0.001){
    //si son estadisticamente independientes => P(A/B) = P(A)*P(B) (se le da cierto margen al ser iteraciones finitas)
    i++;
    if(i>=n){
      i = 0;
      j++;
    }
  }
  return !(j<n);
}

int esErgodica(double mat[][CANTALFABETO],int n){
  int i,j,k,aux[CANTALFABETO][CANTALFABETO];
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      if(mat[i][j]==0)
        aux[i][j] = 99; //no hay camino
      else
        aux[i][j] = 1; //si hay camino
    }
  }
  for(k=0;k<n;k++){
    for(i=0;i<n;i++){
      for(j=0;j<n;j++){
        if(aux[i][k] + aux[k][j] < aux[i][j])
          aux[i][j] = 1;
      }
    }
  }
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

double entropiaFuente(double mat[][CANTALFABETO],int n){
  double acumTotal,acumParcial,vecEstacionario[CANTALFABETO];
  vecEstacionario[0] = 0.5160244;
  vecEstacionario[1] = 0.2869584;
  vecEstacionario[2] = 0.1773729;
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
      acumParcial += mat[i][j]*(log(1/mat[i][j]) / log(CANTALFABETO));
    }
    acumTotal += acumParcial * vecEstacionario[j];
  }
  return acumTotal;
}

//SEGUNDA PARTE
void procesarPalabras(int orden){
  double prob[(int)pow(CANTALFABETO,orden)];
  int i,ind,cont,n,ternario[200];
  n = (int)pow(CANTALFABETO,orden);
  int largo[n];
  char pal[orden + 1];
  double cantInfo,entropia;
  FILE *ejercicioA,*ejercicioB,*ejercicioC,*ejercicioD;
  char nombre[MAX],aux[2];
  strcpy(nombre,"SegundaParteEjercicioAlongitud");
  aux[0] = orden + 48;
  aux[1] = '\0';
  strcat(nombre,aux);
  strcat(nombre,".txt");
  ejercicioA = fopen(nombre,"wt");

  strcpy(nombre,"SegundaParteEjercicioBlongitud");
  aux[0] = orden + 48;
  aux[1] = '\0';
  strcat(nombre,aux);
  strcat(nombre,".txt");
  ejercicioB = fopen(nombre,"wt");

  strcpy(nombre,"SegundaParteEjercicioClongitud");
  aux[0] = orden + 48;
  aux[1] = '\0';
  strcat(nombre,aux);
  strcat(nombre,".txt");
  ejercicioC = fopen(nombre,"wt");

  strcpy(nombre,"SegundaParteEjercicioDlongitud");
  aux[0] = orden + 48;
  aux[1] = '\0';
  strcat(nombre,aux);
  strcat(nombre,".txt");
  ejercicioD = fopen(nombre,"wt");

  cont = cantInfo = entropia = 0;
  inicializarVecInt(largo,n);
  inicializarVecDouble(prob,n);
  FILE *arch = fopen("code.txt","rt");

  //printf("\npalabras codigo longitud %d\n",orden);
  while(!feof(arch)){
    i = 0;
    strcpy(pal,"");
    while(!feof(arch) && i<orden)//lee las palabras
      fscanf(arch,"%c",&pal[i++]);
    pal[i] = '\0';
    if(i==orden){ //si son del largo deseado
      cont++; //cant de palabras
      ind = 0; //indice vector prob
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
      cantInfo += log(1/prob[i]) / log(CANTALFABETO); //log2(1/prob)
      entropia += prob[i] * (log(1/prob[i]) / log(CANTALFABETO)); //prob * log2(1/prob)
    }
  }/*
  for(int k=0;k<n;k++){
    decimalToTernario(k,orden,ternario);
    printf(" %f | ",prob[k]);
  }*/
  fprintf(ejercicioA,"cantidad informacion: %f unidades ternarias/simbolo\nentropia: %f unidades ternarias/simbolo\n",cantInfo,entropia);
  fprintf(ejercicioB,"Los codigos obtenidos son univocamente decodificables al ser todos distintos y tener la misma longitud ya que se sabe donde empieza y termina cada palabra codigo. Tambien son instantaneos al no haber posibilidad de que algun codigo sea prefijo de otro.");
  if(cumpleKraft(largo,n))
    fprintf(ejercicioC,"cumple Kraft y McMillan\n");
  else
    fprintf(ejercicioC,"no cumple Kraft y McMillan\n");
  fprintf(ejercicioC,"longitud media: %f\n",longitudMedia(largo,prob,n));
  if(esCompacto(largo,prob,n))
    fprintf(ejercicioC,"es compacto\n");
  else
    fprintf(ejercicioC,"no es compacto\n");
  //printf("compacto: %d\n",esCompacto(largo,prob,n));
  double rend = entropia / longitudMedia(largo,prob,n);
  fprintf(ejercicioD,"rendimiento: %f%c redundancia: %f%c\n",rend*100,'%',(1-rend)*100,'%');
  shannon(prob,orden);
  /*fclose(ejercicioA);
  fclose(ejercicioB);
  fclose(ejercicioC);
  fclose(ejercicioD);
  fclose(arch);*/
}

int cumpleKraft(int largo[],int k){ //calcula si es posible un codigo instantaneo con esas longitudes de codigo
  double acum = 0;
  for(int i=0;i<k;i++)
    acum += pow(CANTALFABETO,(-1)*largo[i]);
  return acum-1<=0.0001;
}

double longitudMedia(int largo[],double prob[],int k){ //k es cant de posibles palabras distintas
  double acum = 0;
  for(int i=0;i<k;i++)
    acum += prob[i] * largo[i];
  return acum;
}

int esCompacto(int largo[],double prob[],int k){ //log(1/prob[i]) / log(CANTALFABETO)
  int i = 0;
  while(i<k && largo[i] == (int)ceil(log(1/prob[i]) / log(CANTALFABETO)))//redondea para arriba
    i++;
  return !(i<k);
}

void shannon(double prob[],int orden){
  char nombreArch[20],ordenChar[10];
  int tri[MAX],n,h;
  n = (int)pow(3,orden);
  tLista aux;
  tLista vector[n];
  strcpy(nombreArch,"codificacionOrden");
  ordenChar[0] = orden+48;
  ordenChar[1] = '\0';
  strcat(nombreArch,ordenChar);
  strcpy(ordenChar,".txt");
  strcat(nombreArch,ordenChar);
  FILE *arch = fopen(nombreArch,"wt");

  int i,claves[n];
  //codif codificaciones[n];
  for(i=0;i<n;i++){
    claves[i] = i;
    vector[i] = NULL; //inicializo listas
  }
  //inicializarCodificaciones2(codificaciones,n);
  quicksort(prob,0,n-1,claves); //ordena las probabilidades con sus claves
  //for(i=0;i<n;i++)
    //printf("%d %f\n",claves[i],prob[i]);
  shannonFano(prob,0,n-1,vector);
  for(i=0;i<n;i++){
    decimalToTernario(claves[i],orden,tri);
    for(int k=0;k<orden;k++)
      fprintf(arch,"%c",tri[k]+65);
    fprintf(arch," ");
    aux = vector[i];
    while(aux!=NULL){
      h = aux->k;
      while(h>0){
        fprintf(arch,"%d",(aux->code >> (h-1))&0x0001);
        h--;
      }
      aux = aux->sig;
    }
    fprintf(arch,"\n");
    //fprintf(arch," %s\n",codificaciones[i]);
    //printf("%d %f ",claves[i],prob[i]);
    //printf("%s\n",codificaciones[i]);
  }
  fclose(arch);
}

void agregarBit(int i,int bit,tLista codi[]){
  tLista aux,nuevo;
  aux = codi[i];
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
    codi[i] = nuevo;
  }
}

void shannonFano(double prob[],int i,int j,tLista codificaciones[]){//i donde arranca j donde termina
  int x,h;
  double acum,suma;
  acum = suma = 0;
  if(i!=j){
    if(i+1 == j){
      agregarBit(i,0,codificaciones);
      //pal[0] = '0';
      //strcat(codificaciones[i],pal);
      //pal[0] = '1';
      //strcat(codificaciones[j],pal);
      agregarBit(j,1,codificaciones);
    }else{
      for(h=i;h<=j;h++)
        acum += prob[h];
      h = i;
      //pal[0] = '0'; //sqrt(pow(suma+prob[h]-(acum-suma-prob[h]),2))-sqrt(pow(suma-prob[h]-(acum-suma-prob[h]),2))
      while(h<j && fabs(suma+prob[h]-(acum-suma-prob[h]))-fabs(suma-prob[h]-(acum-suma-prob[h])) <= 0.000001){ //<=
        agregarBit(h,0,codificaciones);
        //strcat(codificaciones[h],pal);
        suma += prob[h++];
      }
      x = h - 1;
      //pal[0] = '1';
      while(h<=j){
        agregarBit(h,1,codificaciones);
        h++;
      }//strcat(codificaciones[h++],pal);
    shannonFano(prob,i,x,codificaciones);
    shannonFano(prob,x+1,j,codificaciones);
    }
  }
}
