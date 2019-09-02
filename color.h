#ifndef COLOR_H
#define COLOR_H

class Color {
    private:
        float red;
        float green;
        float blue;

    public:
        Color(float red, float green, float blue);
        
        float getRed();
        void setRed(float red);

        float getGreen();
        void setGreen(float green);
        
        float getBlue();
        void setBlue(float blue);
};

#endif