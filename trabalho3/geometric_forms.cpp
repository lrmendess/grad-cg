#include "geometric_forms.h"

void drawEllipse(GLfloat lx, GLfloat ly, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    glBegin(GL_POLYGON);
        GLfloat angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = cos(angle) * lx;
            py = sin(angle) * ly;
            glVertex2f(px, py);
        }
    glEnd();
}

void drawCircle(GLfloat l, GLfloat R, GLfloat G, GLfloat B) {
    glColor3f(R, G, B);

    glBegin(GL_POLYGON);
        GLfloat angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = cos(angle) * l;
            py = sin(angle) * l;
            glVertex2f(px, py);
        }
    glEnd();
}