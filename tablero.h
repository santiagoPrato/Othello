#include <stdio.h>
#ifndef TABLERO_H
#define TABLERO_H

typedef struct {
    char ficha;        // 'B', 'N' o 'X'
} Celda;

typedef struct {
    Celda celdas[8][8];
} Tablero;

void inicializarTablero(Tablero *t);
void imprimirTablero(const Tablero *t);

void jugadaInvalida(const Tablero *t, const char jugada[], const char nombreJugador[]);
int esJugadaValida(const Tablero *t, int fila, int col, char color);
void aplicarJugada(Tablero *t, int fila, int col, char color);

int existeJugada(const Tablero *t, char color);
int contarFichas(const Tablero *t, char color);

void guardarArchivoParaPython(const Tablero *t, char colorSiguiente);

void estadoJuego(const Tablero *t, char colorActual, char colorSiguiente,const char jugadorActual[], const char jugadorSiguiente[]);

#endif