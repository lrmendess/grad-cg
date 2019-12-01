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
    //glColor3f(color[0], color[1], color[2]);
    
    glColor3f(1,1,1);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    GLUquadricObj* obj = gluNewQuadric();
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluQuadricTexture(obj, texture);
        gluCylinder(obj, this->radius, this->radius, this->radius * 2, 360, 360);
        gluDeleteQuadric(obj);    
/*
    glBegin(GL_POLYGON);
        GLfloat angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = cx + (cos(angle) * radius);
            py = cy + (sin(angle) * radius);
            glVertex3f(px, py, 0);
            glVertex3f(px, py, 500);
        }
    glEnd();*/
}

void Circle::reset() {
    this->dead = false;
}