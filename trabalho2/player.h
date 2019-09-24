#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include "line.h"
#include "circle.h"

class Player : public Circle {
    private:

    public:
        Player(const GLint& cx, const GLint& cy, const GLfloat& radius, const GLfloat* color);
};

#endif