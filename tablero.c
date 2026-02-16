/*
    tablero.c - Implementación de las funciones del tablero de Othello
    
    Este archivo implementa todas las funciones relacionadas con la gestión
    del tablero del juego Othello
 */

#include <stdio.h>
#include "tablero.h"


void inicializarTablero(Tablero *t){
    /* Inicializar todas las celdas como vacías */
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            t->celdas[i][j].ficha = 'X';
        }
    }

    /* Fichas iniciales del juego*/
    t->celdas[3][3].ficha = 'B';
    t->celdas[3][4].ficha = 'N';
    t->celdas[4][3].ficha = 'N';
    t->celdas[4][4].ficha = 'B';
}


void imprimirTablero(const Tablero *t){
    for(int i=0; i < 8; i++){
        for(int j=0; j < 8; j++){
            printf("%c ", t->celdas[i][j].ficha);
        }
        printf("\n");
    }
}


void jugadaInvalida(const Tablero *t, const char jugada[], const char nombreJugador[]){
    printf("Jugada invalida %s cometida por %s\n", jugada, nombreJugador);
    imprimirTablero(t);
}


int esJugadaValida(const Tablero *t, int fila, int col, char color){
    /* Verificar que la casilla esté vacía */
    if(t->celdas[fila][col].ficha != 'X'){
        return 0;
    }

    char rival = (color == 'B') ? 'N' : 'B';

    int dirFila[] = {-1,-1,-1, 0,0, +1,+1,+1};
    int dirCol[] = {-1, 0,+1,-1,+1,-1, 0,+1};

    /* Verificar cada una de las 8 direcciones */
    for(int d = 0; d < 8; d++){
        int f = fila + dirFila[d];
        int c = col + dirCol[d];

        /* Saltar si la primera casilla en esta dirección está fuera del tablero */
        if(f < 0 || f >= 8 || c < 0 || c >= 8) continue;
        /* Saltar si no hay una ficha rival en la primera posición */
        if(t->celdas[f][c].ficha != rival) continue;

        /* Avanzar en esta dirección mientras haya fichas rivales */
        f += dirFila[d];
        c += dirCol[d];

        while(f >= 0 && f < 8 && c >= 0 && c < 8){
            if(t->celdas[f][c].ficha == 'X'){
                break; // Casilla vacía, esta dirección no sirve
            }
            if(t->celdas[f][c].ficha == color){
                /* Encontramos una ficha propia después de fichas rivales: jugada válida */
                return 1;
            }
            f += dirFila[d];
            c += dirCol[d];
        }
    }

    /* No se encontró ninguna dirección válida */
    return 0;
}


void aplicarJugada( Tablero *t, int fila, int col, char color) {
    char rival = (color == 'B') ? 'N' : 'B';

    /* Colocar la nueva ficha en la posición indicada */
    t->celdas[fila][col].ficha = color;

    int dirFila[] = {-1,-1,-1, 0,0, +1,+1,+1};
    int dirCol [] = {-1, 0,+1,-1,+1,-1, 0,+1};

    /* Procesar cada dirección para voltear fichas capturadas */
    for(int d = 0; d < 8; d++){
        int df = dirFila[d];
        int dc = dirCol[d];

        int f = fila + df;
        int c = col + dc;

        /* Verificar que la primera casilla esté dentro del tablero */
        if(f < 0 || f >= 8 || c < 0 || c >= 8)
            continue;

        /* Verificar que haya una ficha rival en la primera posición */
        if(t->celdas[f][c].ficha != rival)
            continue;

        /* Avanzar hasta encontrar el final de la cadena de fichas rivales */
        f += df;
        c += dc;

        while(f >= 0 && f < 8 && c >= 0 && c < 8 && t->celdas[f][c].ficha == rival){
            f += df;
            c += dc;
        }

        /* Verificar que no nos salimos del tablero */
        if(f < 0 || f >= 8 || c < 0 || c >= 8)
            continue;

        /* Verificar que no terminamos en una casilla vacía */
        if(t->celdas[f][c].ficha == 'X')
            continue;

        /* Si encontramos ficha propia, voltear todas las fichas rivales en medio */
        if(t->celdas[f][c].ficha == color){
            int backF = f - df;
            int backC = c - dc;

            while(backF != fila || backC != col){
                t->celdas[backF][backC].ficha = color;
                backF -= df;
                backC -= dc;
            }
        }
    }
}


int existeJugada(const Tablero *t, char color){
    /* Recorrer todo el tablero buscando al menos una jugada válida */
    for(int i=0; i < 8; i++){
        for(int j=0; j < 8; j++){
            /* Solo verificar casillas vacías */
            if(t->celdas[i][j].ficha == 'X'){
                if(esJugadaValida(t, i, j, color)){
                    return 1; // Encontramos al menos una jugada válida
                }
            }
        }
    }
    return 0; // No hay jugadas válidas
}


int contarFichas(const Tablero *t, char color){
    int contador = 0;
    /* Recorrer todo el tablero contando fichas del color especificado */
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(t->celdas[i][j].ficha == color){
                contador++;
            }
        }
    }
    return contador;
}


void guardarArchivoParaPython(const Tablero *t, char colorSiguiente) {
    FILE *salida = fopen("tablero_python.txt", "w");
    if (!salida) {
        printf("Error al crear tablero_python.txt\n");
        return;
    }

    /* Guardar el tablero: 8 líneas de 8 caracteres cada una */
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fprintf(salida, "%c", t->celdas[i][j].ficha);
        }
        fprintf(salida, "\n");
    }

    /* Línea 9: color del jugador que debe jugar a continuación */
    fprintf(salida, "%c\n", colorSiguiente);

    fclose(salida);
}


void estadoJuego(const Tablero *t, char colorActual, char colorSiguiente,const char jugadorActual[], const char jugadorSiguiente[]){
    imprimirTablero(t);

    /* Verificar si cada jugador tiene jugadas disponibles */
    int puedeSiguiente = existeJugada(t, colorSiguiente);
    int puedeActual = existeJugada(t, colorActual);
    
    /* Caso 1: La partida continúa - el siguiente jugador tiene jugadas */
    if (puedeSiguiente) {
        printf("\nJuega: %c", colorSiguiente);
        guardarArchivoParaPython(t, colorSiguiente);
        return;
    }

    /* Caso 2: El siguiente no puede jugar, pero el actual sí - se salta el turno */
    if (!puedeSiguiente && puedeActual) {
        printf("\n%c no tiene jugadas. Juega nuevamente %c ", colorSiguiente, colorActual);
        printf("Juega nuevamente: %c\n", colorActual);
        guardarArchivoParaPython(t, colorActual);
        return;
    }

    /* Caso 3: Ninguno puede jugar - fin del juego */
    int fichasActual = contarFichas(t, colorActual);
    int fichasSiguiente = contarFichas(t, colorSiguiente);

    printf("\nFin del juego.\n");
    printf("Fichas %s (%c): %d\n", jugadorActual, colorActual, fichasActual);
    printf("Fichas %s (%c): %d\n", jugadorSiguiente, colorSiguiente, fichasSiguiente);

    /* Determinar y anunciar el ganador */
    if (fichasActual > fichasSiguiente)
        printf("Ganó %s\n", jugadorActual);
    else if (fichasSiguiente > fichasActual)
        printf("Ganó %s\n", jugadorSiguiente);
    else
        printf("Empate\n");
}