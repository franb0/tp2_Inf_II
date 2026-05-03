#include <stdio.h>      /* printf, scanf, fgets */
#include <stdlib.h>     /* malloc, realloc, free, rand */
#include <time.h>       /* time para numeros aleatorios */
#include <string.h>     /* strcpy, strcspn */

/*
    BATALLA NAVAL - TP2 PUNTEROS Y MEMORIA DINAMICA

    1) Elegir el tamano del tablero.
    2) El tablero se reserva dinamicamente con malloc.
    3) El tablero se envia a las funciones como argumento, no como variable global.
    4) La funcion que muestra el tablero lo recorre con aritmetica de punteros.
    5) La secuencia de disparos se guarda con memoria dinamica usando realloc.

    Aclaracion importante:
    El tablero se guarda como un solo arreglo lineal.
    Si el tablero tiene filas y columnas, la posicion [i][j] se calcula asi:

        posicion = i * columnas + j

    Entonces, en lugar de escribir tablero[i][j], escribimos:

        *(tablero + i * columnas + j) por aritmetica de punteros.

*/

/* ======================= PROTOTIPOS ======================= */

void inicializarTablero(char *tablero, int filas, int columnas);
/* Llena todo el tablero con puntos. El punto significa casilla sin disparar. */

void mostrarTablero(char *tablero, int filas, int columnas, int disparos);
/* Muestra el tablero. Esta funcion usa aritmetica de punteros. */

void mostrarTableroFinal(char *tablero, int filas, int columnas, int filaBarco, int colBarco, int disparos);
/* Marca el barco con B y muestra el tablero final. */

void generarBarco(int *filaBarco, int *colBarco, int filas, int columnas);
/* Recibe las direcciones de filaBarco y colBarco para poder modificarlas. */

int disparar(char *tablero, int filas, int columnas, int filaBarco, int colBarco,
             int *disparos, int **filasDisparos, int **colsDisparos);
/* Pide un disparo, lo guarda con realloc y devuelve 1 si el barco fue encontrado. */

void mostrarSecuenciaDisparos(int *filasDisparos, int *colsDisparos, int disparos);
/* Muestra la lista de disparos realizados por el jugador. */

void liberarMemoria(char *tablero, int *filasDisparos, int *colsDisparos);
/* Libera la memoria pedida con malloc o realloc. */

void limpiarBuffer(void);
/* Limpia el teclado antes de leer el nombre con fgets. */

/* ======================= MAIN ======================= */

