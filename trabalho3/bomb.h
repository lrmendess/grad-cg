#ifndef BOMB_H
#define BOMB_H

#include "circle.h"
#include "player.h"

class Player;

class Bomb {
    private:
        GLfloat cx           = 0.0;
        GLfloat cy           = 0.0;
        GLfloat angle        = 0.0;
        GLfloat speed        = 0.0;
        GLfloat radius       = 0.0;
        GLfloat startRadius  = 0.0;
        GLfloat radiusSpeed  = 0.0;
        GLfloat timeReleased = 0.0;
        
    public:
        ~Bomb();
        Bomb(Player* player);

        GLfloat getCx() { return cx; }
        void setCx(GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(GLfloat cy) { this->cy = cy; }
        
        GLfloat getRadius() { return radius; }
        void setRadius(GLfloat radius) { this->radius = radius; }

        GLfloat getAngle() { return angle; }
        GLfloat getSpeed() { return speed; }
        GLfloat getStartRadius() { return startRadius; }
        GLfloat getRadiusSpeed() { return radiusSpeed; }
        GLfloat getTimeReleased() { return timeReleased; }

        void draw();
};

#endif