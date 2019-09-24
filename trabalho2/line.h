#ifndef LINE_H
#define LINE_H

#include <GL/glut.h>

class Line {
    private:
        GLint x1;
        GLint y1;
        GLint x2;
        GLint y2;
        GLfloat color[3] = { 0, 0, 0 };

    public:
        ~Line();
        
        Line();
        Line(GLint& x1, GLint& y1, GLint& x2, GLint& y2, GLfloat* color);

        GLint& getX1() { return x1; }
        void setX1(const GLint& x1) { this->x1 = x1; }

        GLint& getY1() { return y1; }
        void setY1(const GLint& y1) { this->y1 = y1; }

        GLint& getX2() { return x2; }
        void setX2(const GLint& x2) { this->x2 = x2; }

        GLint& getY2() { return y2; }
        void setY2(const GLint& y2) { this->y2 = y2; }

        GLfloat* getColor() { return color; }
        void setColor(const GLfloat* color) { for (GLint i = 0; i < 3; i++) this->color[i] = color[i]; }

        void drawSolidLine();
};

#endif