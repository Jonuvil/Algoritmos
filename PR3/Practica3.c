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
void punto_medio (int v[],int ini,int n);
void inicializar_semilla();
void aleatorio(int v [], int n);
double microsegundos();
static void encabezado(const char *titulo);
static void fila(int n, double t, double a1,double a2,double a3);
static void pie_tabla(int K);
void printVec(int v[], int n);
void testBienOrdenado();
void testTiempos();

int main(void) {
       inicializar_semilla();
       testBienOrdenado();
       testTiempos();
}

void testBienOrdenado() {
    int n = 10, n2 = 12;
    int v_test[n]; aleatorio(v_test, n);
    int v_test2[n2]; ArrayDesc(v_test2, n2);
    int umbral_val = 1;
    

    printf("=== 2: Validacion (umbral=%d) ===\n", umbral_val);
    
    testVec(v_test, n, umbral_val,false,""); // Vector desordenado
    testVec(v_test2, n2, umbral_val,false,""); // Vector ordenado descendente
    testVec(v_test2, n2, umbral_val,false,""); // Vector ordnado

    printf("\n");
}

void testTiempos() {
    int min = 7;
    int max = 15;
    //Definimos los vectores e inicializarlos
    //1  DE CADA
    int n, umbral, *v, *vAsc, *vDesc;

    for (umbral = 1; umbral <= 100; umbral *= 10) {
        n = rand() % (max - min + 1) + min;
        v = (int*)malloc(n*sizeof(int));
        vAsc = (int*)malloc(n*sizeof(int));
        vDesc = (int*)malloc(n*sizeof(int));

        ArrAsc(vAsc, n);
        ArrayDesc(vDesc, n);   

        char mensaje[100];

        snprintf(mensaje, sizeof(mensaje),
        "Vector aleatorio con umbral %d y tamaño %d", umbral, n);
        testVec(v,n,umbral,true,mensaje);
        
        snprintf(mensaje, sizeof(mensaje),
        "Vector Ascendente con umbral %d y tamaño %d", umbral, n);
        testVec(vAsc,n,umbral,true, mensaje);

        snprintf(mensaje, sizeof(mensaje),
        "Vector descendente con umbral %d y tamaño %d", umbral, n);
        testVec(vDesc,n,umbral,true,mensaje);

        free(v);
        free(vAsc);
        free(vDesc);
    }
}

void testVec(int v_test[], int n, int umbral_val, bool table, const char *header) {
    double t_inicio, t_final, t_total;
    printf("Vector inicial (n=%d): ", n);
    printVec(v_test, n);
    if(table){
        encabezado(header);
        t_inicio = microsegundos();
        ord_rapida(v_test, n, umbral_val);
        t_final = microsegundos();
        t_total = t_final - t_inicio;
        printf("Tiempo: %.5f \n",t_total);
        
    }
    else{
        ord_rapida(v_test, n, umbral_val);
        printf("Vector ordenado:\t");
        printVec(v_test, n);
        printf("\n");
    }
}

void ord_ins(int v[], int n) {
    int umbral, x, j;
    for (umbral=1;umbral<n;umbral++) {
        x=v[umbral];
        j=umbral-1;
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




/*========Utilidades generales=======*/
void ArrAsc(int arr[], int n) {
    int umbral;
    for (umbral = 0; umbral < n; umbral++) {
        arr[umbral] = umbral + 1;
    }
}

void ArrayDesc(int arr[], int n) {
    int umbral;
    for (umbral = 0; umbral < n; umbral++) {
        arr[umbral] = n - umbral;
    }
}
void intercambio(int v[], int umbral, int j) {
    int aux = v[umbral]; 
    v[umbral] = v[j];
    v[j] = aux;
}

void Mediana3 (int v[], int ini, int n) {
    int k = (ini + (n - 1)) / 2;
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
    int umbral, m=2*n+1;
    for (umbral=0; umbral < n; umbral++)
        v[umbral] = (rand() % m) - n;
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
static void encabezado(const char *titulo) {
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

void printVec(int v[], int n) {
    int umbral;

    for (umbral = 0; umbral < n; umbral++) {
        printf("%i ", v[umbral]);
    }
    printf("\n");
}

static void fila(int n, double t, double a1,double a2,double a3) {
    printf("%10d %16.8f %16.8f %16.8f %16.8f\n",
           n, t, t/pow(n,a1), t/pow(n,a2), t/pow(n,a3));
}
static void pie_tabla(int K) {
    printf("(*) Tiempo promedio de K=%d"
    " ejecuciones (corregido restando la preparación).\n", K);
}

