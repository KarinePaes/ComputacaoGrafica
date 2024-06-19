#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

void display(void);

void multiplicaMatriz(float mat1[4][3], float mat2[3][3], float result[4][3]) {
    int i, j, k;
    // Inicializar a matriz de resultado com zeros
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = 0.0;
        }
    }
    // Multiplicação das matrizes
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
    glOrtho(0, 1, 0, 1, -1, 1);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    // Vertices do quadrado original:
    float vertices[4][3] = {
        {0.0, 0.0, 1},
        {0.0, 0.25, 1},
        {0.25, 0.25, 1},
        {0.25, 0.0, 1}
    };

    // Desenhar o quadrado original na cor vermelha:
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; ++i) {
        glVertex2f(vertices[i][0], vertices[i][1]);
    }

    glEnd();
    glPopMatrix();
    glFlush();

    // Calculo de seno e cosseno para a rotação
    float angulo_graus = 45.0; // Ângulo em graus
    float angulo_radianos = angulo_graus * (M_PI / 180.0); // Convertendo para radianos

    float seno = sin(angulo_radianos);
    float cosseno = cos(angulo_radianos);

    // Define a matriz de rotação em 45º (com translação em 0.25 para que os poligonos não se sobreponham)
    float matrizRotacao[3][3] = {
        {cosseno, seno, 0.0},
        {-seno, cosseno, 0.0},
        {0.25, 0.25, 1}
    };

    float result[4][3];

    // Aplicar rotação com translação
    for (int i = 0; i < 4; ++i) {
        multiplicaMatriz(vertices, matrizRotacao, result);
    }

    // Desenhar o quadrado rotacionado na cor verde:
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; ++i) {
        glVertex2f(result[i][0], result[i][1]);
    }

    glEnd();
    glPopMatrix();
    glFlush();

    // Marcação visual para diferenciar o polígono original e o polígono rotacionado
    glColor3f(0.0, 0.0, 0.0); // Cor preta
    glRasterPos2f(0.1, 0.1); // Posição do texto
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1'); // Desenha o caractere "1"

    glFlush();

    glColor3f(0.0, 0.0, 0.0); // Cor preta
    glRasterPos2f(0.225, 0.4); // Posição do texto
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2'); // Desenha o caractere "1"

    glFlush();
}
