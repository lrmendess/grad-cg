#ifndef CURSOR_H
#define CURSOR_H

#include "circle.h"
#include "color.h"

class Cursor : public Circle {
    private:
        Color* overlapColor;
        bool rightButton = false;
        bool visible = false;

    public:
        Cursor(float radius, float x, float y, Color& color, Color& overlapColor);

        ~Cursor();

        Color& getOverlapColor();
        void setOverlapColor(Color& overlapColor);

        bool rightButtonIsPressed();
        void pressRightButton();
        void unpressRightButton();

        bool isVisible();
        bool makeVisible();
};

#endif