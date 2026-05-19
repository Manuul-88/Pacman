#include <GL/glut.h>
#include <cmath>
#include <string>

const int ANCHO = 900, ALTO = 900;
const int FILAS = 15, COLUMNAS = 15, CELDA = 50;
int offsetX = 75, offsetY = 75;

int mapaOriginal[FILAS][COLUMNAS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,3,2,2,2,2,1,2,1,2,2,2,2,3,1},
    {1,2,1,1,1,2,1,2,1,2,1,1,1,2,1},
    {1,2,2,2,1,2,2,2,2,2,1,2,2,2,1},
    {1,1,1,2,1,1,1,0,1,1,1,2,1,1,1},
    {1,2,2,2,2,2,0,0,0,2,2,2,2,2,1},
    {1,2,1,1,1,2,1,1,1,2,1,1,1,2,1},
    {1,2,2,2,2,2,1,0,1,2,2,2,2,2,1},
    {1,1,1,2,1,2,1,0,1,2,1,2,1,1,1},
    {1,2,2,2,1,2,2,2,2,2,1,2,2,2,1},
    {1,2,1,2,1,1,1,0,1,1,1,2,1,2,1},
    {1,2,1,2,2,2,2,2,2,2,2,2,1,2,1},
    {1,2,1,1,1,2,1,1,1,2,1,1,1,2,1},
    {1,3,2,2,2,2,2,0,2,2,2,2,2,3,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int mapa[FILAS][COLUMNAS];

float pacmanX, pacmanY;
float boca = 25;
bool abriendo = true;

int direccion = 0;
int direccionDeseada = 0;

int puntaje = 0;
bool gameOver = false;
bool ganador = false;

bool modoPoder = false;
int timerPoder = 0;

int contadorPacman = 0;
int contadorFantasma = 0;

struct Fantasma {
    int fila, col;
    int dir;
    float x, y;
    float r, g, b;
};

Fantasma fantasmas[2];

int obtenerColumna(float x) {
    return (int)((x - offsetX) / CELDA);
}

int obtenerFila(float y) {
    return FILAS - 1 - (int)((y - offsetY) / CELDA);
}

float centroX(int c) {
    return offsetX + c * CELDA + CELDA / 2;
}

float centroY(int f) {
    return offsetY + (FILAS - 1 - f) * CELDA + CELDA / 2;
}

void texto(float x, float y, std::string t) {
    glRasterPos2f(x, y);
    for(char c : t) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void rect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void circulo(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for(int i = 0; i <= 360; i++) {
            float a = i * 3.14159265 / 180.0;
            glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
        }
    glEnd();
}

void circuloParcial(float cx, float cy, float r, float ini, float fin) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for(float i = ini; i <= fin; i += 2) {
            float a = i * 3.14159265 / 180.0;
            glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
        }
    glEnd();
}

void dibujarPacman(float x, float y, float r) {
    float ini, fin;

    if(direccion == 0) { ini = boca; fin = 360 - boca; }
    else if(direccion == 1) { ini = 180 + boca; fin = 540 - boca; }
    else if(direccion == 2) { ini = 90 + boca; fin = 450 - boca; }
    else { ini = 270 + boca; fin = 630 - boca; }

    glColor3f(1.0, 0.85, 0.0);
    circuloParcial(x, y, r, ini, fin);
}

void dibujarFantasma(float x, float y, float r, float cr, float cg, float cb) {
    glColor3f(cr, cg, cb);

    circuloParcial(x, y + 5, r, 0, 180);
    rect(x - r, y - r, r * 2, r + 5);

    glBegin(GL_TRIANGLES);
        glVertex2f(x - r, y - r);
        glVertex2f(x - r + 10, y - r - 12);
        glVertex2f(x - r + 20, y - r);

        glVertex2f(x - 8, y - r);
        glVertex2f(x + 2, y - r - 12);
        glVertex2f(x + 12, y - r);

        glVertex2f(x + r - 20, y - r);
        glVertex2f(x + r - 10, y - r - 12);
        glVertex2f(x + r, y - r);
    glEnd();

    glColor3f(1,1,1);
    circulo(x - 9, y + 4, 6);
    circulo(x + 9, y + 4, 6);

    glColor3f(0,0,1);
    circulo(x - 9, y + 4, 3);
    circulo(x + 9, y + 4, 3);
}

void dibujarMapa() {
    for(int f = 0; f < FILAS; f++) {
        for(int c = 0; c < COLUMNAS; c++) {
            float x = offsetX + c * CELDA;
            float y = offsetY + (FILAS - 1 - f) * CELDA;

            if(mapa[f][c] == 1) {
                glColor3f(0.02, 0.02, 0.45);
                rect(x + 3, y + 3, CELDA - 6, CELDA - 6);

                glColor3f(0.0, 0.55, 1.0);
                glLineWidth(3);
                glBegin(GL_LINE_LOOP);
                    glVertex2f(x + 3, y + 3);
                    glVertex2f(x + CELDA - 3, y + 3);
                    glVertex2f(x + CELDA - 3, y + CELDA - 3);
                    glVertex2f(x + 3, y + CELDA - 3);
                glEnd();
            }

            if(mapa[f][c] == 2) {
                glColor3f(1.0, 0.85, 0.6);
                circulo(x + CELDA / 2, y + CELDA / 2, 4);
            }

            if(mapa[f][c] == 3) {
                glColor3f(1,1,1);
                circulo(x + CELDA / 2, y + CELDA / 2, 10);
            }
        }
    }
}

bool esCamino(int f, int c) {
    if(f < 0 || f >= FILAS || c < 0 || c >= COLUMNAS) return false;
    return mapa[f][c] != 1;
}

float distanciaCuadrada(int f1, int c1, int f2, int c2) {
    return (f1 - f2) * (f1 - f2) + (c1 - c2) * (c1 - c2);
}

void comerPunto() {
    int f = obtenerFila(pacmanY);
    int c = obtenerColumna(pacmanX);

    if(mapa[f][c] == 2) {
        mapa[f][c] = 0;
        puntaje += 10;
    }

    if(mapa[f][c] == 3) {
        mapa[f][c] = 0;
        puntaje += 50;
        modoPoder = true;
        timerPoder = 0;
    }
}

bool quedanPuntos() {
    for(int f = 0; f < FILAS; f++)
        for(int c = 0; c < COLUMNAS; c++)
            if(mapa[f][c] == 2 || mapa[f][c] == 3) return true;

    return false;
}

void revisarColision() {
    for(int i = 0; i < 2; i++) {
        float dx = pacmanX - fantasmas[i].x;
        float dy = pacmanY - fantasmas[i].y;
        float d = sqrt(dx * dx + dy * dy);

        if(d < 25) {
            if(modoPoder) {
                puntaje += 200;
                fantasmas[i].fila = 7;
                fantasmas[i].col = 7;
                fantasmas[i].x = centroX(7);
                fantasmas[i].y = centroY(7);
            } else {
                gameOver = true;
            }
        }
    }
}

bool puedeMover(int dir) {
    int f = obtenerFila(pacmanY);
    int c = obtenerColumna(pacmanX);

    if(dir == 0) c++;
    if(dir == 1) c--;
    if(dir == 2) f--;
    if(dir == 3) f++;

    return esCamino(f, c);
}

void moverPacman() {
    if(puedeMover(direccionDeseada)) {
        direccion = direccionDeseada;
    }

    int f = obtenerFila(pacmanY);
    int c = obtenerColumna(pacmanX);

    if(direccion == 0) c++;
    if(direccion == 1) c--;
    if(direccion == 2) f--;
    if(direccion == 3) f++;

    if(esCamino(f, c)) {
        pacmanX = centroX(c);
        pacmanY = centroY(f);

        comerPunto();
        revisarColision();

        if(!quedanPuntos()) ganador = true;
    }
}

void moverFantasma(int i) {
    int pf = obtenerFila(pacmanY);
    int pc = obtenerColumna(pacmanX);

    int objetivoF = pf;
    int objetivoC = pc;

    if(i == 1) {
        if(direccion == 0) objetivoC += 3;
        if(direccion == 1) objetivoC -= 3;
        if(direccion == 2) objetivoF -= 3;
        if(direccion == 3) objetivoF += 3;
    }

    if(modoPoder) {
        objetivoF = 13;
        objetivoC = (i == 0) ? 1 : 13;
    }

    if(objetivoF < 0) objetivoF = 0;
    if(objetivoF >= FILAS) objetivoF = FILAS - 1;
    if(objetivoC < 0) objetivoC = 0;
    if(objetivoC >= COLUMNAS) objetivoC = COLUMNAS - 1;

    int f = fantasmas[i].fila;
    int c = fantasmas[i].col;

    int df[4] = {0,0,-1,1};
    int dc[4] = {1,-1,0,0};

    int mejorDir = fantasmas[i].dir;
    float mejorDist = 999999;

    for(int d = 0; d < 4; d++) {
        int nf = f + df[d];
        int nc = c + dc[d];

        if(esCamino(nf, nc)) {
            float dist = distanciaCuadrada(nf, nc, objetivoF, objetivoC);
            if(dist < mejorDist) {
                mejorDist = dist;
                mejorDir = d;
            }
        }
    }

    int nf = f + df[mejorDir];
    int nc = c + dc[mejorDir];

    if(esCamino(nf, nc)) {
        fantasmas[i].fila = nf;
        fantasmas[i].col = nc;
        fantasmas[i].dir = mejorDir;
        fantasmas[i].x = centroX(nc);
        fantasmas[i].y = centroY(nf);
    }
}

void reiniciarJuego() {
    for(int f = 0; f < FILAS; f++)
        for(int c = 0; c < COLUMNAS; c++)
            mapa[f][c] = mapaOriginal[f][c];

    pacmanX = centroX(1);
    pacmanY = centroY(13);
    direccion = 0;
    direccionDeseada = 0;

    fantasmas[0] = {5,7,0,centroX(7),centroY(5),1.0,0.0,0.0};
    fantasmas[1] = {7,7,1,centroX(7),centroY(7),1.0,0.4,0.7};

    puntaje = 0;
    gameOver = false;
    ganador = false;
    modoPoder = false;
    timerPoder = 0;
    contadorPacman = 0;
    contadorFantasma = 0;
    boca = 25;
    abriendo = true;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.06, 0.06, 0.09);
    rect(0, 0, ANCHO, ALTO);

    dibujarMapa();
    dibujarPacman(pacmanX, pacmanY, 20);

    for(int i = 0; i < 2; i++) {
        if(modoPoder)
            dibujarFantasma(fantasmas[i].x, fantasmas[i].y, 20, 0.1, 0.3, 1.0);
        else
            dibujarFantasma(fantasmas[i].x, fantasmas[i].y, 20,
                            fantasmas[i].r, fantasmas[i].g, fantasmas[i].b);
    }

    if(gameOver) {
        glColor3f(1,0,0);
        texto(390, 460, "GAME OVER");
        texto(325, 430, "Presiona R para reiniciar");
    }

    if(ganador) {
        glColor3f(0,1,0);
        texto(400, 460, "GANASTE");
        texto(325, 430, "Presiona R para reiniciar");
    }

    glutSwapBuffers();
}

