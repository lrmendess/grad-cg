#include "projectile.h"

Projectile::Projectile(Player* player, GLfloat mul, GLfloat mulVelAirplane) {
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

}

Projectile::Projectile(Enemy* enemy, GLfloat mul, GLfloat mulVelAirplane) {
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

}

Projectile::~Projectile() {

}

void Projectile::draw() {
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