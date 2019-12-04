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
    GLfloat bombEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat bombColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, bombEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bombColor);
    glColor3f(1.0, 1.0, 1.0);
    
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bombTexture);
        
        GLUquadricObj* bomb = gluNewQuadric();
            gluQuadricDrawStyle(bomb, GLU_FILL);
            gluQuadricNormals(bomb, GLU_SMOOTH);
            gluQuadricTexture(bomb, GLU_TRUE);
            gluQuadricOrientation(bomb, GLU_OUTSIDE);
            gluSphere(bomb, this->radius, 16, 16);
        gluDeleteQuadric(bomb);
    glDisable(GL_TEXTURE_2D);
}
