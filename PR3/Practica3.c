#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

void ord_ins(int v[], int n);
void ord_rapida(int v[], int n, int umbral);
void ordenar_aux(int v[], int ini, int n, int umbral);
void inicializar_semilla();
void aleatorio(int v [], int n);
void ArrAsc(int v[],int n);
void ArrDesc(int v[],int n);
double microsegundos();
static void encabezado(char *titulo,int umbral);
void encabezadoUmb1(char *titulo);
void encabezadoUmb10(char *titulo);
void encabezadoUmb100(char *titulo);
static void fila(int n, double t, double a1,double a2,double a3);
static void pie_tabla(int K);
void printVec(int v[], int n);
void testBienOrdenado();
void testTiempos();
double medirTiempoOrd(int v[], int n,int umbral);
void testVec(int v_test[], int n, int umbral_val);
void Mediana3 (int v[], int ini, int n);
void intercambio(int v[], int i, int j);
void controlFilas(int c,int umbral,int n,double t);

int main(void) {
    inicializar_semilla();
    testBienOrdenado();
    testTiempos();
}

void testBienOrdenado() {
    int n = 10, n2 = 12, umbral_val = 1;
    int v_test[n], v_test2[n2];

    aleatorio(v_test, n);
    ArrDesc(v_test2, n2);

    printf("=== 2: Validacion (umbral=%d) ===\n", umbral_val);
    testVec(v_test, n, umbral_val); // Vector desordenado
    testVec(v_test2, n2, umbral_val); // Vector ordenado descendente
    testVec(v_test2, n2, umbral_val); // Vector ordnado
    printf("\n");
}

void testTiempos() {
    int k, n, *v, c = 0, u, umbral;
    double t;
    static const int Ns[] = {500,1000,2000,4000,8000,16000,32000,64000,128000,256000};
    // static const int Ns[] = {500,1000,2000,4000,8000,16000,32000,64000,128000,256000,512000,1024000,2048000};
    static const int umbrales[] = {1,10,100};
    static const int NUM_N = sizeof(Ns)/sizeof(Ns[0]);
    int numU = sizeof(umbrales)/sizeof(umbrales[0]);
    struct {
        char *nombre;
        void (*orden)(int[], int);
    } casos[] = {
        {"ORDENACION QUICKSORT ALEATORIO",aleatorio},
        {"ORDENACION QUICKSORT ASCENDENTE",ArrAsc},
        {"ORDENACION QUICKSORT DESCENDENTE",ArrDesc},
    };
    printf("==== 3: medicion tiempos====\n");
    for (u = 0; u <numU; u++) {
        umbral=umbrales[u];
        printf("\n====== UMBRAL %d ======\n", umbral);
        for (c = 0; c < 3; c++) {
            encabezado(casos[c].nombre,umbral);
            for (k = 0; k < NUM_N; k++) {
                n = Ns[k];
                v = (int*)malloc(n * sizeof(int));
                casos[c].orden(v,n);
                t = medirTiempoOrd(v,n,umbral);
                controlFilas(c,umbral,n,t);
                free(v);
            }
            pie_tabla(1000);
        }
    }
}
    
double medirTiempoOrd(int v[], int n,int umbral) {
    double t1, t2, t, tb1, tb2;
    int K = 1000, i;
    int *w = (int*)malloc(n*sizeof(int));
    memcpy(w, v, n*sizeof(int));
    //Inicializamos el tiempo y ejecutamos el algoritmo y 
    //medimos el tiempo utilizado
    t1 = microsegundos();
    ord_rapida(w, n, umbral);
    t2 = microsegundos();
    t = t2 - t1;

    //SI el tiempo es muy pequeño, lo medimos k veces y hacemos la media
    if (t<500.0){
        t1 = microsegundos();
        for (i=0;i<K;i++){
            memcpy(w, v, n*sizeof(int));
            ord_rapida(w, n,umbral);
        }
        t2 = microsegundos();
        //Acabamos de medir el tiempo base del algorimto,
        //ahora hay que restarle el tiempo de copiar el vector k veces 
        tb1 = microsegundos();
        for (i=0;i<K;i++){
            memcpy(w, v, n*sizeof(int));
        }
        tb2 = microsegundos();
        t = ((t2 - t1) - (tb2 - tb1)) / K;    
        printf("(*)");
    }
    else(printf("   "));
    free(w);
    return t;
}

void testVec(int v_test[], int n, int umbral_val) {
    printf("Vector inicial (n=%d): ", n);
    printVec(v_test, n);
    ord_rapida(v_test, n, umbral_val);
    printf("Vector ordenado:\t");
    printVec(v_test, n);
    printf("\n");
}

void ord_ins(int v[], int n) {
    int i, x, j;
    for (i=1;i<n;i++) {
        x=v[i];
        j=i-1;
        while(j>=0 && v[j]>x) {
            v[j+1] = v[j] ;
            j--;
        }
        v[j+1]=x;
    }
}

void ord_rapida(int v[], int n, int umbral) {
    ordenar_aux(v, 0, n-1, umbral);
    if (umbral > 1) 
        ord_ins(v, n);
}       

