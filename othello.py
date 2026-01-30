"""
Módulo principal del juego Othello (Reversi)
Este módulo implementa la lógica del juego Othello, incluyendo la validación de jugadas,
aplicación de movimientos, y dos niveles de IA para la computadora.
"""

def leerTablero(lineas):
    """
    Lee y construye el tablero a partir de las líneas de un archivo.
    
    Args:
        lineas: Lista de strings donde cada línea representa una fila del tablero (8x8)
    
    Returns:
        Una matriz 8x8 representando el tablero del juego
    
    Raises:
        ValueError: Si alguna fila no tiene exactamente 8 columnas
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
    Valida el color inicial proporcionado.
    
    Args:
        color: String representando el color ('B' para blancas, 'N' para negras)
    
    Returns:
        El color validado
    
    Raises:
        ValueError: Si el color no es 'B' ni 'N'
    """
    if color not in ("B", "N"):
        raise ValueError("El color inicial debe ser 'B' o 'N'")

    return color


def imprimirTablero(tablero):
    """
    Imprime el tablero en la consola de forma legible.
    
    Args:
        tablero: Matriz 8x8 con el estado actual del tablero
    """
    for i in range(8):
        for j in range(8):
            print(tablero[i][j], end=" ")

        print()


def convertirJugada(jugada):
    """
    Convierte una jugada en notación algebraica (ej: "D3") a coordenadas de matriz.
    
    Args:
        jugada: String con la jugada en formato de columna-letra + fila-número (ej: "D3")
    
    Returns:
        Tupla (fila, columna) con índices de 0 a 7, o None si el formato es inválido
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
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero
        fila: Índice de fila (0-7)
        col: Índice de columna (0-7)
        color: Color del jugador ('B' o 'N')
    
    Returns:
        True si la jugada es válida, False en caso contrario
    """
    # La casilla debe estar vacía
    if tablero[fila][col] != 'X':
        return False

    oponente = 'B' if color == 'N' else 'N'

    # Las 8 direcciones posibles (arriba, abajo, izquierda, derecha y diagonales)
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
    """
    Aplica una jugada en el tablero, colocando la ficha y volteando las fichas capturadas.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero (se modifica in-place)
        fila: Índice de fila donde colocar la ficha (0-7)
        col: Índice de columna donde colocar la ficha (0-7)
        color: Color del jugador que realiza la jugada ('B' o 'N')
    """
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
    Maneja el turno de un jugador humano, solicitando y validando la jugada.
    
    Solicita repetidamente una jugada hasta que sea válida, luego la aplica
    y muestra el tablero actualizado.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero (se modifica in-place)
        color: Color del jugador ('B' o 'N')
    """
    while True:
        jugada = input("Ingrese su jugada (ej: D3): ").strip()

        # Convertir la jugada de notación algebraica a coordenadas
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
    Encuentra todas las jugadas válidas posibles para un color dado.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero
        color: Color del jugador ('B' o 'N')
    
    Returns:
        Lista de tuplas (fila, columna) con todas las jugadas válidas
    """
    posibles = []
    for f in range(8):
        for c in range(8):
            if jugadaValida(tablero, f, c, color):
                posibles.append((f, c))
    return posibles


def nivel0(tablero, color):
    """
    IA de nivel 0: elige una jugada aleatoria entre las posibles.
    
    Este es el nivel más básico de dificultad donde la computadora
    simplemente selecciona al azar cualquier movimiento válido.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero (se modifica in-place)
        color: Color de la computadora ('B' o 'N')
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
    """
    Cuenta cuántas fichas del oponente se voltearían con una jugada específica.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero
        fila: Índice de fila de la jugada (0-7)
        col: Índice de columna de la jugada (0-7)
        color: Color del jugador ('B' o 'N')
    
    Returns:
        Número total de fichas que se voltearían
    """
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
    Elige la mejor jugada según el criterio de nivel 1: maximizar fichas volteadas.
    
    Este algoritmo evalúa todas las jugadas posibles y selecciona la que
    voltea la mayor cantidad de fichas del oponente.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero
        color: Color de la computadora ('B' o 'N')
    
    Returns:
        Tupla (fila, columna) con la mejor jugada, o None si no hay jugadas posibles
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
    IA de nivel 1: elige la jugada que voltea más fichas.
    
    Este nivel de dificultad utiliza una estrategia greedy (voraz) que
    busca maximizar las fichas capturadas en cada turno.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero (se modifica in-place)
        color: Color de la computadora ('B' o 'N')
    """
    mejor_jugada = elegirNivel1(tablero, color) 
    
    aplicarJugada(tablero, mejor_jugada[0], mejor_jugada[1], color)

    # Convertir coordenadas a notación algebraica para mostrar
    colLetra = chr(mejor_jugada[1] + ord('A'))
    filNum = mejor_jugada[0] + 1
    print(f"La computadora eligió: {colLetra}{filNum}")
    imprimirTablero(tablero)


def contadorFichas(tablero, color_jugador, color_pc):
    """
    Cuenta las fichas de cada jugador en el tablero.
    
    Args:
        tablero: Matriz 8x8 con el estado del tablero
        color_jugador: Color del jugador humano ('B' o 'N')
        color_pc: Color de la computadora ('B' o 'N')
    
    Returns:
        Tupla (fichas_jugador, fichas_pc) con el conteo de fichas de cada uno
    """
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
    """
    Muestra el resultado final del juego con el conteo de fichas y el ganador.
    
    Args:
        tablero: Matriz 8x8 con el estado final del tablero
        color_jugador: Color del jugador humano ('B' o 'N')
        color_pc: Color de la computadora ('B' o 'N')
    """
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
    Ejecuta el bucle principal del juego alternando turnos hasta que termine.
    
    El juego continúa mientras al menos uno de los jugadores tenga jugadas válidas.
    Si un jugador no tiene jugadas, se salta su turno.
    
    Args:
        tablero: Matriz 8x8 con el estado inicial del tablero (se modifica in-place)
        turno: Color del jugador que comienza ('B' o 'N')
        color_jugador: Color del jugador humano ('B' o 'N')
        color_pc: Color de la computadora ('B' o 'N')
        modo: Nivel de dificultad de la IA ("0" para aleatorio, "1" para greedy)
    """
    # Continuar mientras haya jugadas posibles para algún jugador
    while jugadasPosibles(tablero, 'B') or jugadasPosibles(tablero, 'N'):

        # Si el jugador actual no tiene jugadas, saltar su turno
        if not jugadasPosibles(tablero, turno):
            print(f"{turno} no tiene jugadas, se salta el turno")
            turno = color_pc if turno == color_jugador else color_jugador
            continue

        # Ejecutar el turno correspondiente
        if turno == color_jugador:
            turnoJugador(tablero, turno)
        else:
            # Elegir nivel de IA según el modo
            if modo == "0":
                nivel0(tablero, turno)
            else:
                nivel1(tablero, turno)

        # Cambiar de turno
        turno = color_pc if turno == color_jugador else color_jugador

    mostrarResultadoFinal(tablero, color_jugador, color_pc)


def main(nombre_archivo, color_jugador, modo):
    """
    Función principal que inicializa y ejecuta una partida de Othello.
    
    Lee el tablero inicial desde un archivo, configura los colores de los jugadores,
    y ejecuta la partida completa.
    
    Args:
        nombre_archivo: Ruta del archivo con el tablero inicial (8 líneas de 8 caracteres + color inicial)
        color_jugador: Color elegido por el jugador humano ('B' o 'N')
        modo: Nivel de dificultad de la IA ("0" o "1")
    
    Raises:
        ValueError: Si el archivo no tiene el formato correcto
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