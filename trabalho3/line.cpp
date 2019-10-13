#include "line.h"

Line::~Line() {

}

Line::Line() {

}

Line::Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat* color) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    for (int i = 0; i < 3; i++) {
        this->color[i] = color[i];
    }
}

void Line::drawSolidLine() {
    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}