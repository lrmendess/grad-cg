#ifndef LINE_H
#define LINE_H

#include <GL/glut.h>

class Line {
    private:
        int x1;
        int y1;
        int x2;
        int y2;
        GLfloat color[3] = { 0, 0, 0 };

    public:
        ~Line();
        
        Line();
        Line(int x1, int y1, int x2, int y2, GLfloat* color);

        int getX1() { return x1; }
        void setX1(int x1) { this->x1 = x1; }

        int getY1() { return y1; }
        void setY1(int y1) { this->y1 = y1; }

        int getX2() { return x2; }
        void setX2(int x2) { this->x2 = x2; }

        int getY2() { return y2; }
        void setY2(int y2) { this->y2 = y2; }

        GLfloat* getColor() { return color; }
        void setColor(GLfloat* color) { for (int i = 0; i < 3; i++) this->color[i] = color[i]; }

        void drawSolidLine();
};

#endif