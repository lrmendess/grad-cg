#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <GL/glut.h>
#include <cmath>

using namespace std;

class Circle {
    private:
        int cx;
        int cy;
        GLfloat radius;
        GLfloat color[3] = { 0, 0, 0 };

    public:
        ~Circle();

        Circle();
        Circle(const int cx, const int cy, const GLfloat radius, const GLfloat* color);

        int getCx() { return cx; }
        void setCx(const int cx) { this->cx = cx; }

        int getCy() { return cy; }
        void setCy(const int cy) { this->cy = cy; }

        GLfloat getRadius() { return radius; }
        void setRadius(const GLfloat radius) { this->radius = radius; }

        GLfloat* getColor() { return color; }
        void setColor(const GLfloat* color) { for (int i = 0; i < 3; i++) this->color[i] = color[i]; }

        void drawSolidCircle();
};

#endif