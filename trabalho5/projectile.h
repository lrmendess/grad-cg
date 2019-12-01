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
        GLfloat angle       = 0.0;
        GLfloat speed       = 0.0;
        GLfloat radius      = 0.0;
        
        GLuint texture;

    public:
        Projectile(Player* player, GLfloat mul, GLfloat mulVelAirplane, GLuint texture);
        Projectile(Enemy* enemy, GLfloat mul, GLfloat mulVelAirplane, GLuint texture);
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