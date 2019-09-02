#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glut.h>
#include <math.h>
#include "color.h"

class Circle {
    private:
        float radius;
        float x;
        float y;

        Color color;

    public:
        Circle(float radius, float x, float y, Color color);
        
        float getRadius();
        void setRadius(float radius);

        float getX();
        void setX(float x);

        float getY();
        void setY(float y);

        Color getColor();
        void setColor(Color color);

        void draw();
};

#endif