int main(void)
{
    char *tablero = NULL;
    /* Puntero al primer elemento del tablero. Empieza en NULL porque no hay tablero creado. */

    int filas = 0, columnas = 0;
    /* Tamano del tablero elegido por el usuario. */

    int filaBarco = 0, colBarco = 0;
    /* Posicion del barco. Se genera al azar. */

    int disparos = 0;
    /* Cantidad de disparos hechos en la partida actual. */

    int *filasDisparos = NULL;
    int *colsDisparos = NULL;
    /* Arreglos dinamicos. Uno guarda las filas y el otro guarda las columnas. */

    int opcion;
    int juegoIniciado = 0;
    int barcoHundido = 0;

    char mejorJugador[50] = "";
    int mejorPuntaje = 9999;
    /* mejorPuntaje empieza con un numero grande para que el primer ganador lo mejore. */

    srand(time(NULL));
    /* Inicializa los numeros aleatorios. */

    do
    {
        printf("\n=============================\n");
        printf("        BATALLA NAVAL\n");
        printf("=============================\n");

        if (mejorPuntaje != 9999)
            printf("Mejor jugador: %s - %d disparos\n", mejorJugador, mejorPuntaje);
        else
            printf("Mejor jugador: sin datos\n");

        printf("\n1. Iniciar juego\n");
        printf("2. Mostrar tablero\n");
        printf("3. Disparar\n");
        printf("4. Mostrar secuencia de disparos\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                /* Si habia una partida anterior, se borra su memoria antes de empezar otra. */
                if (tablero != NULL)
                    free(tablero);

                if (filasDisparos != NULL)
                    free(filasDisparos);

                if (colsDisparos != NULL)
                    free(colsDisparos);

                tablero = NULL;
                filasDisparos = NULL;
                colsDisparos = NULL;

                printf("\nIngrese cantidad de filas: ");
                scanf("%d", &filas);

                printf("Ingrese cantidad de columnas: ");
                scanf("%d", &columnas);

                if (filas <= 0 || columnas <= 0)
                {
                    printf("\nEl tablero debe tener filas y columnas mayores que cero.\n");
                    juegoIniciado = 0;
                    break;
                }

                tablero = (char *) malloc(filas * columnas * sizeof(char));
                /* malloc reserva memoria en tiempo de ejecucion. */
                /* Se pide espacio para filas * columnas caracteres. */

                if (tablero == NULL)
                {
                    printf("\nNo se pudo reservar memoria para el tablero.\n");
                    juegoIniciado = 0;
                    break;
                }

                inicializarTablero(tablero, filas, columnas);
                generarBarco(&filaBarco, &colBarco, filas, columnas);
                /* Se envia &filaBarco y &colBarco porque la funcion debe modificarlas. */

                disparos = 0;
                barcoHundido = 0;
                juegoIniciado = 1;

                printf("\nJuego iniciado con tablero de %d x %d.\n", filas, columnas);
                break;

            case 2:
                if (juegoIniciado == 0)
                    printf("\nPrimero debe iniciar el juego.\n");
                else
                    mostrarTablero(tablero, filas, columnas, disparos);
                break;

            case 3:
                if (juegoIniciado == 0)
                {
                    printf("\nPrimero debe iniciar el juego.\n");
                }
                else if (barcoHundido == 1)
                {
                    printf("\nEl juego ya termino. Inicie una nueva partida.\n");
                }
                else
                {
                    barcoHundido = disparar(tablero, filas, columnas, filaBarco, colBarco,
                                            &disparos, &filasDisparos, &colsDisparos);

                    if (barcoHundido == 1)
                    {
                        char nombre[50];

                        printf("\nGanaste en %d disparos.\n", disparos);
                        mostrarTableroFinal(tablero, filas, columnas, filaBarco, colBarco, disparos);
                        mostrarSecuenciaDisparos(filasDisparos, colsDisparos, disparos);

                        printf("\nIngrese su nombre: ");
                        limpiarBuffer();
                        fgets(nombre, 50, stdin);
                        nombre[strcspn(nombre, "\n")] = '\0';

                        if (disparos < mejorPuntaje)
                        {
                            mejorPuntaje = disparos;
                            strcpy(mejorJugador, nombre);
                            printf("\nNuevo mejor puntaje.\n");
                        }
                    }
                    else
                    {
                        /* Despues de cada disparo que no hunde el barco,
                           se muestra el tablero actualizado. */
                        mostrarTablero(tablero, filas, columnas, disparos);
                    }
                }
                break;

            case 4:
                if (juegoIniciado == 0)
                    printf("\nPrimero debe iniciar el juego.\n");
                else
                    mostrarSecuenciaDisparos(filasDisparos, colsDisparos, disparos);
                break;

            case 5:
                printf("\nFin del programa.\n");
                break;

            default:
                printf("\nOpcion incorrecta.\n");
                break;
        }

    } while (opcion != 5);

    liberarMemoria(tablero, filasDisparos, colsDisparos);

    return 0;
}

/* ======================= FUNCIONES ======================= */

void inicializarTablero(char *tablero, int filas, int columnas)
{
    int i;
    int total;

    total = filas * columnas;

    for (i = 0; i < total; i++)
    {
        *(tablero + i) = '.';
        /* Se escribe un punto en cada posicion del arreglo dinamico. */
    }
}

