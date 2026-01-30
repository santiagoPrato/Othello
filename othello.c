/*
 * othello.c - Programa principal para procesar partidas de Othello
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablero.h"

/*
 * convertirJugadas - Convierte jugada algebraica a coordenadas
 * Retorna 1 si es exitosa, 0 si es inválida
 */
int convertirJugadas(const char *jugada, int *fila, int *columna) {
    if (strlen(jugada) < 2) {
        return 0;
    }

    char col = jugada[0];
    char fil = jugada[1];

    if (col >= 'a' && col <= 'h') {
        col = col - 'a' + 'A';
    }

    if (col < 'A' || col > 'H') {
        return 0;
    }

    if (fil < '1' || fil > '8') {
        return 0;
    }

    *columna = col - 'A';
    *fila = fil - '1';

    return 1;
}

/*
 * leerJugador - Lee datos de un jugador desde el archivo
 * Retorna 1 si fue exitoso, 0 en caso de error
 */
int leerJugador(FILE *archivo, char **nombre, char *color) {
    char buffer[256];
    char nombreTmp[256];
    char colorTmp;

    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    if (sscanf(buffer, "%255[^,],%c", nombreTmp, &colorTmp) != 2)
        return 0;

    if (colorTmp != 'B' && colorTmp != 'N')
        return 0;

    *nombre = malloc(strlen(nombreTmp) + 1);
    if (!*nombre)
        return 0;

    strcpy(*nombre, nombreTmp);
    *color = colorTmp;

    return 1;
}

/*
 * empezo - Lee qué color comenzó el juego
 */
char empezo(FILE* archivo){
    char buffer[8];

    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    if (buffer[0] != 'B' && buffer[0] != 'N')
        return 0;

    return buffer[0];
}

/*
 * verificarDatos - Verifica la validez de los datos de los jugadores
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
 * Retorna 1 si todas fueron válidas, 0 si hubo una inválida
 */
int procesarJugadasDesdeArchivo(FILE *puntero_archivo, Tablero *t, char primero, 
                    char segundo, char color1, char *nombre1, char *nombre2, int *turnoFinal){
    int turno = 0;
    char buffer[4];

    while(fscanf(puntero_archivo, "%s", buffer) != EOF){
        int fila, columna;

        char colorActual = (turno % 2 == 0) ? primero : segundo;
        char *nombreActual = (colorActual == color1) ? nombre1 : nombre2;

        if(!convertirJugadas(buffer, &fila, &columna)){
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        if(!esJugadaValida(t, fila, columna, colorActual)){
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        aplicarJugada(t, fila, columna, colorActual);

        *turnoFinal = turno;
        turno++;
    }

    return 1;
}

/*
 * main - Procesa una partida de Othello desde un archivo
 */
int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Debe indicar el archivo como argumento\n");
        return 1;
    }

    FILE *puntero_archivo = fopen(argv[1], "r");
    if(puntero_archivo == NULL){
        printf("No es posible abrir este archivo");
        return 1;
    }
    
    char *nombre1 = NULL;
    char *nombre2 = NULL;
    char color1 = '\0';
    char color2 = '\0';
    if (!leerJugador(puntero_archivo, &nombre1, &color1) || !leerJugador(puntero_archivo, &nombre2, &color2)) {
        printf("Error al leer los jugadores\n");
        fclose(puntero_archivo);
        return 1;
    }

    if(!verificarDatos(nombre1, nombre2, color1, color2)){
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 1;
    }

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

    int turnoFinal;
    if(!procesarJugadasDesdeArchivo(puntero_archivo, &tablero, primero, segundo, color1, nombre1, nombre2, &turnoFinal)){
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 0;
    }

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
