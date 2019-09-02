#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "circle.h"
#include "color.h"

void init(void);
void display(void);
void mouse(int x, int y);

Color white(1.0, 1.0, 1.0);
Circle cursor(0.2, 0.5, 0.5, white);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello");

    init();

    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouse);

    glutMainLoop();

    /* C ANSI requer que main retorne um inteiro */
    return 0;
}

void display(void) {
    /* Limpar todos os pixels */
    glClear(GL_COLOR_BUFFER_BIT);

    cursor.draw();

    /* Nao esperar! */
    glFlush();
}

void init(void) {
    /* Seleciona cor de fundo (vermelho) */
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* Inicializar sistema de viz */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void mouse(int x, int y) {
    cursor.setX(x / 500.0);
    cursor.setY(1 - y / 500.0);

    glutPostRedisplay();

    printf("(%.2f, %.2f)\n", cursor.getX(), cursor.getY());
}
