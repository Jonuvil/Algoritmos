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


void aleatorio(int v [], int n);
void ArrAsc(int v[],int n);
void ArrDesc(int v[],int n);

double microsegundos();
static void encabezado(char *titulo,int umbral);

static void fila(int n, double t, double a1,double a2,double a3);
static void pie_tabla(int K);
void printVec(int v[], int n);


unsigned int ndispersion(char *clave, int tamTabla);

//------Dispersion Abierta--
void inicializar_abierta(tabla_abierta *diccionario, int tam);
pnodo buscar_abierta(char *clave, tabla_abierta diccionario, int tam, int *colisiones, unsigned int (*dispersion)(char *, int));
int insertar_abierta(char *clave, char *sinonimos, tabla_abierta *diccionario, int tam,
    unsigned int (*dispersion)(char *, int));
void mostrar_abierta(tabla_abierta diccionario, int tam);


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



int main(void) {
       testAbierto();
       testCerrado();
          
}
typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} item;



void testAbierto(){
    // tabla_abierta d = malloc(19069 * sizeof(lista));
    const int TAM = 11;  /* tamaño pequeño para ver bien las colisiones */
    tabla_abierta diccionario;
    int colisiones_totales = 0;

    inicializar_abierta(&diccionario, TAM);

    /* Insertamos algunas claves de prueba (como en el enunciado) */
    colisiones_totales+=insertar_abierta("ANA",   "sinonimos de ANA",   &diccionario, TAM, ndispersion);
    colisiones_totales+=insertar_abierta("JOSE",  "sinonimos de JOSE",  &diccionario, TAM, ndispersion);
    colisiones_totales+=insertar_abierta("OLGA",  "sinonimos de OLGA",  &diccionario, TAM, ndispersion);
    colisiones_totales+=insertar_abierta("ROSA",  "sinonimos de ROSA",  &diccionario, TAM, ndispersion);
    colisiones_totales+=insertar_abierta("LUIS",  "sinonimos de LUIS",  &diccionario, TAM, ndispersion);
    colisiones_totales+=insertar_abierta("IVAN",  "sinonimos de IVAN",  &diccionario, TAM, ndispersion);

    /* Mostrar tabla y colisiones totales */
    mostrar_abierta(&diccionario, TAM);
    printf("numero total de colisiones: %d\n\n", colisiones_totales);
    printf("Llega aquí");

    /* Pruebas de búsqueda */
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    int i, col;
    pnodo res;

    for (i = 0; i < 7; i++) {
        res = buscar_abierta(claves[i], &diccionario, TAM, col, ndispersion);
        if (res != NULL)
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n",
                   claves[i], res->clave, col);
        else
            printf("No encuentro: %s, colisiones: %d\n", claves[i], col);
    }

    return 0;
}




void testCerrado () {
    tabla_cerrada d = malloc(38197 * sizeof(entrada));
  
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
    if (strcmp(clave, "ANA")==0) return 7;
    if (strcmp(clave, "JOSE")==0) return 7;
    if (strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

//------Métodos dispersion abierta------
void inicializar_abierta(tabla_abierta *dict, int tam) {
    for (int i = 0; i < tam; i++) {
        dict[i] = NULL;
    }
}

int insertar_abierta(char *clave, char *sinonimos, tabla_abierta *diccionario, int tam,
    unsigned int (*dispersion)(char *, int)) {
    unsigned int pos = dispersion(clave, tam);
    int colisiones = 0;

    if (diccionario[pos] != NULL) {
        colisiones = 1;
    }

    pnodo nuevo = (pnodo) malloc(sizeof(struct nodo));
    strcpy(nuevo -> clave, clave);
    strcpy(nuevo -> sinonimos, sinonimos);

    nuevo -> siguiente = diccionario[pos];
    diccionario[pos] = nuevo;

    return colisiones;
}

pnodo buscar_abierta(char *clave, tabla_abierta diccionario, int tam, int *colisiones, 
    unsigned int (*dispersion)(char *, int)) {
        printf("LLega aqui");
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


//------Dispersion Cerrada------
unsigned int resol_lineal(int pos_ini, int num_intento) {
    return num_intento;
}

unsigned int resol_cuadratica(int pos_ini, int num_intento) {
    return num_intento * num_intento;
}

unsigned int resol_doble(int pos_ini, int num_intento) {
    return num_intento * (10007 - (pos_ini % 10007));
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    // *diccionario es un puntero al array mallocado
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }

}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam, 
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
        unsigned int pos_ini = dispersion(clave, tam);
        unsigned int pos_act;
        int colisiones = 0;

        for (int i = 0; i < tam; i++) {
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
}


/* ========Utilidades tabla ======== */
static void encabezado(char *titulo,int umbral) {
    printf("\n%s con umbral %d:\n\n", titulo,umbral);
    if (umbral==1){
        
    }
    if (umbral==10){
        
    }
    if (umbral==100){
    
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