void tecladoEspecial(int tecla, int x, int y) {
    if(gameOver || ganador) return;

    if(tecla == GLUT_KEY_RIGHT) direccionDeseada = 0;
    if(tecla == GLUT_KEY_LEFT)  direccionDeseada = 1;
    if(tecla == GLUT_KEY_UP)    direccionDeseada = 2;
    if(tecla == GLUT_KEY_DOWN)  direccionDeseada = 3;
}

void tecladoNormal(unsigned char tecla, int x, int y) {
    if(tecla == 'r' || tecla == 'R') {
        reiniciarJuego();
    }
}

void actualizar(int valor) {
    if(!gameOver && !ganador) {
        if(abriendo) boca += 3;
        else boca -= 3;

        if(boca >= 38) abriendo = false;
        if(boca <= 6) abriendo = true;

        contadorPacman++;
        if(contadorPacman >= 6) {
            moverPacman();
            contadorPacman = 0;
        }

        contadorFantasma++;
        if(contadorFantasma >= 16) {
            moverFantasma(0);
            moverFantasma(1);
            contadorFantasma = 0;
        }

        if(modoPoder) {
            timerPoder++;
            if(timerPoder >= 180) {
                modoPoder = false;
            }
        }

        revisarColision();
    }

    glutPostRedisplay();
    glutTimerFunc(30, actualizar, 0);
}

void inicializar() {
    glClearColor(0,0,0,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, ANCHO, 0, ALTO);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    reiniciarJuego();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(ANCHO, ALTO);
    glutInitWindowPosition(80, 20);
    glutCreateWindow("Pac-Man OpenGL por Manuel Silva Madird");

    inicializar();

    glutDisplayFunc(display);
    glutSpecialFunc(tecladoEspecial);
    glutKeyboardFunc(tecladoNormal);
    glutTimerFunc(30, actualizar, 0);

    glutMainLoop();

    return 0;
}