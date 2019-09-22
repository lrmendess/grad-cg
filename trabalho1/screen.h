#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include "color.h"

using namespace std;

class Screen {
    private:
        int width;
        int height;
        Color* color;
        string title;

    public:
        Screen(int width, int height, Color& color, string title);

        int getWidth();
        void setWidth(int width);

        int getHeight();
        void setHeight(int height);

        Color& getColor();
        void setColor(Color& color);

        string getTitle();
        void setTitle(string title);
};

#endif