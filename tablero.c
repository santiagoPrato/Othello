#include <stdio.h>
#include "tablero.h"


void inicializarTablero(Tablero *t){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            t->celdas[i][j].ficha = 'X';
        }
    }

    /* Fichas iniciales */
    t->celdas[3][3].ficha = 'B';
    t->celdas[3][4].ficha = 'N';
    t->celdas[4][3].ficha = 'N';
    t->celdas[4][4].ficha = 'B';
}


void imprimirTablero(const Tablero *t){
    for(int i=0; i < 8; i++){
        for(int j=0; j < 8; j++){
            printf("%c ", t->celdas[i][j].ficha);
        }
        printf("\n");
    }
}


void jugadaInvalida(const Tablero *t, const char jugada[], const char nombreJugador[]){
    printf("Jugada invalida %s cometida por %s\n", jugada, nombreJugador);
    imprimirTablero(t);
}


int esJugadaValida(const Tablero *t, int fila, int col, char color){
    if(t->celdas[fila][col].ficha != 'X'){
        return 0;
    }

    char rival = (color == 'B') ? 'N' : 'B';

    int dirFila[] = {-1,-1,-1, 0,0, +1,+1,+1};
    int dirCol[] = {-1, 0,+1,-1,+1,-1, 0,+1};

    for(int d = 0; d < 8; d++){
        int f = fila + dirFila[d];
        int c = col + dirCol[d];

        if(f < 0 || f >= 8 || c < 0 || c >= 8) continue;
        if(t->celdas[f][c].ficha != rival) continue;

        f += dirFila[d];
        c += dirCol[d];

        while(f >= 0 && f < 8 && c >= 0 && c < 8){
            if(t->celdas[f][c].ficha == 'X'){
                break; // no sirve
            }
            if(t->celdas[f][c].ficha == color){
                return 1;
            }
            f += dirFila[d];
            c += dirCol[d];
        }
    }

    return 0;
}


void aplicarJugada( Tablero *t, int fila, int col, char color) {
    char rival = (color == 'B') ? 'N' : 'B';

    // Colocamos la ficha
    t->celdas[fila][col].ficha = color;

    int dirFila[] = {-1,-1,-1, 0,0, +1,+1,+1};
    int dirCol [] = {-1, 0,+1,-1,+1,-1, 0,+1};

    for(int d = 0; d < 8; d++){
        int df = dirFila[d];
        int dc = dirCol[d];

        int f = fila + df;
        int c = col + dc;

        if(f < 0 || f >= 8 || c < 0 || c >= 8)
            continue;

        if(t->celdas[f][c].ficha != rival)
            continue;

        f += df;
        c += dc;

        while(f >= 0 && f < 8 && c >= 0 && c < 8 && t->celdas[f][c].ficha == rival){
            f += df;
            c += dc;
        }


        if(f < 0 || f >= 8 || c < 0 || c >= 8)
            continue;

        if(t->celdas[f][c].ficha == 'X')
            continue;

        // Si encontramos ficha propia, captura
        if(t->celdas[f][c].ficha == color){
            // Voltear hacia atrás hasta llegar a la casilla original
            int backF = f - df;
            int backC = c - dc;

            while(backF != fila || backC != col){
                t->celdas[backF][backC].ficha = color;
                backF -= df;
                backC -= dc;
            }
        }
    }
}


int existeJugada(const Tablero *t, char color){
    for(int i=0; i < 8; i++){
        for(int j=0; j < 8; j++){
            // Probar casillas vacias
            if(t->celdas[i][j].ficha == 'X'){
                if(esJugadaValida(t, i, j, color)){
                    return 1;
                }
            }
        }
    }
    return 0;
}


int contarFichas(const Tablero *t, char color){
    int contador = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(t->celdas[i][j].ficha == color){
                contador++;
            }
        }
    }
    return contador;
}


void guardarArchivoParaPython(const Tablero *t, char colorSiguiente) {
    FILE *salida = fopen("tablero_python.txt", "w");
    if (!salida) {
        printf("Error al crear tablero_python.txt\n");
        return;
    }

    // Guardar tablero: 8 lineas de 8 caracteres
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fprintf(salida, "%c", t->celdas[i][j].ficha);
        }
        fprintf(salida, "\n");
    }

    // 9na linea: color que debe jugar
    fprintf(salida, "%c\n", colorSiguiente);

    fclose(salida);
}


void estadoJuego(const Tablero *t, char colorActual, char colorSiguiente,const char jugadorActual[], const char jugadorSiguiente[]){
    imprimirTablero(t);

    int puedeSiguiente = existeJugada(t, colorSiguiente);
    int puedeActual = existeJugada(t, colorActual);
    
    // Caso 1: La partida continua
    if (puedeSiguiente) {
        printf("\nJuega: %c", colorSiguiente);
        guardarArchivoParaPython(t, colorSiguiente);
        return;
    }

    // Caso 2: El siguiente no puede jugar, pero el actual si, se saltea turno
    if (!puedeSiguiente && puedeActual) {
        printf("\n%c no tiene jugadas. Juega nuevamente %c ", colorSiguiente, colorActual);
        guardarArchivoParaPython(t, colorActual);
        return;
    }

    // Caso 3: Ninguno puede jugar, fin del juego
    int fichasActual = contarFichas(t, colorActual);
    int fichasSiguiente = contarFichas(t, colorSiguiente);

    printf("\nFin del juego.\n");
    printf("Fichas %s (%c): %d\n", jugadorActual, colorActual, fichasActual);
    printf("Fichas %s (%c): %d\n", jugadorSiguiente, colorSiguiente, fichasSiguiente);

    if (fichasActual > fichasSiguiente)
        printf("Ganó %s\n", jugadorActual);
    else if (fichasSiguiente > fichasActual)
        printf("Ganó %s\n", jugadorSiguiente);
    else
        printf("Empate\n");
}