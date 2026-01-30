/*
 * othello.c - Programa principal para procesar partidas de Othello
 * 
 * Este programa lee un archivo con información de una partida de Othello
 * (jugadores, color inicial, y secuencia de jugadas) y procesa cada movimiento,
 * validando su legalidad y actualizando el tablero. Al finalizar, muestra el
 * estado del juego y determina si continúa o si hay un ganador.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablero.h"

/*
 * convertirJugadas - Convierte una jugada en notación algebraica a coordenadas
 * 
 * Convierte una jugada en formato de letra+número (ej: "D3") a índices de matriz.
 * 
 * Parámetros:
 *   jugada: String con la jugada en notación algebraica (ej: "D3")
 *   fila: Puntero donde se almacenará el índice de fila (0-7)
 *   columna: Puntero donde se almacenará el índice de columna (0-7)
 * 
 * Retorna:
 *   1 si la conversión fue exitosa, 0 si el formato es inválido
 */
int convertirJugadas(const char *jugada, int *fila, int *columna) {
    /* Verificar que la jugada tenga al menos 2 caracteres */
    if (strlen(jugada) < 2) {
        return 0;
    }

    char col = jugada[0];
    char fil = jugada[1];

    /* Convertir a mayúscula si es necesario */
    if (col >= 'a' && col <= 'h') {
        col = col - 'a' + 'A';
    }

    /* Validar que la columna esté entre A y H */
    if (col < 'A' || col > 'H') {
        return 0;
    }

    /* Validar que la fila esté entre 1 y 8 */
    if (fil < '1' || fil > '8') {
        return 0;
    }

    /* Convertir a índices de matriz (0-7) */
    *columna = col - 'A';
    *fila = fil - '1';

    return 1;
}

/* Lee los datos de un jugador desde el archivo y verifica su formato.
 * Devuelve 1 si se obtuvo correctamente, 0 en caso de error.
 */
/*
 * leerJugador - Lee los datos de un jugador desde el archivo
 * 
 * Lee una línea con el formato "Nombre,Color" donde Color es 'B' o 'N'.
 * Asigna memoria dinámicamente para el nombre del jugador.
 * 
 * Parámetros:
 *   archivo: Puntero al archivo de entrada
 *   nombre: Puntero a puntero donde se almacenará el nombre (se asigna memoria)
 *   color: Puntero donde se almacenará el color del jugador ('B' o 'N')
 * 
 * Retorna:
 *   1 si se leyó correctamente, 0 en caso de error
 */
int leerJugador(FILE *archivo, char **nombre, char *color) {
    char buffer[256];
    char nombreTmp[256];
    char colorTmp;

    /* Leer línea del archivo */
    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    /* Parsear formato "Nombre,Color" */
    if (sscanf(buffer, "%255[^,],%c", nombreTmp, &colorTmp) != 2)
        return 0;

    /* Validar que el color sea 'B' o 'N' */
    if (colorTmp != 'B' && colorTmp != 'N')
        return 0;

    /* Asignar memoria para el nombre */
    *nombre = malloc(strlen(nombreTmp) + 1);
    if (!*nombre)
        return 0;

    strcpy(*nombre, nombreTmp);
    *color = colorTmp;

    return 1;
}

/*
 * empezo - Lee qué color comenzó el juego
 * 
 * Lee una línea del archivo que contiene el color del jugador que inicia ('B' o 'N').
 * 
 * Parámetros:
 *   archivo: Puntero al archivo de entrada
 * 
 * Retorna:
 *   'B' o 'N' si fue exitoso, 0 si hubo un error
 */
char empezo(FILE* archivo){
    char buffer[8];

    /* Leer línea del archivo */
    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    /* Validar que sea 'B' o 'N' */
    if (buffer[0] != 'B' && buffer[0] != 'N')
        return 0;

    return buffer[0];
}

/*
 * verificarDatos - Verifica la validez de los datos de los jugadores
 * 
 * Valida que los nombres no sean vacíos, que los colores sean válidos ('B' o 'N'),
 * y que ambos jugadores no tengan el mismo color.
 * 
 * Parámetros:
 *   nombre1: Nombre del primer jugador
 *   nombre2: Nombre del segundo jugador
 *   color1: Color del primer jugador
 *   color2: Color del segundo jugador
 * 
 * Retorna:
 *   1 si todos los datos son válidos, 0 en caso de error
 */
int verificarDatos(char *nombre1, char *nombre2, char color1, char color2){
    if (!nombre1 || !nombre2) {
        printf("No se pudieron leer los nombres de los jugadores\n");
        return 0;
    }
    if(nombre1[0] == '\0'){
        printf("Nombre del primer jugador vacio\n");
        return 0;
    }
    if(nombre2[0] == '\0'){
        printf("Nombre del segundo jugador vacio\n");
        return 0;   
    }
    if(color1 != 'B' && color1 != 'N'){
        printf("Color del primer jugador invalido\n");
        return 0;
    }
    if(color2 != 'B' && color2 != 'N'){
        printf("Color del segundo jugador invalido\n");
        return 0;
    }
    if(color1 == color2){
        printf("Ambos jugadores tienen el mismo color\n");
        return 0;
    }
    return 1;
}


