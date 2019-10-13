#ifndef LINE_H
#define LINE_H

#include <GL/glut.h>

class Line {
    private:
        GLfloat x1;
        GLfloat y1;
        GLfloat x2;
        GLfloat y2;
        GLfloat color[3] = { 0, 0, 0 };

    public:
        ~Line();
        
        Line();
        Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat* color);

        GLfloat getX1() { return x1; }
        void setX1(const GLfloat x1) { this->x1 = x1; }

        GLfloat getY1() { return y1; }
        void setY1(const GLfloat y1) { this->y1 = y1; }

        GLfloat getX2() { return x2; }
        void setX2(const GLfloat x2) { this->x2 = x2; }

        GLfloat getY2() { return y2; }
        void setY2(const GLfloat y2) { this->y2 = y2; }

        GLfloat* getColor() { return color; }
        void setColor(const GLfloat* color) { for (int i = 0; i < 3; i++) this->color[i] = color[i]; }

        void drawSolidLine();
};

#endif