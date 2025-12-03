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

int esJugadaValida(int size, Celda tablero[size][size], int fila, int col, char color){

}