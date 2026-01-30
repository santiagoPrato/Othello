"""Suite de pruebas para el módulo othello.py"""
from othello import *

def tablero_inicial():
    """Crea un tablero en su configuración inicial"""
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
        assert convertirJugada("A1") == (0,0)
        assert convertirJugada("H8") == (7,7)
        assert convertirJugada("D3") == (2,3)
        assert convertirJugada("Z9") is None
        assert convertirJugada("AA") is None
        assert convertirJugada("5D") is None
        print("Tests convertirJugada OK")
    except AssertionError:
        print("Falla en convertirJugada")


def test_jugadaValida():
    t = tablero_inicial()

    try:
        assert jugadaValida(t, 2, 3, 'N') == True
        assert jugadaValida(t, 2, 3, 'B') == False
        assert jugadaValida(t, 3, 3, 'N') == False
    except AssertionError:
        print("Falla en jugadaValida")


def test_aplicarJugada():
    t = tablero_inicial()

    aplicarJugada(t, 2, 3, 'N')
    try:
        assert t[2][3] == 'N'
        assert t[3][3] == 'N'
        print("Tests aplicarJugada OK")
    except AssertionError:
        print("Falla en aplicarJugada")


def test_jugadasPosibles():
    t = tablero_inicial()

    posiblesN = jugadasPosibles(t, 'N')

    esperadas = {(2,3), (3,2), (4,5), (5,4)}

    try:
        assert set(posiblesN) == esperadas
        print("Tests jugadasPosibles OK")
    except AssertionError:
        print("Falla en jugadasPosibles")
    

def test_contarFichasVueltas():
    t = tablero_inicial()

    try:
        assert contarFichasVueltas(t, 2, 3, 'N') == 1
        assert contarFichasVueltas(t, 0, 0, 'N') == 0
        print("Tests contarFichasVueltas OK")
    except AssertionError:
        print("Falla en contarFichasVueltas")

def test_elegirNivel1():
    t = tablero_inicial()

    jugada = elegirNivel1(t, 'N')

    try:
        assert jugada in [(2,3), (3,2), (4,5), (5,4)]
        print("Tests elegirNivel1 OK")
    except AssertionError:
        print("Falla en elegirNivel1")


def test_contarFichas():
    t = tablero_inicial()

    try:
        assert contadorFichas(t, 'N', 'B') == (2,2)
        print("Tests contarFichas OK")
    except AssertionError:
        print("Falla en contarFichas")

def run_tests():
    """Ejecuta todas las pruebas del módulo"""
    test_imprimirTablero()
    test_convertirJugada()
    test_jugadaValida()
    test_aplicarJugada()
    test_jugadasPosibles()
    test_contarFichasVueltas()
    test_elegirNivel1()
    test_contarFichas()