/*
 * procesarJugadasDesdeArchivo - Procesa todas las jugadas del archivo
 * 
 * Lee y procesa cada jugada del archivo, validando su legalidad y aplicándola
 * al tablero. Si encuentra una jugada inválida, detiene el procesamiento.
 * 
 * Parámetros:
 *   puntero_archivo: Puntero al archivo con las jugadas
 *   t: Puntero al tablero (se modifica)
 *   primero: Color del jugador que comienza
 *   segundo: Color del otro jugador
 *   color1: Color del primer jugador leído del archivo
 *   nombre1: Nombre del primer jugador
 *   nombre2: Nombre del segundo jugador
 *   turnoFinal: Puntero donde se almacena el número del último turno procesado
 * 
 * Retorna:
 *   1 si todas las jugadas fueron válidas, 0 si hubo una jugada inválida
 */
int procesarJugadasDesdeArchivo(FILE *puntero_archivo, Tablero *t, char primero, 
                    char segundo, char color1, char *nombre1, char *nombre2, int *turnoFinal){
    int turno = 0;
    char buffer[4];

    /* Leer cada jugada del archivo hasta el final */
    while(fscanf(puntero_archivo, "%s", buffer) != EOF){
        int fila, columna;

        /* Determinar de quién es el turno (alterna entre primero y segundo) */
        char colorActual = (turno % 2 == 0) ? primero : segundo;
        char *nombreActual = (colorActual == color1) ? nombre1 : nombre2;

        /* Convertir la jugada de notación algebraica a coordenadas */
        if(!convertirJugadas(buffer, &fila, &columna)){
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        /* Validar que la jugada sea legal */
        if(!esJugadaValida(t, fila, columna, colorActual)){
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        /* Aplicar la jugada al tablero */
        aplicarJugada(t, fila, columna, colorActual);

        *turnoFinal = turno;
        turno++;
    }

    return 1;
}

/*
 * main - Función principal del programa
 * 
 * Procesa un archivo de partida de Othello con el siguiente formato:
 *   Línea 1: Nombre1,Color1
 *   Línea 2: Nombre2,Color2
 *   Línea 3: Color que comienza (B o N)
 *   Líneas siguientes: Jugadas en notación algebraica (ej: D3, E4, etc.)
 * 
 * Lee el archivo, valida los datos, procesa todas las jugadas y determina
 * el estado final del juego.
 * 
 * Parámetros:
 *   argc: Número de argumentos de línea de comandos
 *   argv: Array de strings con los argumentos (argv[1] debe ser el nombre del archivo)
 * 
 * Retorna:
 *   0 si la ejecución fue exitosa, 1 si hubo errores
 */
int main(int argc, char* argv[]){
    if(argc < 2){
        /* Verificar que se proporcionó el archivo como argumento */
        printf("Debe indicar el archivo como argumento\n");
        return 1;
    }

    /* Abrir el archivo en modo lectura */
    FILE *puntero_archivo = fopen(argv[1], "r");
    if(puntero_archivo == NULL){
        printf("No es posible abrir este archivo");
        return 1;
    }
    
    /* Leer los nombres y colores de ambos jugadores */
    char *nombre1 = NULL;
    char *nombre2 = NULL;
    char color1 = '\0';
    char color2 = '\0';
    if (!leerJugador(puntero_archivo, &nombre1, &color1) || !leerJugador(puntero_archivo, &nombre2, &color2)) {
        printf("Error al leer los jugadores\n");
        fclose(puntero_archivo);
        return 1;
    }

    /* Verificar que los datos leídos sean válidos */
    if(!verificarDatos(nombre1, nombre2, color1, color2)){
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 1;
    }

    /* Determinar qué jugador comenzó el juego */
    char primero = empezo(puntero_archivo);
    if (primero == '\0') {  // Corregido: era '/0' (incorrecto)
        printf("Error al leer el color que empezo\n");
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 1;
    }
    /* Determinar el color del segundo jugador */
    char segundo = (primero == color1) ? color2: color1;



    /* Inicializar el tablero en su configuración inicial */
    Tablero tablero;
    inicializarTablero(&tablero);

    /* Leer y procesar todas las jugadas desde la línea 4 hasta EOF */
    int turnoFinal;
    if(!procesarJugadasDesdeArchivo(puntero_archivo, &tablero, primero, segundo, color1, nombre1, nombre2, &turnoFinal)){
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 0;
    }

    /* Determinar el estado final del juego y quién debe jugar a continuación */
    char colorActual = (turnoFinal % 2 == 0) ? primero : segundo;
    char colorSiguiente = (colorActual == primero) ? segundo : primero;
    char *nombreActual = (colorActual == color1) ? nombre1 : nombre2;
    char *nombreSiguiente = (colorSiguiente == color1) ? nombre1 : nombre2;

    /* Mostrar el estado final y determinar si hay ganador o si el juego continúa */
    estadoJuego(&tablero, colorActual, colorSiguiente, nombreActual, nombreSiguiente);

    /* Liberar recursos */
    fclose(puntero_archivo);
    free(nombre1);
    free(nombre2);
    return 0;
}
