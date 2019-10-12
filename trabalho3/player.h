#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <cmath>
#include "geometric_forms.h"
#include "line.h"
#include "circle.h"
#include "arena.h"

#define d2p(x1, y1, x2, y2) sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))

class Arena;

class Player {
    private:
        GLfloat cx;
        GLfloat cy;
        GLfloat radius;
        GLfloat color[3] = { 0, 0, 0 };

        GLfloat angle;

        /* Referencia circular para a arena */
        Arena* arena;

        /* Diz se o aviao bateu ou nao */
        bool dead;

        /* Inicialmente o aviao esta no chao */
        GLfloat speed = 0;
        GLboolean flying = false;
        GLboolean takeOff = false;

        /* Posicoes iniciais do player e raio inicial*/
        GLfloat startX;
        GLfloat startY;
        GLfloat startR;

        /* Aceleracoes decompostas do player */
        GLfloat ax;
        GLfloat ay;

        /* Ponto onde o aviao deve levantar voo */
        GLfloat midAirstripX;
        GLfloat midAirstripY;

        /* Tempo que o aviao chega na metade da pista */
        GLfloat midAirstripTime;

        /* Velocidade de crescimento do raio */
        GLfloat radiusSpeed = 0.0;

        /* Momento no tempo em que o raio deve comecar a crescer */
        GLfloat oldRadiusTime = 0.0;
        
    public:
        Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color);

        GLfloat& getCx() { return cx; }
        void setCx(const GLfloat& cx) { this->cx = cx; }

        GLfloat& getCy() { return cy; }
        void setCy(const GLfloat& cy) { this->cy = cy; }

        GLfloat& getAngle() { return angle; }
        void setAngle(const GLfloat& angle) { this->angle = angle; }

        GLfloat& getRadius() { return radius; }
        void setRadius(const GLfloat& radius) { this->radius = radius; }

        GLfloat* getColor() { return color; }
        void setColor(const GLfloat* color) { for (int i = 0; i < 3; i++) this->color[i] = color[i]; }

        GLfloat& getSpeed() { return speed; }
        void setSpeed(const GLfloat& speed) { this->speed = speed; }

        GLboolean& isFlying() { return flying; }
        void setFlying(GLboolean value) { this->flying = value; }

        GLboolean& isTakeOff() { return takeOff; }
        void setTakeOff(GLboolean value) { this->takeOff = value; }

        void moveX(const GLfloat& angle, const GLfloat& dt);
        void move(const GLfloat& mulX, const GLfloat& mulY, const GLfloat& dt);

        /* Funcoes referentes a decolagem */
        void calculatePhysics();
        void takeOffAirplane(GLint& currentTime);

        void drawAirplane();
};

#endif