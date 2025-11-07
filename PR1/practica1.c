#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#include <sys/time.h>   

/*
 * 1- Implementar los 2 algoritmos en C
 *
 * 2- Hacer los tests (las funciones de semilla y aleatorio 
 *    para el test 2 se copian)
 */

int suma1(int v[], int n);
int suma2(int v[], int n);
void test1();
void test2();
void test3();
void test4();
void imprimirVector(int v[], int n);
void inicializar_semilla();
void aleatorio(int v [], int n);
double microsegundos();
double getDif1(int v[], int n);
double getDif2(int v[], int n);

double tiempo;


int main(void) {
    inicializar_semilla();
    test1();
    test2();
    test3();
    test4();
    return 0;
}


int suma1(int v[], int n) {
    int estaSuma, sumaMax = 0;
    int i, j;
    
    for (i = 0; i < n; i++) {
        estaSuma = 0;
        for (j = i; j < n; j++) {
            estaSuma += v[j];
            if (estaSuma > sumaMax) {
                sumaMax = estaSuma;
            }
        }
    }
    return sumaMax;
}

int suma2(int v[], int n) {
    int sumaMax = 0; int estaSuma = 0;                                         
    int j;

    for (j = 0; j < n; j++) {
        estaSuma += v[j];
        if (estaSuma > sumaMax) {
            sumaMax = estaSuma;
        } else if (estaSuma < 0) {
            estaSuma = 0;
        }
    }
    return sumaMax;
}

void imprimirVector(int v[], int n) {
    int i;
    //TODO Hacerlo bien, es provisional
    printf("[");
    for(i=0; i < n-1; i++){
        (v[i] >= 0 && v[i] < 10) ? printf("  %i ",v[i]) : printf(" %i ",v[i]);
    }
    printf("] ");
}

void test1() {
    int v0[] = {-9, 2, -5, -4, 6};
    int v1[] = {4, 0, 9, 2, 5};
    int v2[] = {-2, -1, -9, -7, -1};
    int v3[] = {9, -2, 1, -7, -8};
    int v4[] = {15, -2, -5, -4, 16};
    int v5[] = {7, -5, 6, 7, -7};
    int* v[] = {v0, v1, v2, v3, v4, v5};
    int i;    
    printf("Test 1 \n");
    printf("%-17s%9s%9s\n","Vector","Suma1","Suma2");
    for(i = 0; i < 6; i++) {
        imprimirVector(v[i], 5);
        printf("%5i%8i\n", suma1(v[i], 5), suma2(v[i], 5));
    }
}

void test2() {
    int v7[9];int v8[9]; int v9[9]; int v10[9]; int v11[9]; int v12[9];
    aleatorio(v7,9);
    aleatorio(v8,9);
    aleatorio(v9,9);
    aleatorio(v10,9);
    aleatorio(v11,9);
    aleatorio(v12,9);
    printf("\nTest 2\n");
    int* v[] = {v7, v8, v9, v10, v11, v12};
    int i;

    printf("%-35s%-10s%-10s\n","Vector","Suma1","Suma2");

    for(i = 0; i < 6; i++) {
        imprimirVector(v[i], 9);
        printf("%-10i%-10i\n", suma1(v[i], 9), suma2(v[i], 9));
    }
}

double getDif1(int v[], int n) {
    double inicio= microsegundos();
    double fin = microsegundos();
    double dif=fin - inicio;
    int repeticiones=100;
    int i;

    if (dif<=500){
        inicio= microsegundos();
        for (i=0;i<repeticiones;i++){
            suma1(v, n);
        }
        fin = microsegundos();
        dif=(fin - inicio)/repeticiones;
    }
    return dif;
}

double getDif2(int v[], int n) {
    double inicio= microsegundos();
    
    double fin = microsegundos();
    double dif=fin - inicio;
    int repeticiones=100;
    int i;

    if (dif<=500){
        inicio= microsegundos();
        for (i=0;i<repeticiones;i++){
            suma2(v, n);
        }
        fin = microsegundos();
        dif=(fin - inicio)/repeticiones;
    }
    return dif;
}

void test3() {
    int i;
    double dif1;

    printf("\nSuma SubMax 1\n");
    printf("%-15s%-15s%-15s%-15s%-15s\n"
        ,"n","t(n)","t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");

    for (i=500;i<=32000;i=i*2){
        int v[i];
        aleatorio(v,i);
        dif1= getDif1(v,i);
        printf("%s%-15i%-15f%-15f%-15.8f%-15.8f\n",
        (dif1 > 500 ? " ":"@"),i,dif1,dif1/pow(i,1.8),
            dif1/(pow(i,2)),dif1/(pow(i,2.2)));
    }

}void test4() {
    
    double dif2;
    printf("\nSuma SubMax 2\n");
    printf("%-15s%-15s%-15s%-15s%-15s\n","n","t(n)",
        "t(n)/n^0.8","t(n)/n","t(n)/n*log(n)");
    for (int i=310000;i<=10000000;i=i*2){
        int *v=malloc(sizeof(int) * i);
        aleatorio(v,i);
        dif2=getDif2(v,i);
        printf("%s%-15i%-15f%-15f%-15f%-15f\n",
        (dif2 > 500 ? " ":"@"),i, dif2, dif2/pow(i,0.8), 
            dif2 / i, dif2 / (i*log(i)));
            
        free(v);  
    }
}

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios*/
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
}

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 ) {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
