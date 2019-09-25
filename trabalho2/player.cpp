#include "player.h"

Player::Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color) :
    Circle(cx, cy, radius, color) {
    
    // Velocidade de testes
    this->speed = 1;
    // Inicialmente o aviao esta no chao
    this->flying = false;
    this->takeOff = false;
    // Referencia da arena que o aviao esta
    this->arena = arena;
    // Armazenamento do estado inicial antes da decolagem
    this->startX = cx;
    this->startY = cy;
    this->startR = radius;

    this->oldRadiusTime = 0.0;
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

        if ((distanceFromEnemy <= (enemy->getRadius() + this->getRadius())) && flying) {
            return;
        }
    }

    this->setCx(cx);
}

void Player::moveY(const GLfloat& mul) {
    GLfloat cy = this->getCy() + (mul * speed);
    GLfloat cx = this->getCx();

    GLfloat distanceFromBorder = d2p(cx, cy, arena->getCx(), arena->getCy());

    if ((distanceFromBorder + this->getRadius()) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(cx, cy, enemy->getCx(), enemy->getCy());

        if ((distanceFromEnemy <= (enemy->getRadius() + this->getRadius())) && flying) {
            return;
        }
    }

    this->setCy(cy);
}

// S = So + Vo * t + (a * t^2)/2
void Player::calculatePhysics() {
    Line* airstrip = &arena->getAirstrip();

    /* Tratamento da decolagem do inicio ao fim da pista */
    // a = 2 * s / t^2
    GLfloat t = 4.0;

    GLfloat dy = airstrip->getY2() - airstrip->getY1();
    this->ay = 2 * dy / pow(t, 2);

    GLfloat dx = airstrip->getX2() - airstrip->getX1();
    this->ax = 2 * dx / pow(t, 2);

    /* Tratamento do inicio do crescimento do raio do player */
    // t = sqrt(2 * (S - So)/a)
    GLfloat midAirstripX = airstrip->getX1() + (airstrip->getX2() - airstrip->getX1()) / 2;
    GLfloat midAirstripY = airstrip->getY1() + (airstrip->getY2() - airstrip->getY1()) / 2;

    GLfloat dr = d2p(airstrip->getX1(), airstrip->getY1(), airstrip->getX2(), airstrip->getY2());
    GLfloat dro = d2p(airstrip->getX1(), airstrip->getY1(), midAirstripX, midAirstripY);
    GLfloat ar = 2 * dr / pow(t, 2);
    GLfloat increaseRadiusTime = sqrt(2 * (dr - dro) / ar);

    this->ar = 2 * this->getRadius() / pow(t - increaseRadiusTime, 2);
}

void Player::takeOffAirplane(GLint& currentTime) {
    Line* airstrip = &arena->getAirstrip();

    this->setCy(this->startY + (ay * pow(currentTime / 1000.0, 2)) / 2);
    this->setCx(this->startX + (ax * pow(currentTime / 1000.0, 2)) / 2);

    GLfloat midAirstripX = airstrip->getX1() + (airstrip->getX2() - airstrip->getX1()) / 2;
    GLfloat midAirstripY = airstrip->getY1() + (airstrip->getY2() - airstrip->getY1()) / 2;

    if ((this->getCx() >= midAirstripX) && (this->getCy() >= midAirstripY)) {
        this->setRadius(this->startR + (ar * pow((currentTime - oldRadiusTime) / 1000.0, 2)) / 2);
    } else {
        this->oldRadiusTime = currentTime;
    }
}
