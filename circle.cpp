#include "circle.h"

Circle::Circle(float radius, float x, float y, RGBColor color) : radius(radius), x(x), y(y), color(color) {

}

float Circle::getRadius() {
    return radius;
}

void Circle::setRadius(float radius) {
    this->radius = radius;
}

float Circle::getX() {
    return x;
}

void Circle::setX(float x) {
    this->x = x;
}

float Circle::getY() {
    return y;
}

void Circle::setY(float y) {
    this->y = y;
}

RGBColor Circle::getColor() {
    return color;
}

void Circle::setColor(RGBColor color) {
    this->color = color;
}

void Circle::draw() {
    glColor3f(color.getRed(), color.getGreen(), color.getBlue());

    float angle, px, py;
    
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = this->x + (cos(angle) * this->radius);
            py = this->y + (sin(angle) * this->radius);
            glVertex2f(px, py);
        }
    glEnd();
}