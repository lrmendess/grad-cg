#include "circle.h"

Circle::~Circle() {
    
}

Circle::Circle() {

}

Circle::Circle(const GLfloat cx, const GLfloat cy, const GLfloat radius, const GLfloat* color) {
    this->cx = cx;
    this->cy = cy;

    this->startX = cx;
    this->startY = cy;

    this->radius = radius;

    for (int i = 0; i < 3; i++) {
        this->color[i] = color[i];
    }
}

void Circle::drawSolidCircle() {
        glColor3f(this->color[0], this->color[1], this->color[2]);
    
        glBegin(GL_POLYGON);
            GLfloat angle, px, py;

            for (int i = 0; i < 360; i++) {
                angle = (i * M_PI) / 180.0;
                px = cx + (cos(angle) * radius);
                py = cy + (sin(angle) * radius);
                glVertex2f(px, py);
            }
        glEnd();
}

void Circle::drawSolidRing() {
        glColor3f(this->color[0], this->color[1], this->color[2]);
    
        glBegin(GL_LINE_STRIP);
            GLfloat angle, px, py;

            for (int i = 0; i < 360; i++) {
                angle = (i * M_PI) / 180.0;
                px = cx + (cos(angle) * radius);
                py = cy + (sin(angle) * radius);
                glVertex2f(px, py);
            }
        glEnd();
}

void Circle::reset() {
    this->dead = false;
}