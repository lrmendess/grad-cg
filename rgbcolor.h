#ifndef RGBCOLOR_H
#define RGBCOLOR_H

class RGBColor {
    private:
        float red;
        float green;
        float blue;

    public:
        RGBColor(float red, float green, float blue);
        
        float getRed();
        float getGreen();
        float getBlue();

        void setRed(float red);
        void setGreen(float green);
        void setBlue(float blue);
};

#endif