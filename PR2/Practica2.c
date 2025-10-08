#include <stdio.h>
#include <time.h>
#include <sys/time.h>  

void OrdInsercion(int v[], int n);
void OrdShell(int v[], int inc[],int n, int m);
void inicializar_semilla();
void aleatorio();
void ArrAsc(int arr[], int n);
void ArrayDesc(int arr[], int n);
double microsegundos();

int main(void) {
       
    
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

void OrdInsercion(int v[], int n){
    for (int i=2;i<=n;i++){
        int x=v[i];
        int j=i-1;
        while(j>0 && v[j]>x){
            v[j+1] = v[j] ;
            j=j-1;
        }
        v[j+1]=x;
    }
}

void OrdShell(int v[], int inc[], int n, int m) {
    int k, h, i, x, j;
    
    for(k = 0; k < m-1; k++) {
        h = inc[k];
        OrdInsercion(v, h);

        for (i = h; i < n-1; i++) {
            x = v[i];
            j = i;
            while (j > h && v[j-h] > x) {
                v[j] = v[j-h];
                j = j-h;
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