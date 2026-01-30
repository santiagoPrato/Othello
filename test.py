from othello import *

def tablero_inicial():
    return [
        ['X','X','X','X','X','X','X','X'],
        ['X','X','X','X','X','X','X','X'],
        ['X','X','X','X','X','X','X','X'],
        ['X','X','X','N','B','X','X','X'],
        ['X','X','X','B','N','X','X','X'],
        ['X','X','X','X','X','X','X','X'],
        ['X','X','X','X','X','X','X','X'],
        ['X','X','X','X','X','X','X','X']
    ]


def test_imprimirTablero():
    tablero = [
        ["X", ".", "X"],
        [".", "X", "."],
        ["X", ".", "X"]
    ]

    try:
        imprimirTablero(tablero)
        print("Tests imprimirTablero OK")
    except Exception as e:
        print("Falla al imprimir:", e)


def test_convertirJugada():
    try:
        # Casos válidos
        assert convertirJugada("A1") == (0,0)
        assert convertirJugada("H8") == (7,7)
        assert convertirJugada("D3") == (2,3)
        # Casos inválidos
        assert convertirJugada("Z9") is None  # Fuera de rango
        assert convertirJugada("AA") is None  # Formato incorrecto
        assert convertirJugada("5D") is None  # Orden incorrecto
        print("Tests convertirJugada OK")
    except AssertionError:
        print("Falla en convertirJugada")


def test_jugadaValida():
    t = tablero_inicial()

    try:
        # Jugadas válidas para negras desde la posición inicial
        assert jugadaValida(t, 2, 3, 'N') == True
        # Jugada inválida: mismo lugar pero color incorrecto
        assert jugadaValida(t, 2, 3, 'B') == False
        # Jugada inválida: casilla ocupada
        assert jugadaValida(t, 3, 3, 'N') == False
    except AssertionError:
        print("Falla en jugadaValida")


def test_aplicarJugada():
    t = tablero_inicial()

    # Aplicar jugada negra en posición (2,3)
    aplicarJugada(t, 2, 3, 'N')
    try:
        # Verificar que se colocó la ficha negra
        assert t[2][3] == 'N'
        # Verificar que se volteó la ficha blanca en (3,3)
        assert t[3][3] == 'N'
        print("Tests aplicarJugada OK")
    except AssertionError:
        print("Falla en aplicarJugada")


def test_jugadasPosibles():
    t = tablero_inicial()

    posiblesN = jugadasPosibles(t, 'N')

    # Las 4 jugadas posibles para negras al inicio
    esperadas = {(2,3), (3,2), (4,5), (5,4)}

    try:
        assert set(posiblesN) == esperadas
        print("Tests jugadasPosibles OK")
    except AssertionError:
        print("Falla en jugadasPosibles")
    

def test_contarFichasVueltas():
    t = tablero_inicial()

    try:
        # Al jugar en (2,3) se voltea 1 ficha
        assert contarFichasVueltas(t, 2, 3, 'N') == 1
        # En (0,0) no se voltea ninguna ficha (no es jugada válida)
        assert contarFichasVueltas(t, 0, 0, 'N') == 0
        print("Tests contarFichasVueltas OK")
    except AssertionError:
        print("Falla en contarFichasVueltas")

def test_elegirNivel1():
    t = tablero_inicial()

    jugada = elegirNivel1(t, 'N')

    try:
        # La jugada debe ser una de las 4 posibles
        assert jugada in [(2,3), (3,2), (4,5), (5,4)]
        print("Tests elegirNivel1 OK")
    except AssertionError:
        print("Falla en elegirNivel1")


def test_contarFichas():
    t = tablero_inicial()

    try:
        # Al inicio hay 2 fichas de cada color
        assert contadorFichas(t, 'N', 'B') == (2,2)
        print("Tests contarFichas OK")
    except AssertionError:
        print("Falla en contarFichas")

def run_tests():
    test_imprimirTablero()
    test_convertirJugada()
    test_jugadaValida()
    test_aplicarJugada()
    test_jugadasPosibles()
    test_contarFichasVueltas()
    test_elegirNivel1()
    test_contarFichas()