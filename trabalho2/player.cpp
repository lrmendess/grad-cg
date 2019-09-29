#include "player.h"

Player::Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color) :
    Circle(cx, cy, radius, color) {

    // Referencia da arena que o aviao esta
    this->arena = arena;
    // Armazenamento do estado inicial antes da decolagem
    this->startX = cx;
    this->startY = cy;
    this->startR = radius;
}

void Player::moveX(const GLfloat& mul) {
    GLfloat cx = getCx() + (mul * speed);
    GLfloat cy = getCy();

    GLfloat distanceFromBorder = sqrt(pow(cx - arena->getCx(), 2) + pow(cy - arena->getCy(), 2));

    if ((distanceFromBorder + getRadius()) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = sqrt(pow(cx - enemy->getCx(), 2) + pow(cy - enemy->getCy(), 2));

        if ((distanceFromEnemy <= (enemy->getRadius() + getRadius())) && flying) {
            return;
        }
    }

    setCx(cx);
}

void Player::moveY(const GLfloat& mul) {
    GLfloat cy = getCy() + (mul * speed);
    GLfloat cx = getCx();

    GLfloat distanceFromBorder = d2p(cx, cy, arena->getCx(), arena->getCy());

    if ((distanceFromBorder + getRadius()) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(cx, cy, enemy->getCx(), enemy->getCy());

        GLfloat safetyDistance = enemy->getRadius() + getRadius();
        if ((distanceFromEnemy <= safetyDistance) && flying) {
            return;
        }
    }

    setCy(cy);
}

void Player::moveXY(const GLfloat& mulX, const GLfloat& mulY) {
    GLfloat cy = getCy() + (mulY * sin(M_PI / 4) * speed);
    GLfloat cx = getCx() + (mulX * cos(M_PI / 4) * speed);

    GLfloat distanceFromBorder = d2p(cx, cy, arena->getCx(), arena->getCy());

    if ((distanceFromBorder + getRadius()) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(cx, cy, enemy->getCx(), enemy->getCy());

        GLfloat safetyDistance = enemy->getRadius() + getRadius();
        if ((distanceFromEnemy <= safetyDistance) && flying) {
            return;
        }
    }

    setCy(cy);
    setCx(cx);
}

// S = So + Vo * t + (a * t^2)/2
void Player::calculatePhysics() {
    Line* strip = &arena->getAirstrip();

    /* Tratamento da decolagem do inicio ao fim da pista */
    // a = 2 * S / t ^ 2
    GLfloat t = 4.0;

    GLfloat dy = strip->getY2() - strip->getY1();
    ay = 2 * dy / pow(t, 2);

    GLfloat dx = strip->getX2() - strip->getX1();
    ax = 2 * dx / pow(t, 2);

    GLfloat distance = sqrt(pow(dy, 2) + pow(dx, 2));
    speed = sqrt(pow(ay, 2) + pow(ax, 2)) * t;

    /* Tratamento do inicio do crescimento do raio do player */
    midAirstripX = strip->getX1() + (strip->getX2() - strip->getX1()) / 2;
    midAirstripY = strip->getY1() + (strip->getY2() - strip->getY1()) / 2;

    // t = sqrt(2 * (S - So) / a)
    GLfloat startDistance = d2p(strip->getX1(), strip->getY1(), midAirstripX, midAirstripY);
    GLfloat acc = 2 * distance / pow(t, 2);
    GLfloat midAirstripTime = sqrt(2 * (distance - startDistance) / acc);

    // V = S / t
    radiusSpeed = getRadius() / (t - midAirstripTime);
}

void Player::takeOffAirplane(GLint& currentTime) {
    GLfloat stepY = (ay * pow(currentTime / 1000.0, 2)) / 2;
    setCy(startY + stepY);

    GLfloat stepX = (ax * pow(currentTime / 1000.0, 2)) / 2;
    setCx(startX + stepX);

    // S = So + V * t
    if (getCx() >= midAirstripX && getCy() >= midAirstripY) {
        GLfloat timeR = (currentTime - oldRadiusTime) / 1000.0;
        GLfloat stepR = radiusSpeed * timeR;
        setRadius(startR + stepR);
    } else {
        oldRadiusTime = currentTime;
    }
}
