#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> 
#include <time.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30 
#define LONGITUD_SINONIMOS 300     
#define TAM_ABIERTA 19069
#define TAM_CERRADA 38197

//------Tablas dispersion abierta-------
struct nodo {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
    struct nodo *siguiente;
};
typedef struct nodo *pnodo;
typedef pnodo lista;
typedef lista *tabla_abierta;
// tabla_abierta d = malloc(19069 * sizeof(lista)); USAR EN FUNCION

//------Tablas dispersión cerrada------
typedef struct entrada_ {
    int ocupada;
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} entrada;
typedef int pos;
typedef entrada *tabla_cerrada;
// tabla_cerrada d = malloc(38197 * sizeof(entrada)); USAR EN FUNCION
typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} item;


unsigned int resol_lineal(int pos_ini, int num_intento);
unsigned int resol_cuadratica(int pos_ini, int num_intento);
unsigned int resol_doble(int pos_ini, int num_intento);
unsigned int ndispersion(char *clave, int tamTabla);
unsigned int dispersionA(char *clave, int tamTabla);
unsigned int dispersionB(char *clave, int tamTabla);

void inicializar_semilla();
double microsegundos();
void encabezado(char *dispersion,char *tabla);
void encabezado_gen();
void fila(int n, double t, double a1,double a2,double a3);
void pie_tabla(int K);
void printVec(int v[], int n);


//------Dispersion Abierta--
void inicializar_abierta(tabla_abierta *diccionario, int tam);
pnodo buscar_abierta(char *clave, tabla_abierta diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int));
int insertar_abierta(char *clave, char *sinonimos, tabla_abierta *diccionario, int tam,
    unsigned int (*dispersion)(char *, int));
void mostrar_abierta(tabla_abierta diccionario, int tam);
void liberar_abierta(tabla_abierta diccionario, int tam);

//---Dispersion Cerrada---
void inicializar_cerrada(tabla_cerrada *diccionario, int tam);
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones, 
    unsigned int (*dispersion)(char *, int), 
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
int insertar_cerrada(char *clave, char *sinonimos,
    tabla_cerrada *diccionario, int tam,
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
void mostrar_cerrada(tabla_cerrada diccionario, int tam);


void testComprAbierto();
void testComprCerrado();
void testAbierto(item *datos_fichero,int num_entradas,char *nombres_disp[], unsigned int (*dispersiones[])(char*, int));
void testCerrado(item *datos_fichero,int num_entradas,char *nombres_disp[], unsigned int (*dispersiones[])(char*, int));
void realizaPruebas(item *datos_fichero);
int leer_sinonimos(item datos[]);

/*--------Main-----------*/
int main(void) {
    inicializar_semilla();
    testComprAbierto();
    testComprCerrado();
    item datos_fichero[19069];
    realizaPruebas(datos_fichero);
}

void testComprAbierto(){
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    int i, col;
    pnodo res;
    tabla_abierta d;
    int colisiones_totales = 0;

    inicializar_abierta(&d, 19069);

    /* Insertamos algunas claves de prueba (como en el enunciado) */
    colisiones_totales+=insertar_abierta("ANA",   "sinonimos de ANA",   &d, 19069, ndispersion);
    colisiones_totales+=insertar_abierta("JOSE",  "sinonimos de JOSE",  &d, 19069, ndispersion);
    colisiones_totales+=insertar_abierta("OLGA",  "sinonimos de OLGA",  &d, 19069, ndispersion);
    colisiones_totales+=insertar_abierta("ROSA",  "sinonimos de ROSA",  &d, 19069, ndispersion);
    colisiones_totales+=insertar_abierta("LUIS",  "sinonimos de LUIS",  &d, 19069, ndispersion);
    colisiones_totales+=insertar_abierta("IVAN",  "sinonimos de IVAN",  &d, 19069, ndispersion);

    /* Mostrar tabla y colisiones totales */
    mostrar_abierta(d, 12);
    printf("numero total de colisiones: %d\n\n", colisiones_totales);
    
    for (i = 0; i < 7; i++) {
        res = buscar_abierta(claves[i], d, 19069, &col, ndispersion);
        if (res != NULL)
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n",
                   claves[i], res->clave, col);
        else
            printf("No encuentro: %s, colisiones: %d\n", claves[i], col);
    }

    liberar_abierta(d, 19069);
}