void ordenar_aux(int v[], int ini, int n, int umbral) {
    if (ini + umbral <= n) {
        Mediana3(v, ini, n);
        
        int pivote = v[ini];
        int i = ini-1;
        int j = n+1;

        while (j > i) {
            do { i++; } while (v[i] < pivote);
            do { j--; } while (v[j] > pivote);

            intercambio(v, i, j);
        }
        intercambio(v, i, j);
        intercambio(v, ini, j);
        ordenar_aux(v, ini, j, umbral);
        ordenar_aux(v, j+1, n, umbral);
    }
}

void ArrAsc(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
}

void ArrDesc(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void intercambio(int v[], int i, int j) {
    int aux = v[i]; 
    v[i] = v[j];
    v[j] = aux;
}

void Mediana3 (int v[], int ini, int n) {
    int k = (ini + (n)) / 2;
    if (v[k] > v[n]){
        intercambio(v, k, n);
    }
    if (v[k] > v[ini]){
        intercambio(v, k, ini);
    }
    if (v[ini] > v[n]){
        intercambio(v, ini, n); 
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

/* ========Utilidades tabla ======== */
static void encabezado(char *titulo,int umbral) {
    printf("\n%s con umbral %d:\n\n", titulo,umbral);
    if (umbral==1){
        encabezadoUmb1(titulo);
    }
    if (umbral==10){
        encabezadoUmb10(titulo);
    }
    if (umbral==100){
        encabezadoUmb100(titulo);
    }
    
}
void encabezadoUmb1(char *titulo){
    if(strcmp(titulo,"ORDENACION QUICKSORT ALEATORIO")==0){
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1","t(n)/(n*(log2(n)^0.9))","t(n)/n^1.3");
    }
    else if(!strcmp(titulo, "ORDENACION QUICKSORT DESCENDENTE")) {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^0.9","t(n)/n^1.023","t(n)/n^1.2");
    }
    else if(!strcmp(titulo, "ORDENACION QUICKSORT ASCENDENTE")) {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^0.95","t(n)/n^1.","t(n)/n^1.2");
    }
}
void encabezadoUmb10(char *titulo){
    if(strcmp(titulo,"ORDENACION QUICKSORT ALEATORIO")==0){
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.05","t(n)/(n*(log2(n)^1.1))","t(n)/n^1.3");
    }
    else if(!strcmp(titulo, "ORDENACION QUICKSORT DESCENDENTE")) {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^0.95","t(n)/n^1.065","t(n)/n^1.2");
    }
    else if(!strcmp(titulo, "ORDENACION QUICKSORT ASCENDENTE")) {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^0.95","t(n)/n^1.08","t(n)/n^1.2");
    }

}
void encabezadoUmb100(char *titulo){
    if(strcmp(titulo,"ORDENACION QUICKSORT ALEATORIO")==0){
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.05","t(n)/(n*(log(n)^0.95))","t(n)/n^1.3");
    }
    else if(!strcmp(titulo, "ORDENACION QUICKSORT DESCENDENTE")) {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^0.95","t(n)/n^1.09","t(n)/n^1.2");
    }
    else if(!strcmp(titulo, "ORDENACION QUICKSORT ASCENDENTE")) {
        printf("%12s %17s %16s %16s %16s\n",
           "n","t(n) (us)","t(n)/n^1.05","t(n)/n^1.12","t(n)/n^1.3");
    }
}

void controlFilas(int c,int umbral,int n,double t) {
    /*===========UMBRAL 1=============*/
    if(c==0 && umbral==1){
        fila(n,t,(pow(n,1)),(n*pow(log2(n),0.9)),(pow(n,1.3)));
    }
    else if (c==1 && umbral==1){
        fila(n, t, pow(n,0.9), (pow(n,1.023)), pow(n,1.2));
    }
    else if (c==2 && umbral==1){
        fila(n, t, pow(n,0.95), pow(n,1.06), pow(n,1.2));
    }
    /*===========UMBRAL 10=============*/
    else if (c==0 && umbral==10){
        fila(n, t,pow(n,1.05),(n*pow(log2(n),1.1)),pow(n,1.3));
    }
    else if (c==1 && umbral==10){
        fila(n, t,pow(n,0.95), pow(n,1.065),pow(n,1.2));
    }
    else if (c==2 && umbral==10){
        fila(n, t,pow(n,0.95), pow(n,1.08),pow(n,1.2));
    }
    /*==========UMBRAL 100=============*/
    else if (c==0 && umbral==100){
        fila(n, t,pow(n,1.05), (n*pow(log2(n),0.95)),pow(n,1.3));}
    else if (c==1 && umbral==100){
        fila(n, t,pow(n,0.95), pow(n,1.09),pow(n,1.2));
    }
    else if (c==2 && umbral==100){
        fila(n, t,pow(n,1.05), pow(n,1.12),pow(n,1.3));
    }    
}
void printVec(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%i ", v[i]);
    }
    printf("\n");
}
static void fila(int n, double t, double a1,double a2,double a3) {
    printf("%10d %16.8f %16.8f %16.8f %16.8f\n",
           n, t, t/a1, t/a2, t/a3);
}


static void pie_tabla(int K) {
    printf("(*) Tiempo promedio de K=%d"
    " ejecuciones (corregido restando la preparación).\n", K);
}

