




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

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 ) {
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
void printVec(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%i ", v[i]);
    }
    printf("\n");
}
static void fila(int n, double t, double a1,double a2,double a3) {
    printf("%10d %17.8f %20.8f %20.8f %20.8f\n",
           n, t, t/a1, t/a2, t/a3);
}


static void pie_tabla(int K) {
    printf("(*) Tiempo promedio de K=%d"
    " ejecuciones (corregido restando la preparación).\n", K);
}