#include "bomb.h"

Bomb::Bomb(Player* player, GLuint texture) {
    this->cx = player->getCx();
    this->cy = player->getCy();
    this->angle = player->getAngle();
    this->speed = player->getSpeed();
    this->radius = player->getRadius() / 4;
    this->startRadius = this->radius;
    this->radiusSpeed = (this->radius / 2 - this->radius) / 2.0;
    this->timeReleased = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    
    this->texture = texture;
}

Bomb::~Bomb() {

}

void Bomb::draw() {
    glColor3f(0.0, 0.0, 0.0);
    
    glBindTexture(GL_TEXTURE_2D, this->texture);
    
    glPushMatrix();
        glutSolidSphere(this->radius, 10, 10);
    glPopMatrix();
}