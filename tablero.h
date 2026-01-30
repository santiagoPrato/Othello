/*
 * tablero.h - Definiciones y declaraciones para el tablero de Othello
 * 
 * Este archivo de cabecera define las estructuras de datos y prototipos de funciones
 * necesarias para gestionar el tablero del juego Othello (Reversi).
 * Incluye funciones para inicialización, validación de jugadas, aplicación de movimientos,
 * y determinación del estado del juego.
 */

#include <stdio.h>
#ifndef TABLERO_H
#define TABLERO_H

/*
 * Estructura Celda
 * Representa una casilla individual del tablero.
 * 
 * Campos:
 *   ficha: Carácter que representa el contenido de la casilla
 *          'B' = ficha blanca
 *          'N' = ficha negra  
 *          'X' = casilla vacía
 */
typedef struct {
    char ficha;        // 'B', 'N' o 'X'
} Celda;

/*
 * Estructura Tablero
 * Representa el tablero completo del juego (8x8).
 * 
 * Campos:
 *   celdas: Matriz 8x8 de celdas que conforman el tablero
 */
typedef struct {
    Celda celdas[8][8];
} Tablero;

/*
 * inicializarTablero - Inicializa el tablero en su configuración inicial de Othello
 * 
 * Coloca las cuatro fichas centrales (2 blancas y 2 negras) en sus posiciones
 * iniciales y marca el resto de las casillas como vacías.
 * 
 * Parámetros:
 *   t: Puntero al tablero a inicializar
 */
void inicializarTablero(Tablero *t);

/*
 * imprimirTablero - Imprime el tablero en la salida estándar
 * 
 * Muestra el estado actual del tablero en un formato legible,
 * con cada fila en una línea separada.
 * 
 * Parámetros:
 *   t: Puntero constante al tablero a imprimir
 */
void imprimirTablero(const Tablero *t);

/*
 * jugadaInvalida - Maneja el caso de una jugada inválida
 * 
 * Imprime un mensaje de error indicando la jugada inválida y el jugador
 * que la cometió, seguido del estado actual del tablero.
 * 
 * Parámetros:
 *   t: Puntero constante al tablero
 *   jugada: String con la jugada en notación algebraica (ej: "D3")
 *   nombreJugador: String con el nombre del jugador que cometió el error
 */
void jugadaInvalida(const Tablero *t, const char jugada[], const char nombreJugador[]);

/*
 * esJugadaValida - Verifica si una jugada es válida según las reglas de Othello
 * 
 * Una jugada es válida si la casilla está vacía y en al menos una dirección
 * hay fichas del oponente seguidas por una ficha del color actual.
 * 
 * Parámetros:
 *   t: Puntero constante al tablero
 *   fila: Índice de fila (0-7)
 *   col: Índice de columna (0-7)
 *   color: Color del jugador ('B' o 'N')
 * 
 * Retorna:
 *   1 si la jugada es válida, 0 en caso contrario
 */
int esJugadaValida(const Tablero *t, int fila, int col, char color);

/*
 * aplicarJugada - Aplica una jugada en el tablero
 * 
 * Coloca la ficha del color indicado en la posición especificada y voltea
 * todas las fichas del oponente que quedan capturadas en todas las direcciones.
 * 
 * Parámetros:
 *   t: Puntero al tablero (se modifica)
 *   fila: Índice de fila donde colocar la ficha (0-7)
 *   col: Índice de columna donde colocar la ficha (0-7)
 *   color: Color del jugador que realiza la jugada ('B' o 'N')
 */
void aplicarJugada(Tablero *t, int fila, int col, char color);

/*
 * existeJugada - Verifica si existe al menos una jugada válida para un color
 * 
 * Recorre todo el tablero buscando si hay al menos una casilla donde
 * el jugador del color indicado pueda realizar una jugada válida.
 * 
 * Parámetros:
 *   t: Puntero constante al tablero
 *   color: Color del jugador a verificar ('B' o 'N')
 * 
 * Retorna:
 *   1 si existe al menos una jugada válida, 0 en caso contrario
 */
int existeJugada(const Tablero *t, char color);

/*
 * contarFichas - Cuenta cuántas fichas de un color hay en el tablero
 * 
 * Parámetros:
 *   t: Puntero constante al tablero
 *   color: Color de las fichas a contar ('B' o 'N')
 * 
 * Retorna:
 *   Número total de fichas del color especificado
 */
int contarFichas(const Tablero *t, char color);

/*
 * guardarArchivoParaPython - Guarda el estado del tablero en un archivo para Python
 * 
 * Crea un archivo "tablero_python.txt" con el estado actual del tablero
 * en un formato que puede ser leído por el script de Python.
 * El archivo contiene 8 líneas de 8 caracteres cada una representando el tablero,
 * más una línea adicional con el color que debe jugar a continuación.
 * 
 * Parámetros:
 *   t: Puntero constante al tablero
 *   colorSiguiente: Color del jugador que debe jugar a continuación ('B' o 'N')
 */
void guardarArchivoParaPython(const Tablero *t, char colorSiguiente);

/*
 * estadoJuego - Determina y muestra el estado actual del juego
 * 
 * Analiza el tablero para determinar si el juego continúa, si hay que saltar
 * un turno, o si el juego ha terminado. En caso de finalización, determina
 * y muestra el ganador. También guarda el estado en un archivo para Python.
 * 
 * Parámetros:
 *   t: Puntero constante al tablero
 *   colorActual: Color del jugador que acaba de jugar ('B' o 'N')
 *   colorSiguiente: Color del jugador que debería jugar a continuación ('B' o 'N')
 *   jugadorActual: Nombre del jugador que acaba de jugar
 *   jugadorSiguiente: Nombre del jugador que debería jugar a continuación
 */
void estadoJuego(const Tablero *t, char colorActual, char colorSiguiente,const char jugadorActual[], const char jugadorSiguiente[]);

#endif