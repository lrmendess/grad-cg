#include "circle.h"

Circle::Circle(float radius, float x, float y) : radius(radius), x(x), y(y) {

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