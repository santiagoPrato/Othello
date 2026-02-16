"""
Módulo principal del juego Othello
Implementa la lógica del juego y dos niveles de IA.
"""

def leerTablero(lineas):
    """
    Construye el tablero 8x8 a partir de las líneas del archivo.
    Lanza ValueError si alguna fila no tiene 8 columnas.
    """
    tablero = []
    for i in range(8):
        fila = list(lineas[i])
        if len(fila) != 8:
            raise ValueError(f"La fila {i+1} del tablero no tiene 8 columnas")
        tablero.append(fila)

    return tablero

def leerColorInicial(color):
    """
    Valida y devuelve el color inicial ('B' o 'N').
    Lanza ValueError si es inválido.
    """
    if color not in ("B", "N"):
        raise ValueError("El color inicial debe ser 'B' o 'N'")

    return color


def imprimirTablero(tablero):
    for i in range(8):
        for j in range(8):
            print(tablero[i][j], end=" ")

        print()


def convertirJugada(jugada):
    """
    Convierte una jugada tipo "D3" a (fila, columna).
    Devuelve None si el formato es inválido.
    """
    if len(jugada) != 2:
        return None

    col = jugada[0].upper()
    fil = jugada[1]

    # Validar que la columna esté entre A y H
    if col < 'A' or col > 'H':
        return None
    # Validar que la fila esté entre 1 y 8
    if fil < '1' or fil > '8':
        return None

    # Convertir a índices de matriz (0-7)
    columna = ord(col) - ord('A')
    fila = int(fil) - 1

    return fila, columna


def jugadaValida(tablero, fila, col, color):
    """
    Verifica si una jugada es válida según las reglas de Othello.
    Una jugada es válida si:
    1. La casilla está vacía ('X')
    2. En al menos una dirección hay fichas del oponente seguidas por una ficha propia
    """
    # La casilla debe estar vacía
    if tablero[fila][col] != 'X':
        return False

    oponente = 'B' if color == 'N' else 'N'

    # Las 8 direcciones posibles
    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]

    valido = False

    # Revisar cada dirección
    for df, dc in dirs:
        f = fila + df
        c = col + dc
        encontrado_oponente = False

        # Avanzar mientras encontremos fichas del oponente
        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            encontrado_oponente = True
            f += df
            c += dc

        # Si encontramos al menos una ficha del oponente y luego una ficha propia, es válida
        if encontrado_oponente and 0<= f < 8 and 0 <= c < 8 and tablero[f][c] == color:
            valido = True
            break

    return valido


def aplicarJugada(tablero, fila, col, color):
    # Colocar la ficha en la posición indicada
    tablero[fila][col] = color
    oponente = 'B' if color == 'N' else 'N'

    # Las 8 direcciones posibles
    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]

    # Para cada dirección, voltear las fichas correspondientes
    for df, dc in dirs:
        f = fila + df
        c = col + dc
        dar_vuelta = []

        # Recolectar fichas del oponente en esta dirección
        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            dar_vuelta.append((f,c))
            f += df
            c += dc

        # Si encontramos una ficha propia al final, voltear todas las del oponente
        if dar_vuelta and 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == color:
            for (fx, cx) in dar_vuelta:
                tablero[fx][cx] = color


def turnoJugador(tablero, color):
    """
    Gestiona el turno del jugador humano.
    Solicita jugadas hasta que una sea válida.
    """
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
    """
    Devuelve una lista de todas las jugadas válidas para un color.
    """
    posibles = []
    for f in range(8):
        for c in range(8):
            if jugadaValida(tablero, f, c, color):
                posibles.append((f, c))
    return posibles


def nivel0(tablero, color):
    """
    IA nivel 0: elige una jugada válida al azar.
    """
    posibles = jugadasPosibles(tablero, color)
    if not posibles:
        return
    
    # Elegir aleatoriamente entre las jugadas posibles
    jugada = random.choice(posibles)
    aplicarJugada(tablero, jugada[0], jugada[1], color)

    # Convertir coordenadas a notación algebraica para mostrar
    colLetra = chr(jugada[1] + ord('A'))
    filNum = jugada[0] + 1
    print(f"La computadora eligió: {colLetra}{filNum}")
    imprimirTablero(tablero)
        

