#define MAXPALABRA 50

typedef struct nodoDicc{
  char palabra[MAXPALABRA];
  char codificacion[MAXPALABRA];
  struct nodoDicc* sig;
}nodoDicc;

typedef nodoDicc * tDiccionario;

typedef struct palFrec{
  char palabra[MAXPALABRA];
  int frecuencia,largo;
  double probabilidad;
}palFrec;

//codificar
typedef struct nodo{
   int code,k;
   struct nodo * sig;
}nodo;

typedef nodo * tLista;

//shannon
typedef struct nodoShannon{
  char palabra[MAXPALABRA];
  double probabilidad;
  int largo;
  struct nodo* codificacion;
}nodoShannon;

//operar
typedef struct nodoClave{
  char palabra[MAXPALABRA];
  double prob;
  int largo;
  struct nodoClave *izq,*der;
  tLista codificacion;
}nodoClave;

//limpio
typedef struct nodoCodificacion{
  char palabra[MAXPALABRA];
  tLista codificacion;
  double probabilidad;
  int largo;
  struct nodoCodificacion* sig;
}nodoCodificacion;

typedef nodoCodificacion* tListaCodificacion;

typedef nodoClave * tListaHuffman;
//procesar
void procesar();
int estaPalabra(palFrec[],int,char[]);
//huffman
tListaCodificacion huff(palFrec[],int,double*);
void huffman(tListaHuffman[],int);
void quicksortHuffman(tListaHuffman[],int,int);
void agregarBitHuffman(tLista*,int);
tLista copiarCode(tLista);
void codificar(tListaHuffman,tLista);
void limpiarVector(tListaHuffman,tListaCodificacion*);
void recorrerLista(tListaCodificacion);

void shannon(palFrec[],int,double*);
void shannonFano(nodoShannon[],int,int);
void quicksortShannon(nodoShannon[],int,int);

double calcularLargo(tLista,double);
void codificarArchivoHuffman(tListaCodificacion,int);
tLista codePalabra(tListaCodificacion,char[MAXPALABRA]);
tLista encontrarCode(nodoShannon[],char[MAXPALABRA]);
int codePostMascara(int,int);
int estaCaracter(char caracteres[],int,char);

int codePostMascaraInverso(int,int);

void calcularEquivocacion(int,int cantEntrada,int cantSalida,double probEntrada[],double mat[][cantSalida]);
void mostrarVector(double[],int);
void mostrarMatriz(int,int j,double mat[][j]);
void inicializarVector(double[],int);
double calcularEntropia(double[],int);
void calcularEntropiaPosteriori(int cantEntrada,int,double[][cantEntrada],double[]);

void bitsToString(int,int,char[MAXPALABRA]);
void bitsToStringInversa(int,int,char[MAXPALABRA]);
void quePalabra(int*,tDiccionario,char[MAXPALABRA],int*);
void rellenarTitular(int *,int,int *);
void rellenarSuplente(short,int *,int *);
int codePostMascaraMejorado(int,int);
int codePostMascaraInversoMejorado(int,int);

void descomprimir(char[MAXPALABRA]);
