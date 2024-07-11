#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

// Inicializa altura e largura da janela
int WIDTH = 400;
int HEIGHT = 400;
GLubyte* PixelBuffer = new GLubyte[WIDTH * HEIGHT * 3];

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glutSwapBuffers();
}

// Função que desenha um pixel na tela, passando x, y e as cores em RGB
void write_pixel(int x, int y, double r, double g, double b, GLubyte* pixels, int width, int height) {
    if (0 <= x && x < width && 0 <= y && y < height) {
        int position = (x + y * width) * 3;
        pixels[position] = r; // Define o r
        pixels[position + 1] = g; // Define o g
        pixels[position + 2] = b; // Define o b
    }
}

// Função que desenha os pontos da elipse considerando simetria
void draw_ellipse_points(int xc, int yc, int x, int y) {
    // Desenha todos os quatro pontos simétricos da elipse
    write_pixel(xc + x, yc + y, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
    write_pixel(xc - x, yc + y, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
    write_pixel(xc + x, yc - y, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
    write_pixel(xc - x, yc - y, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
}

// Função que desenha uma elipse usando o algoritmo de ponto médio
void draw_ellipse(int xc, int yc, int a, int b) {
    int x = 0;
    int y = b;

    // Variável de decisão inicial para a região 1
    // F(1, b - 1/2) = b^2 + a^2(1/4 - b + b^2 - 1)
    // O ponto médio entre E e SE
    float d1 = b * b - a * a * b + 0.25 * a * a;

    // Desenha o primeiro conjunto de pontos simétricos
    draw_ellipse_points(xc, yc, x, y);

    // Região 1
    // A condição para a região 1 é a^2(y - 0.5) > b^2(x + 1)
    while ((a * a * (y - 0.5)) > (b * b * (x + 1))) {
        if (d1 < 0) {
            // d1 < 0 significa que o ponto médio está dentro da elipse
            // Incrementa x e recalcula d1
            d1 += b * b * (2 * x + 3);
        }
        else {
            // d1 >= 0 significa que o ponto médio está fora da elipse
            // Incrementa x, decrementa y e recalcula d1
            d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            y--;
        }
        x++;
        draw_ellipse_points(xc, yc, x, y);
    }

    // Variável de decisão inicial para a região 2
    // F(x+1/2, y-1) = b^2(x+1/2)^2 + a^2(y-1)^2 - a^2b^2
    float d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;

    // Região 2
    // A condição para a região 2 é y > 0
    while (y > 0) {
        if (d2 < 0) {
            // d2 < 0 significa que o ponto médio está dentro da elipse
            // Incrementa x e recalcula d2
            d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x++;
        }
        else {
            // d2 >= 0 significa que o ponto médio está fora da elipse
            // Decrementa y e recalcula d2
            d2 += a * a * (-2 * y + 3);
        }
        y--;
        draw_ellipse_points(xc, yc, x, y);
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);

    int MainWindow = glutCreateWindow("Elipse de Ponto Médio");
    glClearColor(1.0, 1.0, 1.0, 0); // Cor de fundo branca

    draw_ellipse(200, 200, 100, 100); // Desenha uma elipse com centro em (200, 200), semi-eixo maior 100 e semi-eixo menor 100

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
