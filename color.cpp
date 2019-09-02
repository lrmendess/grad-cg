#include "color.h"

Color::Color(float red, float green, float blue) : red(red), green(green), blue(blue) {

}

float Color::getRed() {
    return red;
}

float Color::getGreen() {
    return green;
}

float Color::getBlue() {
    return blue;
}

void Color::setRed(float red) {
    this->red = red;
}

void Color::setGreen(float green) {
    this->green = green;
}

void Color::setBlue(float blue) {
    this->blue = blue;
}