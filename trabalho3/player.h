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
        Arena* arena        = nullptr;
        GLfloat cx          = 0.0;
        GLfloat cy          = 0.0;
        GLfloat radius      = 0.0;
        GLfloat color[3]    = { 0.0, 1.0, 0.0 };

        /* Inicialmente o aviao esta no chao */
        GLfloat speed       = 0.0;
        GLfloat angle       = 0.0;
        GLboolean flying    = false;
        GLboolean takeOff   = false;
        GLboolean dead      = false;
        GLfloat startX      = 0.0;
        GLfloat startY      = 0.0;
        GLfloat startR      = 0.0;
        GLfloat ax          = 0.0;
        GLfloat ay          = 0.0;

        /* Ponto onde o aviao deve levantar voo */
        GLfloat midAirstripX    = 0.0;
        GLfloat midAirstripY    = 0.0;
        GLfloat midAirstripTime = 0.0;

        /* Dados do raio */
        GLfloat radiusSpeed     = 0.0;
        GLfloat oldRadiusTime   = 0.0;

        /* Angulo das helices do aviao */
        GLfloat propellerAngle  = 0.0;

        GLfloat cannonAngle     = 0.0;
        
    public:
        Player(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius, const GLfloat* color);

        GLfloat getCx() { return cx; }
        void setCx(GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(GLfloat cy) { this->cy = cy; }

        GLfloat getAngle() { return angle; }
        void setAngle(GLfloat angle) { this->angle = angle; }

        GLfloat getRadius() { return radius; }
        void setRadius(GLfloat radius) { this->radius = radius; }

        GLfloat* getColor() { return color; }
        void setColor(const GLfloat* color) { for (int i = 0; i < 3; i++) this->color[i] = color[i]; }

        GLfloat getSpeed() { return speed; }
        void setSpeed(GLfloat speed) { this->speed = speed; }

        GLboolean isFlying() { return flying; }
        void setFlying(GLboolean value) { this->flying = value; }

        GLboolean isTakeOff() { return takeOff; }
        void setTakeOff(GLboolean value) { this->takeOff = value; }

        GLboolean isDead() { return dead; }
        void setDead(GLboolean value) { this->dead = value; }

        void moveX(GLfloat angle, GLfloat dt);
        void move(GLfloat mulX, GLfloat mulY, GLfloat dt);

        /* Funcoes referentes a decolagem */
        void calculatePhysics();
        void takeOffAirplane(GLint currentTime);

        void drawWings();
        void drawCannon();
        void drawFuselage();
        void drawTriangles(GLfloat length);
        void drawLeftPropeller();
        void drawRightPropeller();
        void drawFin();
        void drawCockpit();
        void drawAirplane();

        void reset();
};

#endif