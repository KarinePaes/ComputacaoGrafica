#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

// inicializa altura e largura da janela
int WIDTH = 400;
int HEIGHT = 400;
GLubyte* PixelBuffer = new GLubyte[WIDTH * HEIGHT * 3];

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glutSwapBuffers();
}


// função que desenha um pixel na tela, passado x y e as cores em RGB
void write_pixel(int x, int y, double r, double g, double b, GLubyte* pixels, int width, int height) {
    if (0 <= x && x < width && 0 <= y && y < height) {
        int position = (x + y * width) * 3;
        pixels[position] = r; // define o r
        pixels[position + 1] = g; // define o g
        pixels[position + 2] = b; // define o b
    }
}

// função que desenha a linha dado um (x,y) inicial e (x,y) final (apenas para o primeiro quadrante)
void write_line_simple(int x1, int y1, int x2, int y2) {
    int dx, dy, e, ne, d, x, y;

    // calcula a diferença entre os pontos x1-x2 e y1-y2
    dx = x2 - x1;
    dy = y2 - y1;

    // calcula o primeiro D seguindo a fórmula
    d = 2 * dy - dx;

    // define qual será o incremento de d caso a linha passe por baixo
    e = 2 * dy;
    // define qual será o incremento de d caso a linha passe por cima
    ne = 2 * (dy - dx);

    x = x1;
    y = y1;

    // desenha o primeiro pixel
    write_pixel(x, y, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);

    // usando x como padrão (primeiro octante apenas), o algoritmo de decisão vai percorrer até alcançar x2
    while (x < x2) {
        // se d for menor do que zero, o pixel escolhido será o de baixo, portanto a pixel desenhado andará apenas uma posição em x
        if (d <= 0) {
            d = d + e;
            x = x + 1;
        }
        // se d for maior do que zero, o pixel escolhido é o de cima, então tanto x quanto y andarão 1 casa
        else {
            d = d + ne;
            x = x + 1;
            y = y + 1;
        }
        // desenha o pixel da interação
        write_pixel(x, y, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
    }
}

// função que desenha a linha dado um (x,y) inicial e (x,y) final em qualquer direção
void write_line(int x1, int y1, int x2, int y2) {
    int dx, dy, inc_x, inc_y, d, x, y;

    // calcula a diferença entre os pontos x1-x2 e y1-y2 de forma absoluta
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    /* determina a direção de incremento para x e y com base nos pontos inicial e final:
        se x2 estiver à direita de x1, ele irá somar 1 no incremento de D
        se x2 estiver à esquerda de x1, ele irá diminuir 1 no incremento de D
    */
    inc_x = (x2 > x1) ? 1 : -1;
    inc_y = (y2 > y1) ? 1 : -1;

    // se a diferença entre x1 e x2 for maior que y1 e y2, a linha tem inclinação menor que 45 graus (primeiro quadrante)
    // se o contrário, a inclinação estará fora e sera preciso um segundo tratamento
    if (dx >= dy) {
        // Algoritmo para linhas com inclinação menor ou igual a 45 graus

        // calcula o primeiro D seguindo a fórmula
        d = 2 * dy - dx;
        while (x1 != x2) {
            write_pixel(x1, y1, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
            // atualiza a posição em x
            x1 += inc_x;
            // calcula o próximo d e incrementa y se a linha estiver acima do ponto 
            if (d >= 0) {
                y1 += inc_y;
                d -= 2 * dx;
            }
            // se não, apenas faz o incremento de x e calcula o novo d com base no dy
            d += 2 * dy;
        }
    }
    else {
        // algoritmo para linhas com inclinação maior que 45 graus

        // calcula o primeiro D seguindo a fórmula
        d = 2 * dx - dy;
        while (y1 != y2) {
            write_pixel(x1, y1, 0.0, 0.0, 0.0, PixelBuffer, WIDTH, HEIGHT);
            // atualiza a posição em y
            y1 += inc_y;
            // calcula o próximo d e incrementa x se a linha estiver acima do ponto 
            if (d >= 0) {
                x1 += inc_x;
                d -= 2 * dy;
            }
            // se não, apenas faz o incremento de y e calcula o novo d com base no dx
            d += 2 * dx;
        }
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);

    int MainWindow = glutCreateWindow("Reta Otimizada");
    glClearColor(0.0, 0.0, 0.0, 0);
  
    write_line(100, 100, 300, 150);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

