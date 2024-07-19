#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

// Inicializa altura e largura da janela
int WIDTH = 800;
int HEIGHT = 600;
bool isOrtho = true; // Alternar entre ortogonal e perspectiva
int face = 0; // Vari�vel para armazenar a face a ser exibida

void init() {
    glEnable(GL_DEPTH_TEST); // Habilita teste de profundidade
    glClearColor(0.0, 0.0, 0.0, 1.0); // Define a cor de fundo como preto
}

void drawCube() {
    glBegin(GL_QUADS); // Come�a a desenhar os quadrados

    // Frente
    glColor3f(1.0, 0.0, 0.0); // Vermelho
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);

    // Tr�s
    glColor3f(0.0, 1.0, 0.0); // Verde
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);

    // Superior
    glColor3f(0.0, 0.0, 1.0); // Azul
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);

    // Inferior
    glColor3f(1.0, 1.0, 0.0); // Amarelo
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);

    // Direita
    glColor3f(1.0, 0.0, 1.0); // Magenta
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);

    // Esquerda
    glColor3f(0.0, 1.0, 1.0); // Ciano
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);

    glEnd(); // Termina de desenhar os quadrados
}

void setOrthoProjection(float left, float right, float bottom, float top, float nearVal, float farVal) {
    // Muda para a matriz de proje��o
    glMatrixMode(GL_PROJECTION);
    // Reseta a matriz de proje��o para a identidade
    glLoadIdentity();

    // Define a matriz ortogr�fica manualmente
    float ortho[16] = {
        2 / (right - left), 0, 0, 0,
        0, 2 / (top - bottom), 0, 0,
        0, 0, -2 / (farVal - nearVal), 0,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farVal + nearVal) / (farVal - nearVal), 1
    };
    // Carrega a matriz ortogr�fica
    glLoadMatrixf(ortho);
}


void setPerspectiveProjection(float fovY, float aspect, float nearVal, float farVal) {
    // Muda para a matriz de proje��o
    glMatrixMode(GL_PROJECTION);
    // Reseta a matriz de proje��o para a identidade
    glLoadIdentity();
    // Calcula o fator de escala baseado no campo de vis�o vertical
    float f = 1.0 / tan(fovY * 3.14159265 / 360.0);
    // Define a matriz de perspectiva manualmente
    float perspective[16] = {
        f / aspect, 0, 0, 0,                          // Primeira coluna
        0, f, 0, 0,                                    // Segunda coluna
        0, 0, (farVal + nearVal) / (nearVal - farVal), -1,  // Terceira coluna
        0, 0, (2 * farVal * nearVal) / (nearVal - farVal), 0  // Quarta coluna
    };
    // Carrega a matriz de perspectiva
    glLoadMatrixf(perspective);
}


void setFaceView(int face) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch(face) {
        case 1:
            gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 2:
            gluLookAt(0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 3:
            gluLookAt(0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
            break;
        case 4:
            gluLookAt(0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
            break;
        case 5:
            gluLookAt(2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 6:
            gluLookAt(-2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        default:
            break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isOrtho) {
        setOrthoProjection(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
    } else {
        setPerspectiveProjection(45.0, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!isOrtho) {
        // Posiciona a c�mera apenas para proje��o em perspectiva
        gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    } else {
        // Configura a visualiza��o da face do cubo em proje��o ortogonal
        setFaceView(face);
    }

    drawCube();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    WIDTH = w;
    HEIGHT = h;
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'p' || key == 'P') {
        isOrtho = !isOrtho; // Alterna entre proje��o ortogonal e perspectiva
        glutPostRedisplay();
    } else if (key >= '1' && key <= '6') {
        face = key - '0'; // Atualiza a face a ser exibida
        glutPostRedisplay(); 
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Proje��o Ortogonal e Perspectiva");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
