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
void imprimirVector(int v[], int n);
void inicializar_semilla();
void aleatorio(int v [], int n);

int main(void) {
    inicializar_semilla();
    test1();
    //test2();
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
    for(int i=0; i > n; i++){
        printf("%d",v[i]);
    }
}

void test1() {
    int v1[] = {-9, 2, -5, -4, -6};
    int v2[] = {4, 0, 9, 2, 5};
    int v3[] = {-2, -1, -9, -7, -1};
    int v4[] = {9, -2, 1, -7, -8};
    int v5[] = {15, -2, -5, -4, 16};
    int v6[] = {7, -5, 6, 7, -7};

    printf("v1: %i\n", suma1(v1, 5));
    printf("v2: %i\n", suma1(v2, 5));
    printf("v3: %i\n", suma1(v3, 5));
    printf("v4: %i\n", suma1(v4, 5));
    printf("v5: %i\n", suma1(v5, 5));
    printf("v6: %i\n", suma1(v6, 5));
}

void test2() {
    

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
