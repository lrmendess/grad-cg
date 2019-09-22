#include "circle.h"

Circle::Circle(float radius, int x, int y, Color& color)
    : radius(radius), x(x), y(y), color(&color) {

}

Circle::~Circle() {
    
}

float Circle::getRadius() {
    return this->radius;
}

void Circle::setRadius(float radius) {
    this->radius = radius;
}

int Circle::getX() {
    return this->x;
}

void Circle::setX(int x) {
    this->x = x;
}

int Circle::getY() {
    return this->y;
}

void Circle::setY(int y) {
    this->y = y;
}

Color& Circle::getColor() {
    return *this->color;
}

void Circle::setColor(Color& color) {
    this->color = &color;
}

void Circle::draw() {
    glColor3f(color->getRed(), color->getGreen(), color->getBlue());
    
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

void Circle::draw(Color& color) {
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

void Circle::drawSolid() {
    glColor3f(color->getRed(), color->getGreen(), color->getBlue());
    
    glBegin(GL_POLYGON);
        float angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = this->x + (cos(angle) * this->radius);
            py = this->y + (sin(angle) * this->radius);
            glVertex2f(px, py);
        }
    glEnd();
}

void Circle::drawSolid(Color& color) {
    glColor3f(color.getRed(), color.getGreen(), color.getBlue());
    
    glBegin(GL_POLYGON);
        float angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = this->x + (cos(angle) * this->radius);
            py = this->y + (sin(angle) * this->radius);
            glVertex2f(px, py);
        }
    glEnd();
}