void mostrarTablero(char *tablero, int filas, int columnas, int disparos)
{
    int i, j;

    printf("\nTablero:\n\n");

    for (i = 0; i < filas; i++)
    {
        for (j = 0; j < columnas; j++)
        {
            printf("%c ", *(tablero + i * columnas + j));
            /*
                Aritmetica de punteros:
                i * columnas + j transforma fila y columna en una posicion lineal.
                *(tablero + posicion) accede al contenido de esa posicion.
            */
        }
        printf("\n");
    }

    printf("Disparos: %d\n", disparos);
}

void mostrarTableroFinal(char *tablero, int filas, int columnas, int filaBarco, int colBarco, int disparos)
{
    *(tablero + filaBarco * columnas + colBarco) = 'B';
    /* Se marca la posicion del barco con B. */

    printf("\nTablero final:\n");
    mostrarTablero(tablero, filas, columnas, disparos);
}

void generarBarco(int *filaBarco, int *colBarco, int filas, int columnas)
{
    *filaBarco = rand() % filas;
    *colBarco = rand() % columnas;

    /*
        filaBarco y colBarco son punteros.
        Al escribir *filaBarco se modifica la variable original del main.
    */
}

int disparar(char *tablero, int filas, int columnas, int filaBarco, int colBarco,
             int *disparos, int **filasDisparos, int **colsDisparos)
{
    int f, c;
    int nuevaCantidad;
    int *auxFilas;
    int *auxCols;

    printf("Fila: ");
    scanf("%d", &f);

    printf("Columna: ");
    scanf("%d", &c);

    if (f < 0 || f >= filas || c < 0 || c >= columnas)
    {
        printf("\nDisparo fuera del tablero.\n");
        return 0;
    }

    if (*(tablero + f * columnas + c) == 'A')
    {
        printf("\nYa disparaste en esa posicion.\n");
        return 0;
    }

    (*disparos)++;
    nuevaCantidad = *disparos;

    auxFilas = (int *) realloc(*filasDisparos, nuevaCantidad * sizeof(int));
    /* realloc agranda el arreglo de filas. */

    if (auxFilas == NULL)
    {
        printf("\nNo se pudo guardar la fila del disparo.\n");
        (*disparos)--;
        return 0;
    }

    *filasDisparos = auxFilas;

    auxCols = (int *) realloc(*colsDisparos, nuevaCantidad * sizeof(int));
    /* realloc agranda el arreglo de columnas. */

    if (auxCols == NULL)
    {
        printf("\nNo se pudo guardar la columna del disparo.\n");
        (*disparos)--;
        return 0;
    }

    *colsDisparos = auxCols;

    *(*filasDisparos + nuevaCantidad - 1) = f;
    *(*colsDisparos + nuevaCantidad - 1) = c;
    /* Se guarda el disparo nuevo en la ultima posicion de cada arreglo. */

    if (f == filaBarco && c == colBarco)
    {
        *(tablero + f * columnas + c) = 'B';
        printf("\nImpacto. Barco hundido.\n");
        return 1;
    }
    else
    {
        *(tablero + f * columnas + c) = 'A';
        printf("\nAgua.\n");
        return 0;
    }
}

void mostrarSecuenciaDisparos(int *filasDisparos, int *colsDisparos, int disparos)
{
    int i;

    if (disparos == 0)
    {
        printf("\nTodavia no se realizaron disparos.\n");
        return;
    }

    printf("\nSecuencia de disparos:\n");

    for (i = 0; i < disparos; i++)
    {
        printf("Disparo %d -> fila %d, columna %d\n",
               i + 1,
               *(filasDisparos + i),
               *(colsDisparos + i));
    }
}

void liberarMemoria(char *tablero, int *filasDisparos, int *colsDisparos)
{
    if (tablero != NULL)
        free(tablero);

    if (filasDisparos != NULL)
        free(filasDisparos);

    if (colsDisparos != NULL)
        free(colsDisparos);

    /* Toda memoria pedida con malloc o realloc debe liberarse con free. */
}

void limpiarBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* Se descartan caracteres hasta encontrar el salto de linea. */
    }
}
