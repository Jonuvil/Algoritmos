#include <stdio.h>
#include <sys/time.h>
#include <sys/time.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 
#include <time.h>

void ord_ins(int v[], int n);
void ord_shell(int v[], int n, int inc[], int m);
void inicializar_semilla();
void aleatorio(int v [], int n);
void ArrAsc(int arr[], int n);
void ArrayDesc(int arr[], int n);
void printVec(int v[], int n);
void test_ins();
void test_ins2();
void test_shell();
void test_shell2();
double microsegundos();
static double medir_tiempo_insercion(const int origen[], int n);
static double medir_tiempo_shell(const int origen[], int n, int inc[], int m);
static void encabezado(const char *titulo);
static void fila(int n, double t, double a1,double a2,double a3);
static const int Ns[] = {500,1000,2000,4000,8000,16000,32000};
static const int NUM_N = sizeof(Ns)/sizeof(Ns[0]);

int main(void) {
       inicializar_semilla();
       test_ins();
       test_shell();
       test_ins2();
       test_shell2();   
}

void test_ins() {
    int v[9];
    int vord[9];
    aleatorio(v, 9);
    ArrayDesc(vord, 9);

    printf("\n=== Test de ordenación por inserción ===\n");

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

void test_shell() {
    int v[15], vord[15], tmp[15], n = 15;
    int hibbard[] = {1, 3, 7};
    int knuth[] = {1, 4, 13};
    int sedgewick[] = {1, 5, 19, 41, 109};
    int ciura[] = {1, 4, 10, 23, 57, 132, 301, 701};
    int *incs[] = {hibbard, knuth, sedgewick, ciura};
    int lens[] = {3, 3, 5, 8};
    char *noms[] = {"Hibbard", "Knuth", "Sedgewick", "Ciura"};
    int t, s; int* src;

    printf("\n=== Test de ordenación Shell ===\n");

    aleatorio(v, n);
    ArrayDesc(vord, n);

    for (t = 0; t < 2; t++) {
        src = (t == 0) ? v : vord;
        printf(t == 0 ? "\nVector aleatorio:\n" : "\nVector descendente:\n");
        printVec(src, n);

        for (s = 0; s < 4; s++) {
            memcpy(tmp, src, sizeof(int) * n);
            ord_shell(tmp, n, incs[s], lens[s]);
            printf("\nOrdenado con Shell (%s):\n", noms[s]);
            printVec(tmp, n);
        }
    }
}

//TODO REVISAR SI ESTÁ BIEN + CHECKEAR Y MARCAR ANOMALÍAS 
void test_ins2() {
    int k, n, *v;
    double t;

    encabezado("ORDENACION POR INSERCION (vector desordenado)");
     for (k=0;k<NUM_N;k++){
        n = Ns[k];
        v = (int*)malloc(n*sizeof(int));
        aleatorio(v, n);
        t = medir_tiempo_insercion(v, n);
        fila(n, t, 1.8, 2.0, 2.2);
        free(v);
    }

    encabezado("ORDENACION POR INSERCION (vector ordenado)");
     for (int k=0;k<NUM_N;k++){
        int n = Ns[k];
        int *v = (int*)malloc(n*sizeof(int));
        ArrAsc(v, n);
        double t = medir_tiempo_insercion(v, n);
        fila(n, t, 1.8, 2.0, 2.2);
        free(v);
    }

    encabezado("ORDENACION POR INSERCION (vector ordenado descendiente)");
     for (int k=0;k<NUM_N;k++){
        int n = Ns[k];
        int *v = (int*)malloc(n*sizeof(int));
        ArrayDesc(v, n);
        double t = medir_tiempo_insercion(v, n);
        fila(n, t, 1.8, 2.0, 2.2);
        free(v);
    }
}

//TODO REVISAR SI ESTÁ BIEN + CHECKEAR Y MARCAR ANOMALÍAS 
void test_shell2() {
    int hibbard[]  = {1, 3, 7};
    int knuth[]    = {1, 4, 13};
    int sedgewick[] = {1, 5, 19, 41, 109};
    int ciura[]    = {1, 4, 10, 23, 57, 132, 301, 701};
    int s, k, n, *v; 
    double t;

    struct {
        char *nombre;
        int *seq;
        int len;
    } series[] = {
        {"ORDENACION POR SHELL (Hibbard)",  hibbard,  3},
        {"ORDENACION POR SHELL (Knuth)",    knuth,    3},
        {"ORDENACION POR SHELL (Sedgewick)",sedgewick,5},
        {"ORDENACION POR SHELL (Ciura)",    ciura,    8}
    };

    for (s = 0; s < 4; s++) {
        encabezado(series[s].nombre);
        for (k = 0; k < NUM_N; k++) {
            n = Ns[k];
            v = (int*)malloc(n * sizeof(int));
            aleatorio(v, n);
            t = medir_tiempo_shell(v, n, series[s].seq, series[s].len);
            fila(n, t, 1.8, 2.0, 2.2);
            free(v);
        }
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
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

void ArrayDesc(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void ord_ins(int v[], int n){
    int i, x, j;
    for (i=1;i<n;i++){
        x=v[i];
        j=i-1;
        while(j>=0 && v[j]>x){
            v[j+1] = v[j] ;
            j--;
        }
        v[j+1]=x;
    }
}

void ord_shell(int v[], int n, int inc[], int m) {
    int k, h, i, j, x;

    for (k = 0; k < m; k++) {
        h = inc[k];               

        for (i = h; i < n; i++) {  
            x = v[i];
            j = i;
            while (j >= h && v[j - h] > x) {
                v[j] = v[j - h];
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


static double medir_tiempo_insercion(const int origen[], int n) {
    //COpiamos el vector original para no modificarlo
    int *w = (int*)malloc(n*sizeof(int));
    memcpy(w, origen, n*sizeof(int));
    //Inicializamos el tiempo y ejecutamos el algoritmo y 
    //medimos el tiempo utilizado
    double t1 = microsegundos();
    ord_ins(w, n);
    double t2 = microsegundos();
    double t = t2 - t1;
    int K = 1000, i;
    double tb1, tb2;

    //SI el tiempo es muy pequeño, lo medimos 1000 veces y hacemos la media
    if (t < 500.0){
        t1 = microsegundos();
        for (i=0;i<K;i++){
            memcpy(w, origen, n*sizeof(int));
            ord_ins(w, n);
        }
        t2 = microsegundos();

        //Acabamos de medir el tiempo base del algorimto,
        //ahora hay que restarle el tiempo de copiar el vector k veces 
        tb1 = microsegundos();
        for (i=0;i<K;i++){
            memcpy(w, origen, n*sizeof(int));
        }
        tb2 = microsegundos();
        t = ((t2 - t1) - (tb2 - tb1)) / K;
        
        printf("(*)");
    }
    free(w);
    return t;
}

static double medir_tiempo_shell(const int origen[], int n, int inc[], int m) {
    //COpiamos el vector original para no modificarlo
    int *w = (int*)malloc(n*sizeof(int));
    memcpy(w, origen, n*sizeof(int));
    //Inicializamos el tiempo y ejecutamos el algoritmo y medimos el tiempo 
    //utilizado
    double t1 = microsegundos();
    ord_shell(w, n, inc, m);
    double t2 = microsegundos();
    double t = t2 - t1;
    int K = 1000, i;
    double tb1, tb2;

    //SI el tiempo es muy pequeño, lo medimos 1000 veces y hacemos la media
    if (t < 500.0){
        t1 = microsegundos();
        for (i=0;i<K;i++){
            memcpy(w, origen, n*sizeof(int));
            ord_shell(w, n, inc, m);
        }
        t2 = microsegundos();

        //Acabamos de medir el tiempo base del algorimto,
        //ahora hay que restarle el tiempo de copiar el vector k veces 
        tb1 = microsegundos();
        for (i=0;i<K;i++){
            memcpy(w, origen, n*sizeof(int));
        }
        tb2 = microsegundos();
        t = ((t2 - t1) - (tb2 - tb1)) / K;
        
        printf("(*)");
    }
    
    free(w);
    return t;
}

/* ======== helpers de tabla ======== */
static void encabezado(const char *titulo){
    printf("\n%s\n", titulo);
    printf("%10s %12s %15s %15s %15s\n",
           "n","t(n) (us)","t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
}
static void fila(int n, double t, double a1,double a2,double a3){
    printf("%10d %12.8f %15.8f %15.8f %15.8f\n",
           n, t, t/pow(n,a1), t/pow(n,a2), t/pow(n,a3));
}
