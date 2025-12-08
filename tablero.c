#include <stdio.h>
#include "tablero.h"


void incializarTablero(int SIZE, Celda tablero[SIZE][SIZE]){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            tablero[i][j].ficha = 'X';
        }
    }

    // Fichas iniciales de Othello
    tablero[(SIZE/2)-1][(SIZE/2)-1].ficha = 'N';
    tablero[(SIZE/2)-1][SIZE/2].ficha = 'B';
    tablero[SIZE/2][(SIZE/2)-1].ficha = 'B';
    tablero[SIZE/2][SIZE/2].ficha = 'N';
}


void imprimirTablero(int size, Celda tablero[size][size]){
    for(int i=0; i < size; i++){
        for(int j=0; j < size; j++){
            printf("%c ", tablero[i][j].ficha);
        }
        printf("\n");
    }
}


void jugadaInvalida(int size, Celda tablero[size][size], char jugada[], char colorActual){
    printf("Jugada invalida %s cometida por %s ", jugada, colorActual);

    printf("Tablero hasta la jugada inválida:\n");
    imprimirTablero(size, tablero);
}


int esJugadaValida(int size, Celda tablero[size][size], int fila, int col, char color){
    if(tablero[fila][col].ficha != 'X'){
        return 0;
    }

    char rival = (color == 'B') ? 'N' : 'B';

    int dirFila[] = {-1,-1,-1, 0,0, +1,+1,+1};
    int dirCol[] = {-1, 0,+1,-1,+1,-1, 0,+1};

    for(int d = 0; d < 8; d++){
        int f = fila + dirFila[d];
        int c = col + dirCol[d];

        if(f < 0 || f >= size || c < 0 || c >= size) continue;
        if(tablero[f][c].ficha != rival) continue;

        f += dirFila[d];
        c += dirCol[d];

        while(f >= 0 && f < size && c >= 0 && c < size){
            if(tablero[f][c].ficha == 'X'){
                break; // no sirve
            }
            if(tablero[f][c].ficha == color){
                return 1;
            }
            f += dirFila[d];
            c += dirCol[d];
        }
    }

    return 0;
}


void aplicarJugada(int size, Celda tablero[size][size], int fila, int col, char color) {
    char rival = (color == 'B') ? 'N' : 'B';

    // Colocamos la ficha
    tablero[fila][col].ficha = color;

    int dirFila[] = {-1,-1,-1, 0,0, +1,+1,+1};
    int dirCol [] = {-1, 0,+1,-1,+1,-1, 0,+1};

    for(int d = 0; d < 8; d++){
        int df = dirFila[d];
        int dc = dirCol[d];

        int f = fila + df;
        int c = col + dc;

        if(f < 0 || f >= size || c < 0 || c >= size)
            continue;

        if(tablero[f][c].ficha != rival)
            continue;

        f += df;
        c += dc;

        while(f >= 0 && f < size && c >= 0 && c < size && tablero[f][c].ficha == rival){
            f += df;
            c += dc;
        }

        // Si salimos del tablero → no captura
        if(f < 0 || f >= size || c < 0 || c >= size)
            continue;

        // Si encontramos vacío → no captura
        if(tablero[f][c].ficha == 'X')
            continue;

        // Si encontramos ficha propia → captura
        if(tablero[f][c].ficha == color){
            // Voltear hacia atrás hasta llegar a la casilla original
            int backF = f - df;
            int backC = c - dc;

            while(backF != fila || backC != col){
                tablero[backF][backC].ficha = color;
                backF -= df;
                backC -= dc;
            }
        }
    }
}


int existeJugada(int size, Celda tablero[size][size], char color){
    for(int i=0; i < size; i++){
        for(int j=0; j < size; j++){
            // Probar casillas vacias
            if(tablero[i][j].ficha == 'X'){
                if(esJugadaValida(size, tablero, i, j, color)){
                    return 1;
                }
            }
        }
    }
    return 0;
}


int contarFichas(int size, Celda tablero[size][size], char color){
    int contador = 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(tablero[i][j].ficha == color){
                contador++;
            }
        }
    }
    return contador;
}


void estadoJuego(int size, Celda tablero[size][size], char colorActual, char colorSiguiente, char nombreActual[], char nombreSiguiente[]) {
    imprimirTablero(size, tablero);

    int puedeSiguiente = existeJugada(size, tablero, colorSiguiente);
    int puedeActual    = existeJugada(size, tablero, colorActual);

    // Caso 1: La partida continua
    if (puedeSiguiente) {
        printf("\nJuega: %c", colorSiguiente);
        return;
    }

    // Caso 2: El siguiente no puede jugar, pero el actual si → se saltea turno
    if (!puedeSiguiente && puedeActual) {
        printf("\n%s no tiene jugadas. Juega nuevamente %s ", colorSiguiente, colorActual);
        return;
    }

    // Caso 3: Ninguno puede jugar → fin del juego
    int fichasActual    = contarFichas(size, tablero, colorActual);
    int fichasSiguiente = contarFichas(size, tablero, colorSiguiente);

    printf("\nFin del juego.\n");
    printf("Fichas %s (%c): %d\n", nombreActual, colorActual, fichasActual);
    printf("Fichas %s (%c): %d\n", nombreSiguiente, colorSiguiente, fichasSiguiente);

    if (fichasActual > fichasSiguiente)
        printf("Ganó %s\n", nombreActual);
    else if (fichasSiguiente > fichasActual)
        printf("Ganó %s\n", nombreSiguiente);
    else
        printf("Empate\n");
}