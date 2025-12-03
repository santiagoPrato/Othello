#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablero.h"

void jugadoresAsignacion(FILE* archivo, char* nombre1, char* nombre2, char *color1, char *color2){
    char buffer[256];

    // Leer primera línea
    if(fgets(buffer, sizeof(buffer), archivo) != NULL)
        sscanf(buffer, "%[^,],%c", nombre1, color1);

    // Leer segunda línea
    if(fgets(buffer, sizeof(buffer), archivo) != NULL)
        sscanf(buffer, "%[^,],%c", nombre2, color2);
}

char empezo(FILE* archivo){
    char linea[256];
    char caracter;

    // Leer tercera línea
    if(fgets(linea, sizeof(linea), archivo) != NULL)
        caracter = linea[0];

    return caracter;
} 

int convertirJugadas(char jugada[], int *fila, int *columna){
    if(strlen(jugada) != 2){
        return 0;
    }

    char columnaChar = jugada[0];
    char filaChar = jugada[1];

    if(columnaChar < 'A' || columnaChar > 'H' || filaChar < '0' || filaChar > '8'){
        return 0;
    }

    *columna = columnaChar - 'A';
    *fila = filaChar - '0';

    return 1;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Debe indicar el archivo como argumento\n");
        return 1;
    }

    FILE *puntero_archivo = fopen(argv[1], "r");
    if(puntero_archivo == NULL){
        printf("No es posible abrir este archivo");
        return 1;
    }
    
    // Asignar valores a las variables
    char nombre1[100] = "", nombre2[100] = "";
    char color1 = '\0', color2 = '\0';
    jugadoresAsignacion(puntero_archivo, nombre1, nombre2, &color1, &color2);

    char primero = empezo(puntero_archivo);
    char segundo = (primero == color1) ? color2: color1;

    // Verificar que ningun dato este vacio
    if (nombre1[0] == '\0' || nombre2[0] == '\0' || color1 == '\0' || color2 == '\0') {
        printf("Falta algún dato\n");
        fclose(puntero_archivo);
        return 1;
    }

    // Inicializar tablero
    const int size = 8;
    Celda tablero[size][size]; 

    inicializarTablero(size, tablero);

    // Leer la 4 línea a EOF
    int turno = 0;
    char buffer[5];
    while(fscanf(puntero_archivo, "%s", buffer) != EOF){
        int fila, columna;
        char colorActual = (turno % 2 == 0) ? primero : segundo;
        
        if(!convertirJugadas(buffer, fila, columna)){
            jugadaInvalida(size, tablero, colorActual);
            fclose(puntero_archivo);
            return 0;
        }

        turno++;
    }
    

    fclose(puntero_archivo);
    return 0;
}