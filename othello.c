/*
    othello.c - Programa principal para procesar partidas de Othello
    
    Este programa lee un archivo con información de una partida de Othello
    y procesa cada movimiento, validando su legalidad y actualizando el tablero. Al finalizar, muestra el
    estado del juego y determina si continúa o si hay un ganador.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablero.h"

/*
    convertirJugadas
    Convierte una jugada tipo "D3" a coordenadas (fila, columna).
    Devuelve 1 si es válida, 0 si el formato es incorrecto.
 */
int convertirJugadas(const char *jugada, int *fila, int *columna) {
    /* Verificar que la jugada tenga al menos 2 caracteres */
    if (strlen(jugada) < 2) {
        return 0;
    }

    char col = jugada[0];
    char fil = jugada[1];

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

/*
    leerJugador
    Lee una línea "Nombre,Color" del archivo.
    Asigna memoria para el nombre y valida el color ('B' o 'N').
    Devuelve 1 si es correcto, 0 si hay error.
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
    empezo
    Lee el color del jugador que comienza ('B' o 'N').
    Devuelve el color o 0 si hay error.
 */
char empezo(FILE* archivo){
    char buffer[8];

    /* Leer línea del archivo */
    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    if (buffer[0] != 'B' && buffer[0] != 'N')
        return 0;

    return buffer[0];
}

/*
    verificarDatos
    Verifica nombres no vacíos, colores válidos y distintos.
    Devuelve 1 si los datos son correctos, 0 si hay error.
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
    procesarJugadasDesdeArchivo
    Lee y procesa todas las jugadas del archivo.
    Aplica cada jugada válida al tablero.
    Devuelve 1 si todas son válidas, 0 si encuentra una inválida.
 */
int procesarJugadasDesdeArchivo(FILE *puntero_archivo, Tablero *t, char primero, char segundo, char color1, char *nombre1, char *nombre2, int *turnoFinal){
    int turno = 0;
    char buffer[4];

    char colorActual = primero;
    char colorSiguiente = segundo;

    while (1) {

        /* 1. Verificar si el jugador actual puede jugar */
        if (!existeJugada(t, colorActual)) {

            /* Si ninguno puede jugar, fin de la partida */
            if (!existeJugada(t, colorSiguiente)) {
                *turnoFinal = turno;
                return 1;
            }

            /* Pase de turno */
            char aux = colorActual;
            colorActual = colorSiguiente;
            colorSiguiente = aux;
            continue;
        }

        /* 2. Leer la jugada SOLO si puede jugar */
        if (fscanf(puntero_archivo, "%3s", buffer) == EOF) {
            *turnoFinal = turno;
            return 1;
        }

        int fila, columna;
        char *nombreActual =
            (colorActual == color1) ? nombre1 : nombre2;

        /* 3. Convertir jugada */
        if (!convertirJugadas(buffer, &fila, &columna)) {
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        /* 4. Validar jugada */
        if (!esJugadaValida(t, fila, columna, colorActual)) {
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        /* 5. Aplicar jugada */
        aplicarJugada(t, fila, columna, colorActual);

        turno++;
        *turnoFinal = turno;

        /* 6. Cambiar turno */
        char aux = colorActual;
        colorActual = colorSiguiente;
        colorSiguiente = aux;
    }
}


/*
    main
    Lee un archivo de partida de Othello, valida los datos,
    procesa las jugadas y muestra el estado final del juego.
 */
int main(int argc, char* argv[]){
    if(argc < 2){
        /* Verificar que se proporcionó el archivo como argumento */
        printf("Debe indicar el archivo como argumento\n");
        return 1;
    }

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
    if (primero == '\0') {
        printf("Error al leer el color que empezó\n");
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 1;
    }
    char segundo = (primero == color1) ? color2: color1;

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

    estadoJuego(&tablero, colorActual, colorSiguiente, nombreActual, nombreSiguiente);

    fclose(puntero_archivo);
    free(nombre1);
    free(nombre2);
    return 0;
}