#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "player.h"
#include "arena.h"
#include "enemy.h"

class Player;
class Enemy;

class Projectile {
    private:
        GLfloat cx          = 0.0;
        GLfloat cy          = 0.0;
        GLfloat cz          = 0.0;
        GLfloat angle       = 0.0;
        GLfloat angleTheta  = 0.0;
        GLfloat speed       = 0.0;
        GLfloat radius      = 0.0;
        GLfloat cannonLength= 0.0;

    public:
        /* FOR DRAW */
        GLfloat airplaneX   = 0.0;
        GLfloat airplaneY   = 0.0;
        GLfloat airplaneZ   = 0.0;
        GLfloat airplaneTheta = 0.0;
        GLfloat airplaneFi  = 0.0;
        GLfloat cannonTheta = 0.0;
        GLfloat cannonFi    = 0.0;

        Projectile(Player* player, GLfloat mul, GLfloat mulVelAirplane);
        Projectile(Enemy* enemy, GLfloat mul, GLfloat mulVelAirplane);
        ~Projectile();

        GLfloat getCx() { return cx; }
        void setCx(GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(GLfloat cy) { this->cy = cy; }

        GLfloat getCz() { return cz; }
        void setCz(GLfloat cz) { this->cz = cz; }

        GLfloat getAngle() { return angle; }
        GLfloat getAngleTheta() { return angleTheta; }
        GLfloat getSpeed() { return speed; }
        GLfloat getCannonLength() { return cannonLength; }
        GLfloat getRadius() { return radius; }

        void draw(GLuint projTexture);
};

#endif