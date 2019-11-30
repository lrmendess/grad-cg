#ifndef LINE_H
#define LINE_H

#include <GL/glut.h>

class Line {
    private:
        GLfloat x1;
        GLfloat y1;
        GLfloat x2;
        GLfloat y2;

    public:
        ~Line();
        
        Line();
        Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

        GLfloat getX1() { return x1; }
        GLfloat getY1() { return y1; }
        GLfloat getX2() { return x2; }
        GLfloat getY2() { return y2; }

        void drawSolidLine();
};

#endif