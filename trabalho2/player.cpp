#include "player.h"

Player::Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color) :
    Circle(cx, cy, radius, color) {
    
    // Velocidade de testes
    this->speed = 1;
    // Inicialmente o aviao esta no chao
    this->flying = false;
    // Referencia da arena que o aviao esta
    this->arena = arena;
}

void Player::moveX(const GLfloat& mul) {
    GLfloat cx = this->getCx() + (mul * speed);
    GLfloat cy = this->getCy();

    GLfloat distanceFromBorder = sqrt(pow(cx - arena->getCx(), 2) + pow(cy - arena->getCy(), 2));

    if ((distanceFromBorder + this->getRadius()) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = sqrt(pow(cx - enemy->getCx(), 2) + pow(cy - enemy->getCy(), 2));

        if (distanceFromEnemy <= (enemy->getRadius() + this->getRadius())) {
            return;
        }
    }

    this->setCx(cx);
}

void Player::moveY(const GLfloat& mul) {
    GLfloat cy = this->getCy() + (mul * speed);
    GLfloat cx = this->getCx();

    GLfloat distanceFromBorder = sqrt(pow(cx - arena->getCx(), 2) + pow(cy - arena->getCy(), 2));

    if ((distanceFromBorder + this->getRadius()) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distance = sqrt(pow(cx - enemy->getCx(), 2) + pow(cy - enemy->getCy(), 2));

        if (distance <= (enemy->getRadius() + this->getRadius())) {
            return;
        }
    }

    this->setCy(cy);
}
