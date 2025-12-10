#include <stdio.h>
#ifndef TABLERO_H
#define TABLERO_H

typedef struct {
    char ficha;        // 'B', 'N' o 'X'
} Celda;

void inicializarTablero(int size, Celda tablero[size][size]);
void jugadaInvalida(int size, Celda tablero[size][size], char jugada[], char nombreJugador[]);
int esJugadaValida(int size, Celda tablero[size][size], int fila, int col, char color);
void aplicarJugada(int size, Celda tablero[size][size], int fila, int col, char color);
int existeJugada(int size, Celda tablero[size][size], char color);
int contarFichas(int size, Celda tablero[size][size], char color);
void guardarArchivoParaPython(int size, Celda tablero[size][size], char colorSiguiente);
void estadoJuego(int size, Celda tablero[size][size],char colorActual, char colorSiguiente, char jugadorActual[], char jugadorSiguiente[]);
void imprimirTablero(int size, Celda tablero[size][size]);

#endif