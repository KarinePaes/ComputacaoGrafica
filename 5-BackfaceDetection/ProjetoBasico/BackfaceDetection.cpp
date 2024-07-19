#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

// Estrutura para representar um vetor 3D
struct Vector3 {
    float x, y, z;

    // Subtra��o de vetores
    Vector3 operator-(const Vector3& v) const {
        return { x - v.x, y - v.y, z - v.z };
    }

    // Produto vetorial para obter a normal
    Vector3 cross(const Vector3& v) const {
        return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
    }

    // Produto escalar para verificar orienta��o
    float dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Normaliza��o do vetor
    Vector3 normalize() const {
        float mag = std::sqrt(x * x + y * y + z * z);
        return { x / mag, y / mag, z / mag };
    }
};

// Estrutura para representar um tri�ngulo
struct Triangle {
    Vector3 v0, v1, v2;
    float color[3]; // Cor RGB do tri�ngulo
};

// Fun��o para calcular a normal de um tri�ngulo
Vector3 calculateNormal(const Triangle& tri) {
    // Calcula os vetores das arestas do tri�ngulo
    Vector3 edge1 = tri.v1 - tri.v0;
    Vector3 edge2 = tri.v2 - tri.v0;

    // Calcula a normal do tri�ngulo usando o produto vetorial
    Vector3 normal = edge1.cross(edge2);

    // Normaliza a normal para ter comprimento 1
    return normal.normalize();
}

// Fun��o para verificar se uma face est� oculta com base na orienta��o da normal
bool isBackface(const Triangle& tri, const Vector3& viewDir) {
    // Calcula a normal da face do tri�ngulo
    Vector3 normal = calculateNormal(tri);

    // Verifica se a face est� voltada para o observador ou para longe dele
    // A face � considerada traseira se o produto escalar entre a dire��o de vis�o e a normal for positivo
    return normal.dot(viewDir) > 0;
}

// Inicializa altura e largura da janela
int WIDTH = 800;
int HEIGHT = 600;
std::vector<Triangle> triangles;
Vector3 viewPoint = { 0.0f, 0.0f, 5.0f }; // Ponto de vista do observador
int activeTriangle = 0; // �ndice do tri�ngulo ativo

// Fun��o para renderizar texto na tela
void renderText(const std::string& text, float x, float y, float scale = 1.0f) {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0); // Cor do texto (branco)
    glRasterPos2f(x, y);

    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

// Fun��o para desenhar um tri�ngulo com uma cor espec�fica
void drawTriangle(const Triangle& tri) {
    glBegin(GL_TRIANGLES);
    glColor3f(tri.color[0], tri.color[1], tri.color[2]); // Define a cor do tri�ngulo
    glVertex3f(tri.v0.x, tri.v0.y, tri.v0.z);
    glVertex3f(tri.v1.x, tri.v1.y, tri.v1.z);
    glVertex3f(tri.v2.x, tri.v2.y, tri.v2.z);
    glEnd();
}

// Fun��o de callback para teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        activeTriangle = 0; // Seleciona o primeiro tri�ngulo
        break;
    case '2':
        activeTriangle = 1; // Seleciona o segundo tri�ngulo
        break;
    case '3':
        activeTriangle = 2; // Seleciona o terceiro tri�ngulo
        break;
    default:
        break;
    }
    glutPostRedisplay(); // Redesenha a tela ap�s a mudan�a
}

void init() {
    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade
    glClearColor(0.0, 0.0, 0.0, 1.0); // Define a cor de fundo como preto

    // Adiciona tri�ngulos com cores diferentes
    triangles.push_back({ {0.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }); // Vermelho
    triangles.push_back({ {0.0f, 1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f} }); // Verde
    triangles.push_back({ {0.0f, 1.0f, -0.5f}, {-1.0f, -1.0f, -0.5f}, {1.0f, -1.0f, -0.5f}, {0.0f, 0.0f, 1.0f} }); // Azul
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e profundidade

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewPoint.x, viewPoint.y, viewPoint.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Calcula a dire��o de vis�o do ponto de vista para o centro da cena
    Vector3 viewDir = { 0.0f - viewPoint.x, 0.0f - viewPoint.y, 0.0f - viewPoint.z };

    // Desenha o tri�ngulo ativo
    if (activeTriangle < triangles.size()) {
        const auto& tri = triangles[activeTriangle];
        if (!isBackface(tri, viewDir)) {
            drawTriangle(tri);
        }
    }

    // Renderiza texto indicando o tri�ngulo ativo
    renderText("Tri�ngulo ativo: " + std::to_string(activeTriangle + 1), 10, HEIGHT - 20);

    glutSwapBuffers(); // Troca os buffers para exibir o desenho
}

void reshape(int w, int h) {
    WIDTH = w;
    HEIGHT = h;
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Detec��o de Face Oculta");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); // Adiciona callback para teclado

    glutMainLoop();
    return 0;
}
