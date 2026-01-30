/*
 * tablero.h - Definiciones para el tablero de Othello
 */

#include <stdio.h>
#ifndef TABLERO_H
#define TABLERO_H

/*
 * Estructura Celda - Casilla individual del tablero
 * ficha: 'B' (blanca), 'N' (negra), 'X' (vacía)
 */
typedef struct {
    char ficha;
} Celda;

/*
 * Estructura Tablero - Tablero 8x8 del juego
 */
typedef struct {
    Celda celdas[8][8];
} Tablero;

/* Inicializa el tablero en su configuración inicial */
void inicializarTablero(Tablero *t);

/* Imprime el tablero en la salida estándar */
void imprimirTablero(const Tablero *t);

/* Maneja el caso de una jugada inválida */
void jugadaInvalida(const Tablero *t, const char jugada[], const char nombreJugador[]);

/* Verifica si una jugada es válida */
int esJugadaValida(const Tablero *t, int fila, int col, char color);

/* Aplica una jugada en el tablero */
void aplicarJugada(Tablero *t, int fila, int col, char color);

/* Verifica si existe al menos una jugada válida */
int existeJugada(const Tablero *t, char color);

/* Cuenta cuántas fichas de un color hay en el tablero */
int contarFichas(const Tablero *t, char color);

/* Guarda el estado del tablero en un archivo para Python */
void guardarArchivoParaPython(const Tablero *t, char colorSiguiente);

/* Determina y muestra el estado actual del juego */
void estadoJuego(const Tablero *t, char colorActual, char colorSiguiente,const char jugadorActual[], const char jugadorSiguiente[]);

#endif