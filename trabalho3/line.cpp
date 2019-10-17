#include "line.h"

Line::~Line() {

}

Line::Line() {

}

Line::Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

void Line::drawSolidLine() {
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}