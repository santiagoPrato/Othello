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
    assert convertirJugada("A1") == (0,0)
    assert convertirJugada("H8") == (7,7)
    assert convertirJugada("D3") == (2,3)
    assert convertirJugada("Z9") is None
    assert convertirJugada("AA") is None
    assert convertirJugada("5D") is None


def test_jugadaValida():
    t = tablero_inicial()

    assert jugadaValida(t, 2, 3, 'N') == True
    assert jugadaValida(t, 2, 3, 'B') == False
    assert jugadaValida(t, 3, 3, 'N') == False


def test_aplicarJugada():
    t = tablero_inicial()

    aplicarJugada(t, 2, 3, 'N')
    assert t[2][3] == 'N'
    assert t[3][3] == 'N'


def test_jugadasPosibles():
    t = tablero_inicial()

    posiblesN = jugadasPosibles(t, 'N')

    esperadas = {(2,3), (3,2), (4,5), (5,4)}

    assert set(posiblesN) == esperadas
    

def test_contarFichasVueltas():
    t = tablero_inicial()

    assert contarFichasVueltas(t, 2, 3, 'N') == 1
    assert contarFichasVueltas(t, 0, 0, 'N') == 0


def test_elegirNivel1():
    t = tablero_inicial()

    jugada = elegirNivel1(t, 'N')

    assert jugada in [(2,3), (3,2), (4,5), (5,4)]


def test_contarFichas():
    t = tablero_inicial()

    assert contadorFichas(t, 'N', 'B') == (2,2)