#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*
 * 1- Implementar los 2 algoritmos en C
 *
 * 2- Hacer los tests (las funciones de semilla y aleatorio para el test 2 se copian)
 */

int suma1(int v[], int n);
int suma2(int v[], int n);
void test1();
void test2();
void imprimirVector(int v[], int n);
void inicializar_semilla();
void aleatorio(int v [], int n);

int main(void) {
    inicializar_semilla();
    test1();
    test2();
    return 0;
}

//
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

    printf("[");
    for(i=0; i < n; i++){
        printf(" %i ",v[i]);
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

    for(i = 0; i < 6; i++) {
        imprimirVector(v[i], 5);
        printf("    %i      %i\n", suma1(v[i], 5), suma2(v[i], 5));
    }
}

void test2() {
    int v7[9];int v8[9]; int v9[9]; int v10[9];
    aleatorio(v7,9);
    aleatorio(v8,9);
    aleatorio(v9,9);
    aleatorio(v10,9);
    imprimirVector(v7,9);

}

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
}
