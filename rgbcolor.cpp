#include "rgbcolor.h"

RGBColor::RGBColor(float red, float green, float blue) : red(red), green(green), blue(blue) {

}

float RGBColor::getRed() {
    return red;
}

float RGBColor::getGreen() {
    return green;
}

float RGBColor::getBlue() {
    return blue;
}

void RGBColor::setRed(float red) {
    this->red = red;
}

void RGBColor::setGreen(float green) {
    this->green = green;
}

void RGBColor::setBlue(float blue) {
    this->blue = blue;
}