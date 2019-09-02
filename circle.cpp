#include "circle.h"

Circle::Circle(float radius, float x, float y, Color& color)
    : radius(radius), x(x), y(y), color(color) {

}

Circle::~Circle() {
    
}

float Circle::getRadius() {
    return this->radius;
}

void Circle::setRadius(float radius) {
    this->radius = radius;
}

float Circle::getX() {
    return this->x;
}

void Circle::setX(float x) {
    this->x = x;
}

float Circle::getY() {
    return this->y;
}

void Circle::setY(float y) {
    this->y = y;
}

Color Circle::getColor() {
    return this->color;
}

void Circle::setColor(Color color) {
    this->color = color;
}

void Circle::draw() {
    glColor3f(color.getRed(), color.getGreen(), color.getBlue());
    
    glBegin(GL_LINE_LOOP);
        float angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = this->x + (cos(angle) * this->radius);
            py = this->y + (sin(angle) * this->radius);
            glVertex2f(px, py);
        }
    glEnd();
}