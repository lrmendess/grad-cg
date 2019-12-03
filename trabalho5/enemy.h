#ifndef ENEMY_H
#define ENEMY_H

#include <GL/glut.h>
#include <cmath>
#include <list>
#include "line.h"
#include "circle.h"
#include "arena.h"
#include "projectile.h"
#include "player.h"

#define d2p(x1, y1, x2, y2) sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))

class Arena;
class Projectile;
class Bomb;
class Player;

class Enemy {
    private:
        Arena* arena            = nullptr;
        GLfloat cx              = 0.0;
        GLfloat cy              = 0.0;
        GLfloat cz              = 0.0; //
        GLfloat radius          = 0.0;

        /* Inicialmente o aviao esta no chao */
        GLfloat speed           = 0.0;
        GLfloat angle           = 0.0;
        GLfloat angleTheta      = 0.0; //
        GLboolean dead          = false;
        GLfloat startX          = 0.0;
        GLfloat startY          = 0.0;
        GLfloat startZ          = 0.0; //

        /* Angulo das helices do aviao */
        GLfloat propellerAngle  = 0.0;

        /* Canhao */
        GLfloat cannonAngle     = 0.0;

        /* Tempo para disparo */
        GLfloat maxTimeFire = 0.0;
        GLfloat currentTimeFire = 0.0;

        /* Projeteis */
        list<Projectile*> projectiles;
        
    public:
        ~Enemy();
        Enemy(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius);

        GLfloat getCx() { return cx; }
        void setCx(GLfloat cx) { this->cx = cx; }

        GLfloat getCy() { return cy; }
        void setCy(GLfloat cy) { this->cy = cy; }

        GLfloat getCz() { return cz; }
        void setCz(GLfloat cz) { this->cz = cz; }

        GLfloat getAngle() { return angle; }
        void setAngle(GLfloat angle) { this->angle = angle; }

        GLfloat getAngleTheta() { return angleTheta; }
        void setAngleTheta(GLfloat angleTheta) { this->angleTheta = angleTheta; }

        GLfloat getRadius() { return radius; }
        void setRadius(GLfloat radius) { this->radius = radius; }
        
        GLfloat getSpeed() { return speed; }
        void setSpeed(GLfloat speed) { this->speed = speed; }

        GLfloat getCannonAngle() { return cannonAngle; }
        void setCannonAngle(GLfloat cannonAngle) { this->cannonAngle = cannonAngle; }

        GLboolean isDead() { return dead; }

        void moveX(GLfloat angle, GLfloat dt);
        void moveZ(GLfloat angle, GLfloat dt);
        void move(GLfloat mul, GLfloat dt);

        void fire(GLfloat mul, GLfloat mulVelAirplane, GLfloat dt);
        void setFireFreq(GLfloat fireFreq) { this->maxTimeFire = 1.0 / fireFreq; }

        void kill();

        /* Funcoes referentes a decolagem */
        void updateProjectiles(GLfloat dt);

        void drawWings();
        void drawCannon();
        void drawProjectiles(GLuint projTexture);
        void drawFuselage(GLuint airEnemiesTexture);
        void drawHourglass(GLfloat length);
        void drawLeftPropeller();
        void drawRightPropeller();
        void drawFin();
        void drawCockpit();
        void drawAirplane(GLuint airEnemiesTexture, GLuint projTexture);

        void reset();
};

#endif