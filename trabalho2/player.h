#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <cmath>
#include "line.h"
#include "circle.h"
#include "arena.h"

#define d2p(x1, y1, x2, y2) sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))

class Arena;

class Player : public Circle {
    private:
        Arena* arena;

        GLfloat speed;
        GLboolean flying;
        GLboolean takeOff;

        /* Posicoes iniciais do player e raio inicial*/
        GLfloat startX;
        GLfloat startY;
        GLfloat startR;

        /* Aceleracoes decompostas do player */
        GLfloat ax;
        GLfloat ay;
        GLfloat ar;

        /* Momento no tempo em que o raio deve comecar a crescer */
        GLfloat oldRadiusTime;
        
    public:
        Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color);

        GLfloat& getSpeed() { return speed; }
        void setSpeed(const GLfloat& speed) { this->speed = speed; }

        GLboolean& isFlying() { return flying; }
        void setFlying(GLboolean value) { this->flying = value; }

        GLboolean& isTakeOff() { return takeOff; }
        void setTakeOff(GLboolean value) { this->takeOff = value; }

        void moveX(const GLfloat& mul);
        void moveY(const GLfloat& mul);

        /* Funcoes referentes a decolagem */
        void calculatePhysics();
        void takeOffAirplane(GLint& currentTime);
};

#endif