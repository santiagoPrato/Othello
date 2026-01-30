/*
 * test.c - Suite de pruebas para las funciones del tablero de Othello
 * 
 * Este archivo contiene tests unitarios para validar el correcto funcionamiento
 * de las funciones implementadas en tablero.c, incluyendo inicialización,
 * validación de jugadas, aplicación de movimientos, y conteo de fichas.
 */

#include <assert.h>
#include <stdio.h>
#include "tablero.h"

/*
 * test_inicializarTablero - Prueba la inicialización del tablero
 * 
 * Verifica que:
 * 1. Todas las casillas estén vacías ('X') excepto las 4 centrales
 * 2. Las 4 fichas centrales estén en las posiciones correctas
 */
void test_inicializarTablero() {
    Tablero t;
    inicializarTablero(&t);

    /* Verificar que todas las casillas no centrales estén vacías */
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(!((i == 3 && j == 3) ||
                 (i == 3 && j == 4) ||
                 (i == 4 && j == 3) ||
                 (i == 4 && j == 4))) {
                assert(t.celdas[i][j].ficha == 'X');
            }
        }
    }

    /* Verificar las fichas centrales */
    assert(t.celdas[3][3].ficha == 'B');
    assert(t.celdas[3][4].ficha == 'N');
    assert(t.celdas[4][3].ficha == 'N');
    assert(t.celdas[4][4].ficha == 'B');

    printf("OK test_inicializarTablero\n");
}

/*
 * test_esJugadaValida - Prueba la validación de jugadas
 * 
 * Verifica que la función esJugadaValida identifique correctamente:
 * 1. Las 4 jugadas válidas para negras desde la posición inicial
 * 2. Jugadas inválidas en casillas ocupadas
 * 3. Jugadas inválidas en posiciones sin capturas posibles
 */
void test_esJugadaValida() {
    Tablero t;
    inicializarTablero(&t);

    /* Verificar las 4 jugadas válidas para negras desde la posición inicial */
    assert(esJugadaValida(&t, 2, 3, 'N') == 1); // C4
    assert(esJugadaValida(&t, 3, 2, 'N') == 1); // D3
    assert(esJugadaValida(&t, 4, 5, 'N') == 1); // F5
    assert(esJugadaValida(&t, 5, 4, 'N') == 1); // E6

    /* Verificar que una casilla ocupada no es válida */
    assert(esJugadaValida(&t, 3, 3, 'N') == 0);

    /* Verificar que una casilla sin capturas posibles no es válida */
    assert(esJugadaValida(&t, 0, 0, 'N') == 0);

    printf("OK test_esJugadaValida\n");
}

/*
 * test_aplicarJugada - Prueba la aplicación de jugadas
 * 
 * Verifica que al aplicar una jugada:
 * 1. Se coloque la ficha en la posición correcta
 * 2. Se volteen las fichas capturadas apropiadamente
 */
void test_aplicarJugada() {
    Tablero t;
    inicializarTablero(&t);

    /* Aplicar una jugada negra en C4 (2,3) */
    aplicarJugada(&t, 2, 3, 'N');

    /* Verificar que se colocó la ficha negra */
    assert(t.celdas[2][3].ficha == 'N');
    /* Verificar que se volteó la ficha blanca en D4 (3,3) */
    assert(t.celdas[3][3].ficha == 'N');

    printf("OK test_aplicarJugada\n");
}

/*
 * test_existeJugada - Prueba la detección de jugadas disponibles
 * 
 * Verifica que la función existeJugada detecte correctamente:
 * 1. Cuando sí hay jugadas disponibles para un color
 * 2. Cuando no hay jugadas disponibles
 */
void test_existeJugada() {
    Tablero t;
    inicializarTablero(&t);

    /* En el tablero inicial, ambos colores tienen jugadas disponibles */
    assert(existeJugada(&t, 'N') == 1);
    assert(existeJugada(&t, 'B') == 1);

    /* Crear un tablero completamente lleno de fichas negras */
    Tablero vacio;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            vacio.celdas[i][j].ficha = 'N';

    /* Las blancas no pueden jugar en un tablero sin casillas vacías */
    assert(existeJugada(&vacio, 'B') == 0);

    printf("OK test_existeJugada\n");
}

/*
 * test_contarFichas - Prueba el conteo de fichas
 * 
 * Verifica que la función contarFichas cuente correctamente:
 * 1. Las fichas en el tablero inicial (2 de cada color)
 * 2. Las fichas después de agregar más al tablero
 */
void test_contarFichas() {
    Tablero t;
    inicializarTablero(&t);

    /* En el tablero inicial hay 2 fichas de cada color */
    assert(contarFichas(&t, 'B') == 2);
    assert(contarFichas(&t, 'N') == 2);

    /* Agregar fichas adicionales */
    t.celdas[0][0].ficha = 'B';
    t.celdas[0][1].ficha = 'N';

    /* Verificar el nuevo conteo */
    assert(contarFichas(&t, 'B') == 3);
    assert(contarFichas(&t, 'N') == 3);

    printf("OK test_contarFichas\n");
}


/*
 * main - Ejecuta todos los tests
 * 
 * Ejecuta la suite completa de pruebas para validar las funciones del tablero.
 * 
 * Retorna:
 *   0 si todos los tests pasaron exitosamente
 */
int main() {
    test_inicializarTablero();
    test_esJugadaValida();
    test_aplicarJugada();
    test_existeJugada();
    test_contarFichas();
    return 0;
}