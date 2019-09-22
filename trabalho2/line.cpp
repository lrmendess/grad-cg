#include "line.h"

Line::~Line() {

}

Line::Line() {

}

Line::Line(int x1, int y1, int x2, int y2, GLfloat* color) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    for (int i = 0; i < 3; i++) {
        this->color[i] = color[i];
    }
}
