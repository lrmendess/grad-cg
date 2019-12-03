#include "projectile.h"

Projectile::Projectile(Player* player, GLfloat mul, GLfloat mulVelAirplane) {
    this->speed = player->getSpeed() * mul * mulVelAirplane;
    this->radius = player->getRadius() / 8;

    GLfloat airplaneFiRad = player->getAngle() * M_PI / 180;
    GLfloat airplaneThetaRad = player->getAngleTheta() * M_PI / 180;
    
    GLfloat cannonFiRad = player->getCannonAngle() * M_PI / 180;
    GLfloat cannonThetaRad = player->getCannonAngleTheta() * M_PI / 180;

    GLfloat px = player->getCx();
    px += player->getRadius() * cos(airplaneThetaRad) * cos(airplaneFiRad);
    px += player->getRadius() / 2 * cos(cannonThetaRad + airplaneThetaRad) * cos(cannonFiRad + airplaneFiRad);
    
    GLfloat py = player->getCy();
    py += player->getRadius() * cos(airplaneThetaRad) * sin(airplaneFiRad);
    py += player->getRadius() / 2 * cos(cannonThetaRad + airplaneThetaRad) * sin(cannonFiRad + airplaneFiRad);

    GLfloat pz = player->getCz();
    pz += player->getRadius() * sin(airplaneThetaRad);
    pz += player->getRadius() / 2 * sin(cannonThetaRad);

    this->cx = px;
    this->cy = py;
    this->cz = pz;

    this->angle = player->getAngle() + player->getCannonAngle();
    this->angleTheta = player->getAngleTheta() + player->getCannonAngleTheta();
}

Projectile::Projectile(Enemy* enemy, GLfloat mul, GLfloat mulVelAirplane) {
    this->speed = enemy->getSpeed() * mul * mulVelAirplane;
    this->radius = enemy->getRadius() / 8;

    GLfloat airplaneFiRad = enemy->getAngle() * M_PI / 180;
    GLfloat airplaneThetaRad = enemy->getAngleTheta() * M_PI / 180;
    
    GLfloat cannonFiRad = enemy->getCannonAngle() * M_PI / 180;
    GLfloat cannonThetaRad = 0.0;

    GLfloat px = enemy->getCx();
    px += enemy->getRadius() * cos(airplaneThetaRad) * cos(airplaneFiRad);
    px += enemy->getRadius() / 2 * cos(cannonThetaRad + airplaneThetaRad) * cos(cannonFiRad + airplaneFiRad);
    
    GLfloat py = enemy->getCy();
    py += enemy->getRadius() * cos(airplaneThetaRad) * sin(airplaneFiRad);
    py += enemy->getRadius() / 2 * cos(cannonThetaRad + airplaneThetaRad) * sin(cannonFiRad + airplaneFiRad);

    GLfloat pz = enemy->getCz();
    pz += enemy->getRadius() * sin(airplaneThetaRad);
    pz += enemy->getRadius() / 2 * sin(cannonThetaRad);

    this->cx = px;
    this->cy = py;
    this->cz = pz;

    this->angle = enemy->getAngle() + enemy->getCannonAngle();
    this->angleTheta = enemy->getAngleTheta() + 0.0;
}

Projectile::~Projectile() {

}

void Projectile::draw(GLuint projTexture) {
    glColor3f(1.0, 1.0, 1.0);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, projTexture);
    
    GLUquadricObj* obj = gluNewQuadric();
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluQuadricTexture(obj, GLU_TRUE);
        gluQuadricOrientation(obj, GLU_OUTSIDE);
        gluSphere(obj, this->radius, 16, 16);
        gluDeleteQuadric(obj);
        
    glDisable(GL_TEXTURE_2D);
}
