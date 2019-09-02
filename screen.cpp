#include "screen.h"

Screen::Screen(int width, int height, Color color, string title) 
    : width(width), height(height), color(color), title(title) {

}

int Screen::getWidth() {
    return this->width;
}

void Screen::setWidth(int width) {
    this->width = width;
}

int Screen::getHeight() {
    return this->height;
}

void Screen::setHeight(int height) {
    this->height = height;
}

Color Screen::getColor() {
    return this->color;
}

void Screen::setColor(Color color) {
    this->color = color;
}

string Screen::getTitle() {
    return this->title;
}

void Screen::setTitle(string title) {
    this->title = title;
}