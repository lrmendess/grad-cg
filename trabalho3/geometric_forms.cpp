#include "geometric_forms.h"

void drawSolidCircle(GLfloat& radius, GLfloat* color) {
    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_POLYGON);
        GLfloat angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = cos(angle) * radius;
            py = sin(angle) * radius;
            glVertex2f(px, py);
        }
    glEnd();
}