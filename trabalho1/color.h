#ifndef COLOR_H
#define COLOR_H

class Color {
    private:
        float red;
        float green;
        float blue;
        float alpha;

    public:
        Color(float red, float green, float blue, float alpha = 0.9);

        ~Color();
        
        float getRed();
        void setRed(float red);

        float getGreen();
        void setGreen(float green);
        
        float getBlue();
        void setBlue(float blue);

        float getAlpha();
        void setAlpha(float alpha);
};

#endif