def contarFichasVueltas(tablero, fila, col, color):
    # Si la casilla no está vacía, no se puede jugar ahí
    if tablero[fila][col] != 'X':
        return 0

    oponente = 'B' if color == 'N' else 'N'

    # Las 8 direcciones posibles
    dirs = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]
    total = 0

    # Contar fichas volteadas en cada dirección
    for df, dc in dirs:
        f = fila + df
        c = col + dc
        contador = 0

        # Contar fichas del oponente consecutivas en esta dirección
        while 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == oponente:
            contador += 1
            f += df
            c += dc

        # Si encontramos una ficha propia al final, estas fichas se voltearían
        if 0 <= f < 8 and 0 <= c < 8 and tablero[f][c] == color:
            total += contador

    return total


def elegirNivel1(tablero, color):
    """
    Elige la jugada que maximiza fichas volteadas.
    Devuelve None si no hay jugadas posibles.
    """
    posibles = jugadasPosibles(tablero, color)

    if not posibles:
        return None
    
    mejor_jugada = None
    max_fichas = -1

    # Evaluar cada jugada posible
    for fila, col in posibles:
        vueltas = contarFichasVueltas(tablero, fila, col, color)
        if max_fichas < vueltas:
            max_fichas = vueltas
            mejor_jugada = (fila, col)
    
    return mejor_jugada


def nivel1(tablero, color):
    """
    IA nivel 1: estrategia maximiza fichas capturadas.
    """
    mejor_jugada = elegirNivel1(tablero, color) 
    
    aplicarJugada(tablero, mejor_jugada[0], mejor_jugada[1], color)

    # Convertir coordenadas a notación algebraica para mostrar
    colLetra = chr(mejor_jugada[1] + ord('A'))
    filNum = mejor_jugada[0] + 1
    print(f"La computadora eligió: {colLetra}{filNum}")
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


def mostrarResultadoFinal(tablero, color_jugador, color_pc):
    cant_j, cant_pc = contadorFichas(tablero, color_jugador, color_pc)

    print("\nFin de la partida")
    print(f"Fichas jugador ({color_jugador}): {cant_j}")
    print(f"Fichas computadora ({color_pc}): {cant_pc}")

    # Determinar y mostrar el ganador
    if cant_j > cant_pc:
        print("Ganó el jugador")
    elif cant_pc > cant_j:
        print("Ganó la computadora")
    else:
        print("Empate")


def ejecutarPartida(tablero, turno, color_jugador, color_pc, modo):
    """
    Ejecuta el bucle principal del juego alternando turnos
    """
    while True:
        puede_jugador = jugadasPosibles(tablero, color_jugador)
        puede_pc = jugadasPosibles(tablero, color_pc)

        # Si ninguno puede jugar → fin del juego
        if not puede_jugador and not puede_pc:
            break

        # Si el turno actual no puede jugar → pasar turno
        if turno == color_jugador and not puede_jugador:
            print(f"{turno} no tiene jugadas, se salta el turno")
            turno = color_pc
            continue

        if turno == color_pc and not puede_pc:
            print(f"{turno} no tiene jugadas, se salta el turno")
            turno = color_jugador
            continue

        # Ejecutar turno
        if turno == color_jugador:
            turnoJugador(tablero, turno)
        else:
            if modo == "0":
                nivel0(tablero, turno)
            else:
                nivel1(tablero, turno)

        # Cambiar turno
        turno = color_pc if turno == color_jugador else color_jugador

    mostrarResultadoFinal(tablero, color_jugador, color_pc)


def main(nombre_archivo, color_jugador, modo):
    """
    Inicializa el juego y ejecuta la partida completa.
    """
    with open(nombre_archivo, "r") as f:
        lineas = [line.strip() for line in f.readlines()]

        if len(lineas) < 9:
            raise ValueError("El archivo no contiene un tablero válido.")
    
    # Leer las primeras 8 líneas como el tablero    
    tablero = leerTablero(lineas[:8])
    # La línea 9 indica qué color comienza
    turno = leerColorInicial(lineas[8])

    print("Tablero inicial:")
    imprimirTablero(tablero)

    # Determinar el color de la computadora
    color_pc = 'B' if color_jugador == 'N' else 'N'

    ejecutarPartida(tablero, turno, color_jugador, color_pc, modo)

import sys

# Punto de entrada del programa
if __name__ == "__main__":
    # Verificar que se proporcionen los argumentos correctos
    if len(sys.argv) != 4:
        print("Uso: ./othello.py archivo color modo")
        sys.exit(1)

    # Ejecutar el juego con los parámetros proporcionados
    # sys.argv[1]: archivo con tablero inicial
    # sys.argv[2]: color del jugador ('B' o 'N')
    # sys.argv[3]: modo de dificultad de la IA ('0' o '1')
    main(sys.argv[1], sys.argv[2], sys.argv[3])