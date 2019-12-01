#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <GL/glut.h>
#include <cmath>

using namespace std;

class Circle {
    private:
        GLfloat cx;
        GLfloat cy;
        GLfloat radius;
        GLfloat color[3] = { 0, 0, 0 };

        GLfloat startX;
        GLfloat startY;
        
        GLuint texture;

        bool dead = false;

    public:        
        ~Circle();

        Circle();
        Circle(const GLfloat cx, const GLfloat cy, const GLfloat radius, const GLfloat* color);

        GLfloat getCx() { return cx; }
        void setCx(const GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(const GLfloat cy) { this->cy = cy; }

        GLfloat getRadius() { return radius; }
        void setRadius(const GLfloat radius) { this->radius = radius; }

        GLfloat* getColor() { return color; }
        void setColor(const GLfloat* color) { for (int i = 0; i < 3; i++) this->color[i] = color[i]; }

        bool isDead() { return dead; }
        void kill() { this->dead = true; }
        
        void setTexture(GLuint texture) { this->texture = texture }

        void reset();

        void drawSolidCircle();
};

#endif