#include <stdio.h>
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
void invertir_vector(int v[], int n);

void test_ins();
void test_ins2();
void test_shell();
void test_shell2();

double microsegundos();
static double medir_tiempo_insercion(const int origen[], int n);
static double medir_tiempo_shell(const int origen[], int n, int inc[], int m);

static void encabezado(const char *titulo);
static void fila(int n, double t, double a1,double a2,double a3);
static void pie_tabla(int K);
int build_hibbard(int inc[], int n);
int build_knuth(int inc[], int n);
int build_sedgewick(int inc[], int n);
int build_ciura(int inc[], int n);


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
    static const int Ns[] = {500,1000,2000,4000,8000,16000,32000};
    static const int NUM_N = sizeof(Ns)/sizeof(Ns[0]);

    encabezado("ORDENACION POR INSERCION (vector desordenado)");
     for (k=0;k<NUM_N;k++){
        n = Ns[k];
        v = (int*)malloc(n*sizeof(int));
        aleatorio(v, n);
        t = medir_tiempo_insercion(v, n);
        fila(n, t, 1.8, 2.0, 2.2);
        free(v);
    }
    pie_tabla(1000);

    encabezado("ORDENACION POR INSERCION (vector acendente)");
     for (int k=0;k<NUM_N;k++){
        int n = Ns[k];
        int *v = (int*)malloc(n*sizeof(int));
        ArrAsc(v, n);
        double t = medir_tiempo_insercion(v, n);
        fila(n, t, 1, 1.1, 1.2);
        free(v);
    }
    pie_tabla(1000);
    
    encabezado("ORDENACION POR INSERCION (vector descendente)");
     for (int k=0;k<NUM_N;k++){
        int n = Ns[k];
        int *v = (int*)malloc(n*sizeof(int));
        ArrayDesc(v, n);
        double t = medir_tiempo_insercion(v, n);
        fila(n, t, 1.8, 2.0, 2.2);
        free(v);
    }
    pie_tabla(1000);
}


//TODO REVISAR SI ESTÁ BIEN + CHECKEAR Y MARCAR ANOMALÍAS 
void test_shell2() {
    int inc[64];
    int s, k, n, m, *v; 
    double t;
    static const int Ns[] = {500,1000,2000,4000,8000,16000,32000};
    static const int NUM_N = sizeof(Ns)/sizeof(Ns[0]);

    struct {
        char *nombre;
        int (*build)(int[], int);
    } series[] = {
        {"ORDENACION POR SHELL (Hibbard)",  build_hibbard},
        {"ORDENACION POR SHELL (Knuth)",    build_knuth},
        {"ORDENACION POR SHELL (Sedgewick)",build_sedgewick},
        {"ORDENACION POR SHELL (Ciura)",   build_ciura}
    };

    for (s = 0; s < 4; s++) {
        encabezado(series[s].nombre);
        for (k = 0; k < NUM_N; k++) {
            n = Ns[k];
            v = (int*)malloc(n * sizeof(int));
            aleatorio(v, n);
            m=series[s].build(inc,n);
            t = medir_tiempo_shell(v, n,inc ,m );
            if(s==0 || s==1)
                fila(n, t, 1.3, 1.5, 1.7);
            else if (s==2)
                fila(n, t, 1.2, 1.33, 1.5);
            else
            fila(n, t, 1.3, 1.5, 1.7);
            free(v);
        }
        pie_tabla(1000);
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
    else(printf("   "));
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
    else(printf("   "));
    
    free(w);
    return t;
}

/* ========Utilidades tabla ======== */
static void encabezado(const char *titulo){
    printf("\n%s:\n\n", titulo);
    if(strcmp(titulo,"ORDENACION POR INSERCION (vector acendente)")==0)
    {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.0","t(n)/n^1.1","t(n)/n^1.2");
    }
    if (strcmp(titulo,"ORDENACION POR INSERCION (vector desordenado)")==0 ||
        strcmp(titulo,"ORDENACION POR INSERCION (vector descendente)")==0)
        {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
    }
    if (strcmp(titulo, "ORDENACION POR SHELL (Hibbard)")==0 ||
        strcmp(titulo, "ORDENACION POR SHELL (Knuth)")==0){
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.3","t(n)/n^1.5","t(n)/n^1.7");
    }
    if (strcmp(titulo, "ORDENACION POR SHELL (Sedgewick)")==0){
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.2","t(n)/n^1.33","t(n)/n^1.5");
    }
    if (strcmp(titulo, "ORDENACION POR SHELL (Ciura)")==0){
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.1","t(n)/n^1.2","t(n)/n^1.3");
    } 
}
static void fila(int n, double t, double a1,double a2,double a3){
    printf("%10d %16.8f %16.8f %16.8f %16.8f\n",
           n, t, t/pow(n,a1), t/pow(n,a2), t/pow(n,a3));
}
static void pie_tabla(int K) {
    printf("(*) Tiempo promedio de K=%d ejecuciones (corregido restando la preparación).\n", K);
}



/*===============Utilidades generales=================*/
int build_hibbard(int inc[], int n) {
    int m = 0, h = 1;
    while (h < n) {
        inc[m++] = h;
        h = 2 * h + 1;           // 1,3,7,15,31,...
    }
    invertir_vector(inc, m);     // ← usar la función común
    return m;
}

int build_knuth(int inc[], int n) {
    int m = 0, h = 1;
    while (h < n) {
        inc[m++] = h;
        h = 3 * h + 1;           // 1,4,13,40,...
    }
    invertir_vector(inc, m);
    return m;
}

int build_sedgewick(int inc[], int n) {
    int m = 0, k = 1;
    long h;
    if (n > 1) inc[m++] = 1;
    do {
        if (k % 2 == 0)
            h = (9*pow(4, k))-(9*pow(2,k)) + 1;
        else
            h=pow(4,k)-3*pow(2,k) + 1;

        if (h > 0 && h<n) 
            inc[m++] = (int)h;
        k++;
    } while (h < n);
    invertir_vector(inc, m);
    return m;
}

int build_ciura(int inc[], int n) {
    int base[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    int m = 0, i = 0;

    while (i < (int)(sizeof(base)/sizeof(base[0])) && base[i] < n)
        inc[m++] = base[i++];

    int last = inc[m - 1];
    while ((int)(last * 2.25) < n) {
        last = (int)round(last * 2.25);
        inc[m++] = last;
    }
    invertir_vector(inc, m);
    return m;
}
void invertir_vector(int v[], int n) {
    for (int i = 0; i < n / 2; i++) {
        int tmp = v[i];
        v[i] = v[n - 1 - i];
        v[n - 1 - i] = tmp;
    }
}
