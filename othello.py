"""Módulo principal del juego Othello (Reversi)"""

def leerTablero(lineas):
    """Lee y construye el tablero desde las líneas de un archivo"""
    tablero = []
    for i in range(8):
        fila = list(lineas[i])
        if len(fila) != 8:
            raise ValueError(f"La fila {i+1} del tablero no tiene 8 columnas")
        tablero.append(fila)

    return tablero

def leerColorInicial(color):
    """Valida el color inicial ('B' o 'N')"""
    if color not in ("B", "N"):
        raise ValueError("El color inicial debe ser 'B' o 'N'")

    return color


def imprimirTablero(tablero):
    """Imprime el tablero en la consola"""
    for i in range(8):
        for j in range(8):
            print(tablero[i][j], end=" ")

        print()


def convertirJugada(jugada):
    """Convierte jugada algebraica (ej: "D3") a coordenadas de matriz"""
    if len(jugada) != 2:
        return None

    col = jugada[0].upper()
    fil = jugada[1]

    if col < 'A' or col > 'H':
        return None
    if fil < '1' or fil > '8':
        return None

    columna = ord(col) - ord('A')
    fila = int(fil) - 1

    return fila, columna


def jugadaValida(tablero, fila, col, color):
    """Verifica si una jugada es válida según las reglas de Othello"""
    if tablero[fila][col] != 'X':
        return False

    oponente = 'B' if color == 'N' else 'N'

    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]

    valido = False

    for df, dc in dirs:
        f = fila + df
        c = col + dc
        encontrado_oponente = False

        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            encontrado_oponente = True
            f += df
            c += dc

        if encontrado_oponente and 0<= f < 8 and 0 <= c < 8 and tablero[f][c] == color:
            valido = True
            break

    return valido


def aplicarJugada(tablero, fila, col, color):
    """Aplica una jugada colocando la ficha y volteando las capturadas"""
    tablero[fila][col] = color
    oponente = 'B' if color == 'N' else 'N'

    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]

    for df, dc in dirs:
        f = fila + df
        c = col + dc
        dar_vuelta = []

        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            dar_vuelta.append((f,c))
            f += df
            c += dc

        if dar_vuelta and 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == color:
            for (fx, cx) in dar_vuelta:
                tablero[fx][cx] = color


def turnoJugador(tablero, color):
    """Maneja el turno de un jugador humano"""
    while True:
        jugada = input("Ingrese su jugada (ej: D3): ").strip()

        coords = convertirJugada(jugada)
        if coords is None:
            print("Formato inválido, intente nuevamente")
            continue

        fila, col = coords
        if jugadaValida(tablero, fila, col, color):
            aplicarJugada(tablero, fila, col, color)
            
            print("Tablero después de su jugada:")
            imprimirTablero(tablero)
            return

        print("Jugada inválida, intente otra")


import random


def jugadasPosibles(tablero, color):
    """Encuentra todas las jugadas válidas posibles para un color"""
    posibles = []
    for f in range(8):
        for c in range(8):
            if jugadaValida(tablero, f, c, color):
                posibles.append((f, c))
    return posibles


def nivel0(tablero, color):
    """IA de nivel 0: elige una jugada aleatoria"""
    posibles = jugadasPosibles(tablero, color)
    if not posibles:
        return
    
    jugada = random.choice(posibles)
    aplicarJugada(tablero, jugada[0], jugada[1], color)

    colLetra = chr(jugada[1] + ord('A'))
    filNum = jugada[0] + 1
    print(f"La computadora eligió: {colLetra}{filNum}")
    imprimirTablero(tablero)
        

def contarFichasVueltas(tablero, fila, col, color):
    """Cuenta cuántas fichas se voltearían con una jugada"""
    if tablero[fila][col] != 'X':
        return 0

    oponente = 'B' if color == 'N' else 'N'

    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]
    total = 0

    for df, dc in dirs:
        f = fila + df
        c = col + dc
        contador = 0

        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            contador += 1
            f += df
            c += dc

        if 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == color:
            total += contador

    return total


def elegirNivel1(tablero, color):
    """Elige la jugada que voltea más fichas"""
    posibles = jugadasPosibles(tablero, color)

    if not posibles:
        return None
    
    mejor_jugada = None
    max_fichas = -1

    for fila, col in posibles:
        vueltas = contarFichasVueltas(tablero, fila, col, color)
        if max_fichas < vueltas:
            max_fichas = vueltas
            mejor_jugada = (fila, col)
    
    return mejor_jugada


def nivel1(tablero, color):
    """IA de nivel 1: elige la jugada que voltea más fichas"""
    mejor_jugada = elegirNivel1(tablero, color) 
    
    aplicarJugada(tablero, mejor_jugada[0], mejor_jugada[1], color)

    colLetra = chr(mejor_jugada[1] + ord('A'))
    filNum = mejor_jugada[0] + 1
    print(f"La computadora eligió: {colLetra}{filNum}")
    imprimirTablero(tablero)


def contadorFichas(tablero, color_jugador, color_pc):
    """Cuenta las fichas de cada jugador en el tablero"""
    cant_fichas_j = 0 
    cant_fichas_pc = 0
    for i in range(8):
        for j in range(8):
            if tablero[i][j] == color_jugador:
                cant_fichas_j += 1
            elif tablero[i][j] == color_pc:
                cant_fichas_pc += 1

    return  cant_fichas_j, cant_fichas_pc


def mostrarResultadoFinal(tablero, color_jugador, color_pc):
    """Muestra el resultado final del juego"""
    cant_j, cant_pc = contadorFichas(tablero, color_jugador, color_pc)

    print("\nFin de la partida")
    print(f"Fichas jugador ({color_jugador}): {cant_j}")
    print(f"Fichas computadora ({color_pc}): {cant_pc}")

    if cant_j > cant_pc:
        print("Ganó el jugador")
    elif cant_pc > cant_j:
        print("Ganó la computadora")
    else:
        print("Empate")


def ejecutarPartida(tablero, turno, color_jugador, color_pc, modo):
    """Ejecuta el bucle principal del juego alternando turnos"""
    while jugadasPosibles(tablero, 'B') or jugadasPosibles(tablero, 'N'):

        if not jugadasPosibles(tablero, turno):
            print(f"{turno} no tiene jugadas, se salta el turno")
            turno = color_pc if turno == color_jugador else color_jugador
            continue

        if turno == color_jugador:
            turnoJugador(tablero, turno)
        else:
            if modo == "0":
                nivel0(tablero, turno)
            else:
                nivel1(tablero, turno)

        turno = color_pc if turno == color_jugador else color_jugador

    mostrarResultadoFinal(tablero, color_jugador, color_pc)


def main(nombre_archivo, color_jugador, modo):
    """Inicializa y ejecuta una partida de Othello"""
    with open(nombre_archivo, "r") as f:
        lineas = [line.strip() for line in f.readlines()]

        if len(lineas) < 9:
            raise ValueError("El archivo no contiene un tablero válido.")
    
    tablero = leerTablero(lineas[:8])
    turno = leerColorInicial(lineas[8])

    print("Tablero inicial:")
    imprimirTablero(tablero)

    color_pc = 'B' if color_jugador == 'N' else 'N'

    ejecutarPartida(tablero, turno, color_jugador, color_pc, modo)

import sys

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Uso: ./othello.py archivo color modo")
        sys.exit(1)

    main(sys.argv[1], sys.argv[2], sys.argv[3])