#include "color.h"
#include <iostream>

Color::Color(float red, float green, float blue, float alpha) 
    : red(red), green(green), blue(blue), alpha(alpha) {
        
}

Color::~Color() {
    
}

float Color::getRed() {
    return this->red;
}

float Color::getGreen() {
    return this->green;
}

float Color::getBlue() {
    return this->blue;
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

float Color::getAlpha() {
    return this->alpha;
}

void Color::setAlpha(float alpha) {
    this->alpha = alpha;
}