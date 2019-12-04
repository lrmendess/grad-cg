#include "bomb.h"

Bomb::Bomb(Player* player, GLfloat mulVelAirplane) {
    this->cx = player->getCx();
    this->cy = player->getCy();
    this->cz = player->getCz();
    this->angle = player->getAngle();
    this->speed = player->getSpeed() * mulVelAirplane;
    this->radius = player->getRadius() / 4;
    this->startRadius = this->radius;
    this->timeReleased = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

Bomb::~Bomb() {

}

void Bomb::draw(GLuint bombTexture) {     
    glColor3f(1.0, 1.0, 1.0); 
     
    glEnable(GL_TEXTURE_2D);
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