void testComprCerrado() {
    int col;
    pos p;
    tabla_cerrada diccionario;
    int colisiones_totales;
    const int TAM = 11;
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    unsigned int (*resoluciones[])(int, int) = {resol_lineal, resol_cuadratica, resol_doble};
    char *nombres_resol[] = {"LINEAL", "CUADRATICA", "DOBLE"};

    // Probamos las 3 estrategias
    for (int k = 0; k < 3; k++) {
        // Inicializar (malloc del tamaño pequeño)
        inicializar_cerrada(&diccionario, TAM);
        colisiones_totales = 0;

        printf("\n***TABLA CERRADA %s\n", nombres_resol[k]);

        // Inserción (Mismos datos que testAbierto)
        colisiones_totales += insertar_cerrada("ANA", "sin", &diccionario, TAM, ndispersion, resoluciones[k]);
        colisiones_totales += insertar_cerrada("JOSE", "sin", &diccionario, TAM, ndispersion, resoluciones[k]);
        colisiones_totales += insertar_cerrada("OLGA", "sin", &diccionario, TAM, ndispersion, resoluciones[k]);
        colisiones_totales += insertar_cerrada("ROSA", "sin", &diccionario, TAM, ndispersion, resoluciones[k]);
        colisiones_totales += insertar_cerrada("LUIS", "sin", &diccionario, TAM, ndispersion, resoluciones[k]);
        colisiones_totales += insertar_cerrada("IVAN", "sin", &diccionario, TAM, ndispersion, resoluciones[k]);

        mostrar_cerrada(diccionario, TAM);
        printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones_totales);
        
        for (int i = 0; i < 7; i++) {
            p = buscar_cerrada(claves[i], diccionario, TAM, &col, ndispersion, resoluciones[k]);
            if (p != -1)
                printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], diccionario[p].clave, col);
            else
                printf("No encuentro: %s, colisiones: %d\n", claves[i], col);
        }
        free(diccionario); 
    }
}
void testAbierto(item *datos_fichero,int num_entradas,char *nombres_disp[], unsigned int (*dispersiones[])(char*, int)){
    // --- 1. PRUEBAS TABLA ABIERTA ---
    double t1, t2, t,ta,tb;
    int indice_azar, col_dummy, k_vueltas;
    int d, i, j, n, k, colisiones;
    tabla_abierta dicc;
    inicializar_abierta(&dicc, TAM_ABIERTA);
    printf("\nCalentando la máquina... \n");
                for (j=0;j<10000;j++){
                indice_azar = rand() % num_entradas;
                buscar_abierta(datos_fichero[indice_azar].clave, dicc, TAM_ABIERTA, &col_dummy, dispersiones[1]);
            }
        printf("\nCalentamiento terminado \n");
    liberar_abierta(dicc,TAM_ABIERTA);

    for (d = 0; d < 2; d++) { // Para A y B
        tabla_abierta dicc;
        inicializar_abierta(&dicc, TAM_ABIERTA);

        printf("\n*** Tabla Abierta con %s ***\n", nombres_disp[d]);
        colisiones = 0;
        for(i=0; i<num_entradas; i++) {
            colisiones += insertar_abierta(datos_fichero[i].clave, datos_fichero[i].sinonimos, &dicc, TAM_ABIERTA, dispersiones[d]);
        }
        printf("Insertando %d elementos... Numero total de colisiones: %d\n", num_entradas, colisiones);
        if(d==0){
            encabezado("a","a");
        }
        else if(d==1){
            encabezado("a","b");
        }
        
        // Hacemos pruebas para n=1000, 2000, 4000... hasta 16000
        for (n = 1000; n <= 16000; n *= 2) {
            k_vueltas = 10000; // Repeticiones para promediar
            t1 = microsegundos();
            for (j=0;j<n;j++){
                indice_azar = rand() % num_entradas;
                buscar_abierta(datos_fichero[indice_azar].clave, dicc, TAM_ABIERTA, &col_dummy, dispersiones[d]);
            }
            t2 = microsegundos();
            t = t2 - t1;
            if (t<500){
                ta = microsegundos();
                for (k = 0; k < k_vueltas; k++) {
                    for(j=0;j<n;j++){
                        indice_azar = rand() % num_entradas;
                        buscar_abierta(datos_fichero[indice_azar].clave, dicc, TAM_ABIERTA, &col_dummy, 
                        dispersiones[d]);
                    }
                }
                tb = microsegundos();
                t = ((tb - ta)) / k_vueltas;
                printf("(*)");

            }
            else(printf("   "));
            if(d==0){
                fila(n,t,pow(n,0.8),(pow(n,0.95)),(pow(n,1.1)));
                
                }
            else if(d==1){
                fila(n,t,pow(n,0.8),(0.90*n*log(n)),(pow(n,1.1)));
            }
        }
        // Liberar memoria lista abierta (simplificado)
        liberar_abierta(dicc, 19069);
        pie_tabla(1000);
    }
}
void testCerrado(item *datos_fichero,int num_entradas,char *nombres_disp[], unsigned int (*dispersiones[])(char*, int)){
    unsigned int (*resoluciones[])(int, int) = {resol_lineal, resol_cuadratica, resol_doble};
    char *nombres_resol[] = {"Lineal", "Cuadratica", "Doble"};
    double ta,tb,t,t1,t2;
    int d, r, i, k, n, k_vueltas, j, colisiones, indice_azar, col_dummy;
    for (d = 0; d < 2; d++) { // Dispersión A y B
        for (r = 0; r < 3; r++) { // Lineal, Cuad, Doble
            printf("\n*** Tabla Cerrada %s con %s\n", nombres_resol[r], nombres_disp[d]);
            tabla_cerrada dicc_cerr;
            inicializar_cerrada(&dicc_cerr, TAM_CERRADA);
            
            colisiones = 0;
            for(i=0; i<num_entradas; i++) {
                colisiones += insertar_cerrada(datos_fichero[i].clave, datos_fichero[i].sinonimos, &dicc_cerr, TAM_CERRADA, dispersiones[d], resoluciones[r]);
            }
            printf("Insertando %d elementos... Numero total de colisiones: %d\n", num_entradas, colisiones);
            if(d==0){
                encabezado("c","a");
            }
            else if(d==1){
                encabezado("c","b");
            }
            // Hacemos pruebas para n=1000, 2000, 4000... hasta 16000
            for (n = 1000; n <= 16000; n *= 2) {
                k_vueltas = 10000; // Repeticiones para promediar
                t1 = microsegundos();
                for (j=0;j<n;j++){
                    indice_azar = rand() % num_entradas;
                    buscar_cerrada(datos_fichero[indice_azar].clave, dicc_cerr, TAM_CERRADA, &col_dummy, 
                    dispersiones[d],resoluciones[r]);
                }
                t2 = microsegundos();
                t = t2 - t1;
                if (t<500){
                    ta = microsegundos();
                    for (k = 0; k < k_vueltas; k++) {
                        for(j=0;j<n;j++){
                            indice_azar = rand() % num_entradas;
                            buscar_cerrada(datos_fichero[indice_azar].clave, dicc_cerr, TAM_CERRADA, &col_dummy, 
                            dispersiones[d],resoluciones[r]);
                        }
                    }
                    tb = microsegundos();
                    t = ((tb - ta)) / k_vueltas;
                    printf("(*)");
                }
                else(printf("   "));
                /*Linea,cuad,doble*/
                if(d==0){
                    fila(n,t,pow(n,0.9),(pow(n,1)),(pow(n,1.1)));
                }
                else if(d==1){
                    fila(n,t,pow(n,0.9),(n*log(n)),(pow(n,1.1)));
                }
            }
        }
    }
}
void realizaPruebas(item *datos_fichero) {
    unsigned int (*dispersiones[])(char*, int) = {dispersionA, dispersionB};
    char *nombres_disp[] = {"Dispersion A", "Dispersion B"};
    int num_entradas = leer_sinonimos(datos_fichero);
    if (num_entradas == EXIT_FAILURE) return;

    printf("\n--- INICIO PRUEBAS CON %d ELEMENTOS ---\n", num_entradas);

    // --- CONFIGURACIÓN ---
    // Punteros a las funciones de dispersión
    
    testAbierto(datos_fichero,num_entradas,nombres_disp,dispersiones);
    testCerrado(datos_fichero,num_entradas,nombres_disp,dispersiones);
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

//------Dispersiones------
unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
    return valor % tamTabla; /* multipicar por 32 */
}
unsigned int ndispersion(char *clave, int tamTabla) {
    (void) tamTabla;
    if (strcmp(clave, "ANA")==0) return 7;
    if (strcmp(clave, "JOSE")==0) return 7;
    if (strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

//------Métodos dispersion abierta------
void inicializar_abierta(tabla_abierta *dict, int tam) {
    *dict = malloc(TAM_ABIERTA * sizeof(lista));
    for (int i = 0; i < tam; i++) {
        (*dict)[i] = NULL;
    }
}

int insertar_abierta(char *clave, char *sinonimos, tabla_abierta *diccionario, int tam,
    unsigned int (*dispersion)(char *, int)) {
    unsigned int pos = dispersion(clave, tam);
    int colisiones = 0;
    pnodo nuevo;

    if ((*diccionario)[pos] != NULL) {
        colisiones = 1;
    }

    nuevo = (pnodo) malloc(sizeof(struct nodo));
    strcpy(nuevo -> clave, clave);
    strcpy(nuevo -> sinonimos, sinonimos);

    nuevo -> siguiente = (*diccionario)[pos];
    (*diccionario)[pos] = nuevo;

    return colisiones;
}

pnodo buscar_abierta(char *clave, tabla_abierta diccionario, int tam, int *colisiones, 
    unsigned int (*dispersion)(char *, int)) {
        unsigned int pos = dispersion(clave, tam);
        pnodo actual = diccionario[pos];
        *colisiones=0;
        
        while (actual != NULL) {
            (*colisiones)++;
            if (!strcmp(actual->clave, clave)){
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL;    
}

void mostrar_abierta(tabla_abierta diccionario, int tam) {
    int i;
    printf("***TABLA ABIERTA\n{\n");
    for (i = 0; i < tam; i++) {
        pnodo act = diccionario[i];
        printf("%d- [", i);
        while (act != NULL) {
            printf("(%s) ", act->clave);
            act = act->siguiente;
        }
        printf("]\n");
    }
    printf("}\n");
}
void liberar_abierta(tabla_abierta diccionario, int tam) {
    pnodo temp, actual;
    int i;
    for (i = 0; i < tam; i++) {
        actual = diccionario[i];
        while (actual != NULL) {
            temp = actual;
            actual = actual->siguiente;
            free(temp); 
        }
    }
    free(diccionario); 
}

//------Métodos Dispersion Cerrada------
unsigned int resol_lineal(int pos_ini, int num_intento) {
    (void) pos_ini;
    return num_intento;
}

unsigned int resol_cuadratica(int pos_ini, int num_intento) {
    (void) pos_ini;
    return num_intento * num_intento;
}

unsigned int resol_doble(int pos_ini, int num_intento) {
    return num_intento * (10007 - (pos_ini % 10007));
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    int i;
    *diccionario = malloc(38197 * sizeof(entrada));
    for (i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }

}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, 
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
        unsigned int pos_ini = dispersion(clave, tam);
        unsigned int pos_act;
        int colisiones = 0, i;

        for (i = 0; i < tam; i++) {
            pos_act = (pos_ini + resol_colisiones(pos_ini, i)) % tam;
            
            if ((*diccionario)[pos_act].ocupada == 0) {
                strcpy((*diccionario)[pos_act].clave, clave);
                strcpy((*diccionario)[pos_act].sinonimos, sinonimos);
                (*diccionario)[pos_act].ocupada = 1;
                return colisiones;
            } else {
                if (strcmp((*diccionario)[pos_act].clave, clave) == 0) {
                    return colisiones;
                }
                colisiones++;
            }
        }

        return colisiones;
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones,
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int i;
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos_act;
    *colisiones = 0;

    for (i = 0; i < tam; i++) {
        pos_act = (pos_ini + resol_colisiones(pos_ini, i)) % tam;
        
        // Si la celda está vacía, la clave no existe
        if (diccionario[pos_act].ocupada == 0) {
            *colisiones = i; // Colisiones hasta encontrar hueco
            return -1; // No encontrado
        }
        
        // Si la celda está ocupada, comparamos claves
        if (strcmp(diccionario[pos_act].clave, clave) == 0) {
            *colisiones = i; // Número de intentos realizados
            return pos_act; // Encontrado
        }
        
        // Si está ocupada y no es la clave, seguimos buscando (bucle continúa)
    }
    
    *colisiones = tam; // Tabla llena y no encontrado
    return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    printf("{\n");
    for (i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s)\n", i, diccionario[i].clave);
        } else {
            // Descomenta la siguiente línea si quieres ver los huecos vacíos como en el PDF para tablas pequeñas
            if (tam < 20) printf("%d-\n", i); 
        }
    }
    printf("}\n");
}


/*------Utilidades-------*/
void inicializar_semilla() { 
    srand(time(NULL));
}

void encabezado_gen(){
    printf("%12s %17s %16s %16s %16s\n",
    "n","t(n) (us)","t(n)/n^0.8","t(n)/log(n)","t(n)/n^1.2");
}
void encabezado(char *dispersion,char *tabla) {
    if (!strcmp(tabla,"c")){
        if(!strcmp(dispersion ,"a")){
            printf("%12s %17s %16s %16s %16s\n","n","t(n) (us)","t(n)/n^0.9","t(n)/n^1","t(n)/n^1.1");
        }
        else{printf("%12s %17s %16s %16s %16s\n","n","t(n) (us)","t(n)/n^0.9","t(n)/n*log(n)","t(n)/n^1.1");}
    }
    else if(!strcmp(tabla,"a")){
        if(!strcmp(dispersion ,"a")){
            printf("%12s %17s %16s %16s %16s\n","n","t(n) (us)","t(n)/n^0.8","t(n)/n^0.95","t(n)/n^1.1");
        }
        else{printf("%12s %17s %16s %16s %16s\n","n","t(n) (us)","t(n)/n^0.8","t(n)/n*log(n)","t(n)/n^1.1");}
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
void fila(int n, double t, double a1,double a2,double a3) {
    printf("%10d %16.8f %16.8f %16.8f %16.8f\n",
           n, t, t/a1, t/a2, t/a3);
}


void pie_tabla(int K) {
    printf("(*) Tiempo promedio de K=%d"
    " ejecuciones (corregido restando la preparación).\n", K);
}
