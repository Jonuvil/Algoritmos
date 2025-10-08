#include <stdio.h>
#include <sys/time.h>
#include <sys/time.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 

void OrdInsercion(int v[], int n);
void OrdShell(int v[], int inc[],int n, int m);
void inicializar_semilla();
void aleatorio();
void ArrAsc(int arr[], int n);
void ArrayDesc(int arr[], int n);
void printVec(int v[], int n);
double microsegundos();
static double medir_tiempo_insercion(const int origen[], int n);
static void encabezado(const char *titulo, double a1,double a2,double a3);
static void fila(int n, double t, double a1,double a2,double a3);
static const int Ns[] = {500,1000,2000,4000,8000,16000,32000};
static const int NUM_N = sizeof(Ns)/sizeof(Ns[0]);
/*Hay que hacer 3 tests en el de ordenación por inserción(3 tablas)
	1-Vectores desordenados
	2-Vectores ya ordenados
	3-Vectoras ordenados al revés(Peor caso)

Hay que hacer 4 casos distintos en el de shell(4 tablas)
	1-Secuencia de Hibbard
	2-Secuencia de Knuth
	3-Secuencia de Sedgewick
	4-Secuencia de Ciura
Total 7 tablas en el informe*/

int main(void) {
       inicializar_semilla();
       test_ins();
       test1();
    
}

//TODO Test ordenacion por inserción (vectores desordenados, ordenados, y ordenados al revés)
void test_ins() {
    int v[9];
    aleatorio(v, 9);

    int vord[9];
    ArrayDesc(vord, 9);

    printf("Vector aleatorio sin ordenar\n");
    printVec(v, 9);

    ord_ins(v, 9);
    
    printf("Vector aleatorio ordenado por inserción\n");
    printVec(v, 9);

    
    printf("Vector al revés\n");
    printVec(vord, 9);

    ord_ins(vord, 9);
    
    printf("Vector al revés ordenado por inserción\n");
    printVec(vord, 9);

} 
void test1(){
    encabezado("ORDENACION POR INSERCION (vector desordenado)", 0.8,1.0,1.2);
     for (int k=0;k<NUM_N;k++){
        int n = Ns[k];
        int *v = (int*)malloc(n*sizeof(int));
        aleatorio(v, n);
        double t = medir_tiempo_insercion(v, n);
        
        fila(n, t, 0.8, 1.0, 1.2);
        free(v);
    }
}

void printVec(int v[], int n) {
    int i;

    for (i = 0; i < n; i++) {
        printf("%i, ", v[i]);
    }
    printf("\n");
}

void ArrAsc(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

void ArrayDesc(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}
static bool ordenado(const int v[], int n){
    for (int i=1;i<n;i++) if (v[i-1] > v[i]) return false;
    return true;
}

void ord_ins(int v[], int n){
    for (int i=1;i<n;i++){
        int x=v[i];
        int j=i-1;
        while(j>=0 && v[j]>x){
            v[j+1] = v[j] ;
            j--;
        }
        v[j+1]=x;
    }
}

void ord_shell(int v[], int n, int inc[], int m) {
    int k, h, i, x, j;
    
    for(k = 0; k < m-1; k++) {
        h = inc[k];

        for (i = h; i < n-1; i++) {
            x = v[i];
            j = i;
            while (j > h && v[j-h] > x) {
                v[j] = v[j-h];
                j -= h;
            }
            v[j] = x;
        }
    }
}

void inicializar_semilla() { 
    srand(time(NULL));
}

void aleatorio(int v [], int n) { 
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 ) {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


static double medir_tiempo_insercion(const int origen[], int n){
    //COpiamos el vector original para no modificarlo
    int *w = (int*)malloc(n*sizeof(int));
    memcpy(w, origen, n*sizeof(int));
    //Inicializamos el tiempo y ejecutamos el algoritmo y medimos el tiempo utilizado
    double t1 = microsegundos();
    ord_ins(w, n);
    double t2 = microsegundos();
    double t = t2 - t1;

    //SI el tiempo es muy pequeño, lo medimos 1000 veces y hacemos la media
    if (t < 500.0){
        int K = 1000;
    
        t1 = microsegundos();
        for (int i=0;i<K;i++){
            memcpy(w, origen, n*sizeof(int));
            ord_ins(w, n);
        }
        t2 = microsegundos();

        //Acabamos de medir el tiempo base del algorimto,
        //ahora hay que restarle el tiempo de copiar el vector k veces 
        double tb1 = microsegundos();
        for (int i=0;i<K;i++){
            memcpy(w, origen, n*sizeof(int));
        }
        double tb2 = microsegundos();
        t = ((t2 - t1) - (tb2 - tb1)) / K;
        
        printf("(*)");
    }
    
    free(w);
    return t;
}

/* ======== helpers de tabla ======== */
static void encabezado(const char *titulo, double a1,double a2,double a3){
    printf("\n%s\n", titulo);
    printf("%10s %12s %15s %15s %15s\n",
           "n","t(n) (us)","t(n)/n^a1","t(n)/n^a2","t(n)/n^a3");
}
static void fila(int n, double t, double a1,double a2,double a3){
    printf("%10d %12.8f %15.8f %15.8f %15.8f\n",
           n, t, t/pow(n,a1), t/pow(n,a2), t/pow(n,a3));
}
