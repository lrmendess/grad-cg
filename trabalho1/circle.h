#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glut.h>
#include <math.h>
#include "color.h"

class Circle {
    private:
        float radius;
        int x;
        int y;

        Color* color;

    public:
        Circle(float radius, int x, int y, Color& color);

        ~Circle();
        
        float getRadius();
        void setRadius(float radius);

        int getX();
        void setX(int x);

        int getY();
        void setY(int y);

        Color& getColor();
        void setColor(Color& color);

        void draw();
        void draw(Color& color);
        void drawSolid();
        void drawSolid(Color& color);
};

#endif