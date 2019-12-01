#include "bomb.h"

Bomb::Bomb(Player* player) {
    this->cx = player->getCx();
    this->cy = player->getCy();
    this->angle = player->getAngle();
    this->speed = player->getSpeed();
    this->radius = player->getRadius() / 4;
    this->startRadius = this->radius;
    this->radiusSpeed = (this->radius / 2 - this->radius) / 2.0;
    this->timeReleased = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

Bomb::~Bomb() {

}

void Bomb::draw() {
    glColor3f(0.0, 0.0, 0.0);
        
    glBegin(GL_POLYGON);    
        GLfloat a, px, py;

        for (int i = 0; i < 360; i += 18) {
            a = (i * M_PI) / 180.0;
            px = cos(a) * this->radius;
            py = sin(a) * this->radius;
            glVertex2f(px, py);
        }
    glEnd();
}