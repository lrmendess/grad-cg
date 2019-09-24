#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <cmath>
#include "line.h"
#include "circle.h"
#include "arena.h"

class Arena;

class Player : public Circle {
    private:
        Arena* arena;
        GLfloat speed;
        GLboolean flying;
        
    public:
        Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color);

        GLfloat& getSpeed() { return speed; }
        void setSpeed(const GLfloat& speed) { this->speed = speed; }

        GLboolean& isFlying() { return flying; }
        void fly() { this->flying = true; }
        void land() { this->flying = false; }

        void moveX(const GLfloat& mul);
        void moveY(const GLfloat& mul);
};

#endif