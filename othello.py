def cargarTableroDesdeArchivo(nombre_archivo):
    tablero = []
    with open(nombre_archivo, "r") as f:
        for linea in f:
            linea = linea.strip()
            if len(linea) == 0:
                continue
            tablero.append(list(linea))
    return tablero


def imprimirTablero(tablero):
    for i in range(8):
        for j in range(8):
            print(tablero[i][j], end=" ")

        print()


def convertirJugada(jugada):
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
    tablero[fila][col] = color
    oponente = 'B' if color == 'N' else 'N'

    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]

    valido = False

    for df, dc in dirs:
        f = fila + df
        c = col + dc
        dar_vuelta = []

        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            dar_vuelta.append((f,c))
            f += df
            c += dc

        if dar_vuelta and 0 <= f < 8 and 0 <= c < 8 and tablero[f][c]:
            for (fx, cx) in dar_vuelta:
                tablero[fx][cx] = color


def turnoJugador(tablero, color):
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
    posibles = []
    for f in range(8):
        for c in range(8):
            if jugadaValida(tablero, f, c, color):
                posibles.append((f, c))
    return posibles


def nivel0(tablero, color):
    posibles = jugadasPosibles(tablero, color)
    
    jugada = random.choice(posibles)
    aplicarJugada(tablero, jugada[0], jugada[1], color)

    colLetra = chr(jugada[1] + ord('A'))
    filNum = jugada[0] + 1
    print(f"La computadora eligio: {colLetra}{filNum}")
    imprimirTablero(tablero)
        

def contarFichasVueltas(tablero, fila, col, color):
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
    mejor_jugada = elegirNivel1(tablero, color) 
    
    aplicarJugada(tablero, mejor_jugada[0], mejor_jugada[1], color)

    colLetra = chr(mejor_jugada[1] + ord('A'))
    filNum = mejor_jugada[0] + 1
    print(f"La computadora eligio: {colLetra}{filNum}")
    imprimirTablero(tablero)


def contadorFichas(tablero, color_jugador, color_pc):
    cant_fichas_j = 0 
    cant_fichas_pc = 0
    for i in range(8):
        for j in range(8):
            if tablero[i][j] == color_jugador:
                cant_fichas_j += 1
            elif tablero[i][j] == color_pc:
                cant_fichas_pc += 1

    return  cant_fichas_j, cant_fichas_pc

def main(nombre_archivo, color_jugador):
    tablero = cargarTableroDesdeArchivo(nombre_archivo)

    print("Tablero inicial:")
    imprimirTablero(tablero)

    modo = input("Elija el modo de juego (0 o 1): ").strip()

    color_pc = 'B' if color_jugador == 'N' else 'N'

    turno = color_jugador

    while True:
        posibles = jugadasPosibles(tablero, turno)

        if not posibles:
            print(f"No hay jugadas disponibles para {turno}")
            
            otro = turno = color_pc if turno == color_jugador else color_jugador
            if not jugadasPosibles(tablero, otro):
                print("Ningún jugador tiene jugadas. Fin de la partida")
                cant_fichas_j, cant_fichas_pc = contadorFichas(tablero, color_jugador, color_pc)

                if cant_fichas_j < cant_fichas_pc:
                    print("Ganó la pc")
                elif cant_fichas_pc < cant_fichas_j:
                    print("Ganó el jugador")
                else:
                    print("Empate")
                break

            turno = otro
            continue
        
        if turno == color_jugador:
            turnoJugador(tablero, turno)
        else:
            if modo == "0":
                nivel0(tablero, turno)
            else:
                nivel1(tablero, turno)

        turno = color_pc if turno == color_jugador else color_jugador