#include "cursor.h"

Cursor::Cursor(float radius, float x, float y, Color& color, Color& overlapColor)
    : Circle(radius, x, y, color), overlapColor(&overlapColor) {

}

Cursor::~Cursor() {

}

Color& Cursor::getOverlapColor() {
    return *this->overlapColor;
}

void Cursor::setOverlapColor(Color& overlapColor) {
    this->overlapColor = &overlapColor;
}

bool Cursor::rightButtonIsPressed() {
    return this->rightButton;
}

void Cursor::pressRightButton() {
    this->rightButton = true;
}

void Cursor::unpressRightButton() {
    this->rightButton = false;
}
