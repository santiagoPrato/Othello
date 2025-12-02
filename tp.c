#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nombreJugadores(FILE* archivo, char* jugador1, char* jugador2){
    char buffer[256];

    // Leer primera línea
    if(fgets(buffer, sizeof(buffer), archivo) != NULL)
        sscanf(buffer, "%[^,]", jugador1);

    // Leer segunda línea
    if(fgets(buffer, sizeof(buffer), archivo) != NULL)
        sscanf(buffer, "%[^,]", jugador2);
}

char empezo(FILE* archivo){
    char linea[256];
    char caracter;

    // Leer tercera línea
    if(fgets(linea, sizeof(linea), archivo) != NULL)
        caracter = linea[0];

    return caracter;
} 

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Debe indicar el archivo como argumento\n");
        return 1;
    }
    
    char* nombre_archivo = argv[1];
    FILE *puntero_archivo;
    puntero_archivo = fopen(nombre_archivo, "r");
    if(puntero_archivo == NULL){
        printf("No es posible abrir este archivo");
        return 1;
    }

    char jugador1[100], jugador2[100];
    nombreJugadores(puntero_archivo, jugador1, jugador2);

    char primero = empezo(puntero_archivo);

    fclose(puntero_archivo);
    return 0;
}