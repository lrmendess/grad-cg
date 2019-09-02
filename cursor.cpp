#include "cursor.h"

Cursor::Cursor(float radius, float x, float y, Color& color, Color& overlapColor)
    : Circle(radius, x, y, color), overlapColor(overlapColor) {

}

Cursor::~Cursor() {

}

Color Cursor::getOverlapColor() {
    return this->overlapColor;
}

void Cursor::setOverlapColor(Color overlapColor) {
    this->overlapColor = overlapColor;
}