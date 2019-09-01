#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "circle.h"

void init(void);
void display(void);
void drawSolidCircle(float radius, float x, float y, float r, float g, float b);
void mouse(int x, int y);

float radius = 0.2;
float cx = 0.5;
float cy = 0.5;

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

    drawSolidCircle(radius, cx, cy, 1.0, 1.0, 1.0);

    /* Nao esperar! */
    glFlush();
}

void drawSolidCircle(float radius, float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);

    float ang, px, py;
    
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            ang = (i * M_PI) / 180.0;
            px = x + cos(ang) * radius;
            py = y + sin(ang) * radius;
            glVertex2f(px, py);
        }
    glEnd();
}

void init(void) {
    /* Seleciona cor de fundo (vermelho) */
    glClearColor(1.0, 0.0, 0.0, 0.0);

    /* Inicializar sistema de viz */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void mouse(int x, int y) {
    y = 500 - y;

    cx = (float) x / 500;
    cy = (float) y / 500;

    glutPostRedisplay();

    printf("(%.2f, %.2f)\n", cx, cy);
}
