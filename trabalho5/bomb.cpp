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

void Bomb::draw(GLuint bombTexture) {    
    glEnable(GL_TEXTURE_2D);
    
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, bombTexture);
    
    GLUquadricObj* obj = gluNewQuadric();
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluQuadricTexture(obj, GLU_TRUE);
        gluQuadricOrientation(obj, GLU_OUTSIDE);
        gluSphere(obj, this->radius, 16, 16);
        gluDeleteQuadric(obj);
        
    glDisable(GL_TEXTURE_2D);
}
