#include "projectile.h"

Projectile::Projectile(Player* player, GLfloat mul, GLfloat mulVelAirplane) {
    this->radius = player->getCannonLength() / 3;
    this->cannonLength = player->getCannonLength();
    this->speed = player->getSpeed() * mul * mulVelAirplane;

    // GLfloat airplaneFiRad = player->getAngle() * M_PI / 180;
    // GLfloat airplaneThetaRad = player->getAngleTheta() * M_PI / 180;
    // 
    // GLfloat cannonFiRad = player->getCannonAngle() * M_PI / 180;
    // GLfloat cannonThetaRad = player->getCannonAngleTheta() * M_PI / 180;
    // 
    // GLfloat py = player->getCy();
    // py += player->getRadius() * cos(airplaneThetaRad) * sin(airplaneFiRad);
    // py += this->cannonLength * cos(airplaneThetaRad + cannonThetaRad) * sin(airplaneFiRad + cannonFiRad);
// 
    // GLfloat px = player->getCx();
    // px += player->getRadius() * cos(airplaneThetaRad) * cos(airplaneFiRad);
    // px += this->cannonLength * cos(airplaneThetaRad + cannonThetaRad) * cos(airplaneFiRad + cannonFiRad);
// 
    // GLfloat pz = player->getCz();
    // pz += player->getRadius() * sin(airplaneThetaRad);
    // pz += this->cannonLength * sin(cannonThetaRad + airplaneThetaRad);
// 
    // this->cy = py;
    // this->cx = px;
    // this->cz = pz;

    this->angle = player->getAngle() + player->getCannonAngle();
    this->angleTheta = player->getAngleTheta() + player->getCannonAngleTheta();

    /* FOR DRAW */
    this->airplaneX     = player->getCx();
    this->airplaneY     = player->getCy();
    this->airplaneZ     = player->getCz();
    this->airplaneTheta = player->getAngleTheta();
    this->airplaneFi    = player->getAngle();
    this->cannonTheta   = player->getCannonAngleTheta();
    this->cannonFi      = player->getCannonAngle();
}

Projectile::Projectile(Enemy* enemy, GLfloat mul, GLfloat mulVelAirplane) {
    this->radius = enemy->getCannonLength() / 3;
    this->cannonLength = enemy->getCannonLength();
    this->speed = enemy->getSpeed() * mul * mulVelAirplane;

    GLfloat airplaneFiRad = enemy->getAngle() * M_PI / 180;
    GLfloat airplaneThetaRad = enemy->getAngleTheta() * M_PI / 180;
    
    GLfloat cannonFiRad = enemy->getCannonAngle() * M_PI / 180;
    GLfloat cannonThetaRad = 0.0;

    GLfloat px = enemy->getCx();
    px += enemy->getRadius() * cos(airplaneThetaRad) * cos(airplaneFiRad);
    px += this->cannonLength * cos(cannonThetaRad + airplaneThetaRad) * cos(cannonFiRad + airplaneFiRad);
    
    GLfloat py = enemy->getCy();
    py += enemy->getRadius() * cos(airplaneThetaRad) * sin(airplaneFiRad);
    py += this->cannonLength * cos(cannonThetaRad + airplaneThetaRad) * sin(cannonFiRad + airplaneFiRad);

    GLfloat pz = enemy->getCz();
    pz += enemy->getRadius() * sin(airplaneThetaRad);
    pz += this->cannonLength * sin(cannonThetaRad);

    this->cx = px;
    this->cy = py;
    this->cz = pz;

    this->angle = enemy->getAngle() + enemy->getCannonAngle();
    this->angleTheta = enemy->getAngleTheta() + 0.0;
}

Projectile::~Projectile() {

}

void Projectile::draw(GLuint projTexture) {
    GLfloat projEmission[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat projColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, projEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, projColor);
    glColor3f(1.0, 1.0, 1.0);
    
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, projTexture);
        
        GLUquadricObj* proj = gluNewQuadric();
            gluQuadricDrawStyle(proj, GLU_FILL);
            gluQuadricNormals(proj, GLU_SMOOTH);
            gluQuadricTexture(proj, GLU_TRUE);
            gluQuadricOrientation(proj, GLU_OUTSIDE);
            gluSphere(proj, this->radius, 16, 16);
        gluDeleteQuadric(proj);
    glDisable(GL_TEXTURE_2D);
}
