/*
    tablero.h - Definiciones y declaraciones para el tablero de Othello

    Define estructuras y prototipos de funciones para gestionar el tablero,
    inicialización, validación de jugadas, aplicación de movimientos y estado del juego.
*/

#include <stdio.h>
#ifndef TABLERO_H
#define TABLERO_H

/*
    Estructura Celda
    Representa una casilla individual del tablero.
    ficha: 'B' = blanca, 'N' = negra, 'X' = vacía
*/
typedef struct {
    char ficha;        // 'B', 'N' o 'X'
} Celda;

/*
    Estructura Tablero
    Representa el tablero completo de 8x8 celdas
*/
typedef struct {
    Celda celdas[8][8];
} Tablero;

/*
    Inicializa el tablero en su configuración inicial
    Coloca las cuatro fichas centrales y marca el resto como vacías
*/
void inicializarTablero(Tablero *t);

/*
    Imprime el tablero en consola de forma legible
*/
void imprimirTablero(const Tablero *t);

/*
    Maneja jugada inválida
    Muestra mensaje de error con la jugada y el jugador, y el estado del tablero
*/
void jugadaInvalida(const Tablero *t, const char jugada[], const char nombreJugador[]);

/*
    Verifica si una jugada es válida según reglas de Othello
    Devuelve 1 si es válida, 0 si no
*/
int esJugadaValida(const Tablero *t, int fila, int col, char color);

/*
    Aplica una jugada en el tablero
    Coloca la ficha y voltea las fichas capturadas
*/
void aplicarJugada(Tablero *t, int fila, int col, char color);

/*
    Verifica si existe al menos una jugada válida para un color
    Devuelve 1 si hay alguna, 0 si no
*/
int existeJugada(const Tablero *t, char color);


/*
    Cuenta cuántas fichas de un color hay en el tablero
*/
int contarFichas(const Tablero *t, char color);

/*
    Guarda el estado del tablero en un archivo para Python
    8 líneas de tablero + línea con color siguiente
*/
void guardarArchivoParaPython(const Tablero *t, char colorSiguiente);

/*
    Determina y muestra el estado actual del juego
    Indica si continúa, si se salta un turno, o si terminó y muestra ganador
*/
void estadoJuego(const Tablero *t, char colorActual, char colorSiguiente,const char jugadorActual[], const char jugadorSiguiente[]);

#endif