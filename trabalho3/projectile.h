#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "player.h"
#include "arena.h"

class Player;

class Projectile {
    private:
        GLfloat cx          = 0.0;
        GLfloat cy          = 0.0;
        GLfloat angle       = 0.0;
        GLfloat speed       = 0.0;
        GLfloat radius      = 0.0;

    public:
        Projectile(Player* player, GLfloat mul);
        ~Projectile();

        GLfloat getCx() { return cx; }
        void setCx(GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(GLfloat cy) { this->cy = cy; }

        GLfloat getAngle() { return angle; }

        GLfloat getSpeed() { return speed; }

        void draw();
};

#endif