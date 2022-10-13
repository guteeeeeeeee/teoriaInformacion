#define CANTALFABETO 3
#define INFINITO 999
#define MAX 1000
#define MAXMAT 10
typedef char codif[300];

void procesarArchivo();
void inicializarVecDouble(double[],int);
double entropiaFuente(double[][CANTALFABETO],int);
void inicializarMat(double[][CANTALFABETO],int);
int esMemoriaNula(double[],double[][CANTALFABETO],int);
double longitudMedia(int[],double[],int);
double calcularRendimiento(double,double);
void shannon(double[],int);
void shannonFano(double[],int,int,codif[]);
void decimalToTrinario(int,int,int[]);
int cumpleKraft(int[],int);
int esCompacto(int[],double[],int);
void procesarPalabras(int);
void decimalToBinario(long int);
int busquedaBinariaRecursiva(int[], int,int,int);
void quicksort(double[],int,int,int[]);
void inicializarVecInt(int[],int);
void inicializarCodificaciones(codif[],int);
int esErgodica(double[][CANTALFABETO],int);
