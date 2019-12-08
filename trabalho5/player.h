#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <cmath>
#include <list>
#include "line.h"
#include "circle.h"
#include "arena.h"
#include "projectile.h"
#include "bomb.h"

#define CANNON_LENGTH this->radius / 4

#define d2p(x1, y1, x2, y2) sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2))
#define d2p3d(x1, y1, z1, x2, y2, z2) sqrt(pow(x1 - x2, 2) + pow (y1 - y2, 2) + pow(z1 - z2, 2))

class Arena;
class Projectile;
class Bomb;

class Player {
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
        GLboolean flying        = false;
        GLboolean takeOff       = false;
        GLboolean dead          = false;
        GLfloat startX          = 0.0;
        GLfloat startY          = 0.0;
        GLfloat startZ          = 0.0; //
        GLfloat startR          = 0.0;
        GLfloat ax              = 0.0;
        GLfloat ay              = 0.0;
        GLfloat az              = 0.0;

        /* Ponto onde o aviao deve levantar voo */
        GLfloat midAirstripX    = 0.0;
        GLfloat midAirstripY    = 0.0;
        GLfloat midAirstripTime = 0.0;

        /* Dados do raio */
        GLfloat oldTime   = 0.0;

        /* Angulo das helices do aviao */
        GLfloat propellerAngle  = 0.0;

        /* Canhao */
        GLfloat cannonAngle     = 0.0;
        GLfloat cannonAngleTheta= 0.0;
        GLfloat cannonLength    = 0.0;

        /* Ultima posicao registrada do mouse */
        GLfloat mouseX          = 0.0;
        GLfloat mouseY          = 0.0;
        GLfloat mouseZ          = 0.0;

        GLint points            = 0;

        /* Projeteis */
        list<Projectile*> projectiles;

        /* Bombas */
        list<Bomb*> bombs;
        GLboolean bombIsAvaliable = true;
        
    public:
        ~Player();
        Player(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius);

        GLint getPoints() { return points; }

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

        GLfloat getCannonAngleTheta() { return cannonAngleTheta; }
        void setCannonAngleTheta(GLfloat cannonAngleTheta) { this->cannonAngleTheta = cannonAngleTheta; }

        GLfloat getCannonLength() { return cannonLength; }
        void setCannonLength(GLfloat cannonLength) { this->cannonLength = cannonLength; }

        GLfloat getMouseX() { return mouseX; }
        void setMouseX(GLfloat mouseX) { this->mouseX = mouseX; }

        GLfloat getMouseY() { return mouseY; }
        void setMouseY(GLfloat mouseY) { this->mouseY = mouseY; }

        GLboolean isFlying() { return flying; }
        void setFlying(GLboolean value) { this->flying = value; }

        GLboolean isTakeOff() { return takeOff; }
        void setTakeOff(GLboolean value) { this->takeOff = value; }

        GLboolean isDead() { return dead; }
        
        list<Bomb*>& getBombs() { return this->bombs; };

        void moveX(GLfloat angle, GLfloat dt);
        void moveZ(GLfloat angle, GLfloat dt);
        void move(GLfloat mul, GLfloat dt);

        void fire(GLfloat mul, GLfloat mulVelAirplane);
        void bomb(GLfloat mulVelAirplane);

        void kill();

        /* Funcoes referentes a decolagem */
        void calculatePhysics();
        void takeOffAirplane(GLint currentTime, GLfloat diffTime);
        void updateProjectiles(GLfloat dt);
        void updateBombs(GLfloat currentTime, GLfloat dt);

        void drawWings();
        void drawCannon();
        void drawProjectiles(GLuint projTexture);
        void drawBombs(GLuint bombTexture);
        void drawFuselage(GLuint playerTexture);
        void drawHourglass(GLfloat length);
        void drawLeftPropeller();
        void drawRightPropeller();
        void drawFin();
        void drawCockpit();
        void drawAirplane(GLuint playerTexture, GLuint projTexture, GLuint bombTexture);

        void reset();
};

#endif