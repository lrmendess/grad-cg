#include "projectile.h"

Projectile::Projectile(Player* player, GLfloat mul, GLfloat mulVelAirplane, GLuint texture) {
    this->speed = player->getSpeed() * mul * mulVelAirplane;
    this->radius = player->getRadius() / 8;

    GLfloat airplaneAngleInRadians = player->getAngle() * M_PI / 180;
    GLfloat cannonAngleInRadians = player->getCannonAngle() * M_PI / 180;

    GLfloat px = player->getCx();
    px += player->getRadius() * cos(airplaneAngleInRadians);
    px += player->getRadius() / 2 * cos(cannonAngleInRadians + airplaneAngleInRadians);
    
    GLfloat py = player->getCy();
    py += player->getRadius() * sin(airplaneAngleInRadians);
    py += player->getRadius() / 2 * sin(cannonAngleInRadians + airplaneAngleInRadians);

    this->cx = px;
    this->cy = py;
    this->angle = player->getAngle() + player->getCannonAngle();
    
    this->texture = texture;

}

Projectile::Projectile(Enemy* enemy, GLfloat mul, GLfloat mulVelAirplane, GLuint texture) {
    this->speed = enemy->getSpeed() * mul * mulVelAirplane;
    this->radius = enemy->getRadius() / 8;

    GLfloat airplaneAngleInRadians = enemy->getAngle() * M_PI / 180;
    GLfloat cannonAngleInRadians = enemy->getCannonAngle() * M_PI / 180;

    GLfloat px = enemy->getCx();
    px += enemy->getRadius() * cos(airplaneAngleInRadians);
    px += enemy->getRadius() / 2 * cos(cannonAngleInRadians + airplaneAngleInRadians);
    
    GLfloat py = enemy->getCy();
    py += enemy->getRadius() * sin(airplaneAngleInRadians);
    py += enemy->getRadius() / 2 * sin(cannonAngleInRadians + airplaneAngleInRadians);

    this->cx = px;
    this->cy = py;
    this->angle = enemy->getAngle() + enemy->getCannonAngle();
    
    this->texture = texture;

}

Projectile::~Projectile() {

}

void Projectile::draw() {
    glColor3f(1,1,1);
    
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, this->texture);
        
        GLUquadricObj* obj = gluNewQuadric();
        gluQuadricDrawStyle(obj, GLU_FILL);
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluQuadricTexture(obj, GLU_TRUE);
        gluQuadricOrientation(obj, GLU_INSIDE);

        gluSphere(obj, this->radius, 360, 360);
        gluDeleteQuadric(obj);
    glPopMatrix();
}