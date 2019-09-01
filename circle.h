#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glut.h>
#include <math.h>
#include "rgbcolor.h"

class Circle {
    private:
        float radius;
        float x;
        float y;

        RGBColor color;

    public:
        Circle(float radius, float x, float y, RGBColor color);
        
        float getRadius();
        void setRadius(float radius);

        float getX();
        void setX(float x);

        float getY();
        void setY(float y);

        RGBColor getColor();
        void setColor(RGBColor color);

        void draw();
};

#endif