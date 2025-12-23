#include <assert.h>
#include <stdio.h>
#include "tablero.h"

void test_inicializarTablero() {
    Tablero t;
    inicializarTablero(&t);

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

    assert(t.celdas[3][3].ficha == 'B');
    assert(t.celdas[3][4].ficha == 'N');
    assert(t.celdas[4][3].ficha == 'N');
    assert(t.celdas[4][4].ficha == 'B');

    printf("OK test_inicializarTablero\n");
}

void test_esJugadaValida() {
    Tablero t;
    inicializarTablero(&t);

    assert(esJugadaValida(&t, 2, 3, 'N') == 1); // C4
    assert(esJugadaValida(&t, 3, 2, 'N') == 1); // D3
    assert(esJugadaValida(&t, 4, 5, 'N') == 1); // F5
    assert(esJugadaValida(&t, 5, 4, 'N') == 1); // E6

    assert(esJugadaValida(&t, 3, 3, 'N') == 0);

    assert(esJugadaValida(&t, 0, 0, 'N') == 0);

    printf("OK test_esJugadaValida\n");
}

void test_aplicarJugada() {
    Tablero t;
    inicializarTablero(&t);

    aplicarJugada(&t, 2, 3, 'N');

    assert(t.celdas[3][3].ficha == 'N');

    printf("OK test_aplicarJugada\n");
}

void test_existeJugada() {
    Tablero t;
    inicializarTablero(&t);

    assert(existeJugada(&t, 'N') == 1);
    assert(existeJugada(&t, 'B') == 1);

    Tablero vacio;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            vacio.celdas[i][j].ficha = 'N';

    assert(existeJugada(&vacio, 'B') == 0);

    printf("OK test_existeJugada\n");
}

void test_contarFichas() {
    Tablero t;
    inicializarTablero(&t);

    assert(contarFichas(&t, 'B') == 2);
    assert(contarFichas(&t, 'N') == 2);

    t.celdas[0][0].ficha = 'B';
    t.celdas[0][1].ficha = 'N';

    assert(contarFichas(&t, 'B') == 3);
    assert(contarFichas(&t, 'N') == 3);

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