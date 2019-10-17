#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "player.h"
#include "arena.h"
#include "geometric_forms.h"

class Player;

class Projectile {
    private:
        Player* player      = nullptr;

        GLfloat cx          = 0.0;
        GLfloat cy          = 0.0;
        GLfloat angle       = 0.0;
        GLfloat speed       = 0.0;
        GLfloat multiplyer  = 1.0;

        GLfloat length      = 0.0;

    public:
        Projectile();
        ~Projectile();

        Player* getPlayer() { return player; }
        void setPlayer(Player* player) { this->player = player; }

        GLfloat getCx() { return cx; }
        void setCx(GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(GLfloat cy) { this->cy = cy; }

        GLfloat getAngle() { return angle; }
        void setAngle(GLfloat angle) { this->angle = angle; }

        GLfloat getSpeed() { return speed; }
        void setSpeed(GLfloat speed) { this->speed = speed; }

        GLfloat getMultiplyer() { return multiplyer; }
        void setMultiplyer(GLfloat multiplyer) { this->multiplyer = multiplyer; }

        GLfloat getLength() { return length; }
        void setLength(GLfloat length) { this->length = length; }

        void draw();
};

#endif