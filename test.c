#include <assert.h>
#include <stdio.h>
#include "tablero.h"

void test_inicializarTablero() {
    Celda t[8][8];
    inicializarTablero(8, t);

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(!((i == 3 && j == 3) ||
                 (i == 3 && j == 4) ||
                 (i == 4 && j == 3) ||
                 (i == 4 && j == 4))) {
                assert(t[i][j].ficha == 'X');
            }
        }
    }

    assert(t[3][3].ficha == 'B');
    assert(t[3][4].ficha == 'N');
    assert(t[4][3].ficha == 'N');
    assert(t[4][4].ficha == 'B');

    printf("OK test_inicializarTablero\n");
}

void test_esJugadaValida() {
    Celda t[8][8];
    inicializarTablero(8, t);

    assert(esJugadaValida(8, t, 2, 3, 'N') == 1); // C4
    assert(esJugadaValida(8, t, 3, 2, 'N') == 1); // D3
    assert(esJugadaValida(8, t, 4, 5, 'N') == 1); // F5
    assert(esJugadaValida(8, t, 5, 4, 'N') == 1); // E6

    assert(esJugadaValida(8, t, 3, 3, 'N') == 0);

    assert(esJugadaValida(8, t, 0, 0, 'N') == 0);

    printf("OK test_esJugadaValida\n");
}

void test_aplicarJugada() {
    Celda t[8][8];
    inicializarTablero(8, t);

    aplicarJugada(8, t, 2, 3, 'N');

    assert(t[3][3].ficha == 'N');

    printf("OK test_aplicarJugada\n");
}

void test_existeJugada() {
    Celda t[8][8];
    inicializarTablero(8, t);

    assert(existeJugada(8, t, 'N') == 1);
    assert(existeJugada(8, t, 'B') == 1);

    Celda vacio[8][8];
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            vacio[i][j].ficha = 'N';

    assert(existeJugada(8, vacio, 'B') == 0);

    printf("OK test_existeJugada\n");
}

void test_contarFichas() {
    Celda t[8][8];
    inicializarTablero(8, t);

    assert(contarFichas(8, t, 'B') == 2);
    assert(contarFichas(8, t, 'N') == 2);

    t[0][0].ficha = 'B';
    t[0][1].ficha = 'N';

    assert(contarFichas(8, t, 'B') == 3);
    assert(contarFichas(8, t, 'N') == 3);

    printf("OK test_contarFichas\n");
}


int main() {
    test_inicializarTablero();
    test_esJugadaValida();
    test_aplicarJugada();
    test_existeJugada();
    test_contarFichas();
    return 0;
}