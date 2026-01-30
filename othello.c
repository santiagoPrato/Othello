#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablero.h"

// Lee los datos de un jugador desde el archivo y verifica su formato.
// Devuelve 1 si se obtuvo correctamente, 0 en caso de error.
int leerJugador(FILE *archivo, char **nombre, char *color) {
    char buffer[256];
    char nombreTmp[256];
    char colorTmp;

    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    if (sscanf(buffer, "%255[^,],%c", nombreTmp, &colorTmp) != 2)
        return 0;

    if (colorTmp != 'B' && colorTmp != 'N')
        return 0;

    *nombre = malloc(strlen(nombreTmp) + 1);
    if (!*nombre)
        return 0;

    strcpy(*nombre, nombreTmp);
    *color = colorTmp;

    return 1;
}

// Determina el color del jugador que comenzó el juego a partir de la entrada.
// Devuelve 'B' o 'N' en caso de éxito, o 0 si hubo un problema.
char empezo(FILE* archivo){
    char buffer[8];

    if (!fgets(buffer, sizeof(buffer), archivo))
        return 0;

    if (buffer[0] != 'B' && buffer[0] != 'N')
        return 0;

    return buffer[0];
}

// Verifica que los datos de los jugadores sean válidos (nombres no vacíos, colores válidos).
// Devuelve 1 si todo es válido, 0 en caso de error.
int verificarDatos(char *nombre1, char *nombre2, char color1, char color2){
    if (!nombre1 || !nombre2) {
        printf("No se pudieron leer los nombres de los jugadores\n");
        return 0;
    }
    if(nombre1[0] == '\0'){
        printf("Nombre del primer jugador vacio\n");
        return 0;
    }
    if(nombre2[0] == '\0'){
        printf("Nombre del segundo jugador vacio\n");
        return 0;   
    }
    if(color1 != 'B' && color1 != 'N'){
        printf("Color del primer jugador invalido\n");
        return 0;
    }
    if(color2 != 'B' && color2 != 'N'){
        printf("Color del segundo jugador invalido\n");
        return 0;
    }
    if(color1 == color2){
        printf("Ambos jugadores tienen el mismo color\n");
        return 0;
    }
    return 1;
}


int procesarJugadasDesdeArchivo(FILE *puntero_archivo, Tablero *t, char primero, 
                    char segundo, char color1, char *nombre1, char *nombre2, int *turnoFinal){
    int turno = 0;
    char buffer[4];

    while(fscanf(puntero_archivo, "%s", buffer) != EOF){
        int fila, columna;

        char colorActual = (turno % 2 == 0) ? primero : segundo;
        char *nombreActual = (colorActual == color1) ? nombre1 : nombre2;

        // Conversión de jugada
        if(!convertirJugadas(buffer, &fila, &columna)){
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        // Validación
        if(!esJugadaValida(t, fila, columna, colorActual)){
            jugadaInvalida(t, buffer, nombreActual);
            return 0;
        }

        // Aplicación
        aplicarJugada(t, fila, columna, colorActual);

        *turnoFinal = turno;
        turno++;
    }

    return 1;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        // Verificar si se proporcionó el archivo como argumento
        printf("Debe indicar el archivo como argumento\n");
        return 1;
    }

    // Abrir el archivo en modo lectura
    FILE *puntero_archivo = fopen(argv[1], "r");
    if(puntero_archivo == NULL){
        printf("No es posible abrir este archivo");
        return 1;
    }
    
    // Leer los nombres y colores de los jugadores
    char *nombre1 = NULL;
    char *nombre2 = NULL;
    char color1 = '\0';
    char color2 = '\0';
    if (!leerJugador(puntero_archivo, &nombre1, &color1) || !leerJugador(puntero_archivo, &nombre2, &color2)) {
        printf("Error al leer los jugadores\n");
        fclose(puntero_archivo);
        return 1;
    }

    // Verificar que los datos leídos sean válidos
    if(!verificarDatos(nombre1, nombre2, color1, color2)){
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 1;
    }

    // Determinar el jugador que comenzó el juego
    char primero = empezo(puntero_archivo);
    if (primero == '/0') {
        printf("Error al leer el color que empezo\n");
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 1;
    }
    char segundo = (primero == color1) ? color2: color1;



    // Inicializar tablero
    Tablero tablero;
    inicializarTablero(&tablero);

    // Leer la 4 línea a EOF
    int turnoFinal;
    if(!procesarJugadasDesdeArchivo(puntero_archivo, &tablero, primero, segundo, color1, nombre1, nombre2, &turnoFinal)){
        fclose(puntero_archivo);
        free(nombre1);
        free(nombre2);
        return 0;
    }

    // Determinar el estado final del juego
    char colorActual = (turnoFinal % 2 == 0) ? primero : segundo;
    char colorSiguiente = (colorActual == primero) ? segundo : primero;
    char *nombreActual = (colorActual == color1) ? nombre1 : nombre2;
    char *nombreSiguiente = (colorSiguiente == color1) ? nombre1 : nombre2;

    estadoJuego(&tablero, colorActual, colorSiguiente, nombreActual, nombreSiguiente);

    fclose(puntero_archivo);
    free(nombre1);
    free(nombre2);
    return 0;
}
