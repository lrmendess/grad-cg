#include "player.h"

Player::Player(Arena* arena, const GLfloat& cx, const GLfloat& cy, const GLfloat& radius, const GLfloat* color) {
    this->cx = cx;
    this->cy = cy;

    this->radius = radius;

    for (int i = 0; i < 3; i++) {
        this->color[i] = color[i];
    }

    // Referencia da arena que o aviao esta
    this->arena = arena;
    // Armazenamento do estado inicial antes da decolagem
    this->startX = this->cx;
    this->startY = this->cy;
    this->startR = radius;
}

void Player::moveX(const GLfloat& mul) {
    GLfloat mx = this->cx + (mul * this->speed);
    GLfloat my = this->cy;

    GLfloat distanceFromBorder = sqrt(pow(mx - arena->getCx(), 2) + pow(my - arena->getCy(), 2));

    if ((distanceFromBorder + this->radius) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = sqrt(pow(mx - enemy->getCx(), 2) + pow(my - enemy->getCy(), 2));

        if ((distanceFromEnemy <= (enemy->getRadius() + this->radius)) && this->flying) {
            return;
        }
    }

    this->cx = mx;
}

void Player::moveY(const GLfloat& mul) {
    GLfloat my = this->cy + (mul * this->speed);
    GLfloat mx = this->cx;

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if ((distanceFromBorder + this->radius) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(mx, my, enemy->getCx(), enemy->getCy());

        GLfloat safetyDistance = enemy->getRadius() + this->radius;
        if ((distanceFromEnemy <= safetyDistance) && this->flying) {
            return;
        }
    }

    this->cy = my;
}

void Player::moveXY(const GLfloat& mulX, const GLfloat& mulY) {
    GLfloat my = this->cy + (mulY * sin(M_PI / 4) * this->speed);
    GLfloat mx = this->cx + (mulX * cos(M_PI / 4) * this->speed);

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if ((distanceFromBorder + this->radius) > arena->getRadius()) {
        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(mx, my, enemy->getCx(), enemy->getCy());

        GLfloat safetyDistance = enemy->getRadius() + this->radius;
        if ((distanceFromEnemy <= safetyDistance) && this->flying) {
            return;
        }
    }

    this->cy = my;
    this->cx = mx;
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
    this->speed = sqrt(pow(ay, 2) + pow(ax, 2)) * t;

    /* Tratamento do inicio do crescimento do raio do player */
    midAirstripX = strip->getX1() + (strip->getX2() - strip->getX1()) / 2;
    midAirstripY = strip->getY1() + (strip->getY2() - strip->getY1()) / 2;

    // t = sqrt(2 * (S - So) / a)
    GLfloat startDistance = d2p(strip->getX1(), strip->getY1(), midAirstripX, midAirstripY);
    GLfloat acc = 2 * distance / pow(t, 2);
    GLfloat midAirstripTime = sqrt(2 * (distance - startDistance) / acc);

    // V = S / t
    radiusSpeed = this->radius / (t - midAirstripTime);
}

void Player::takeOffAirplane(GLint& currentTime) {
    GLfloat stepY = (ay * pow(currentTime / 1000.0, 2)) / 2;
    this->cy = startY + stepY;

    GLfloat stepX = (ax * pow(currentTime / 1000.0, 2)) / 2;
    this->cx = startX + stepX;

    // S = So + V * t
    if (this->cx >= midAirstripX && this->cy >= midAirstripY) {
        GLfloat timeR = (currentTime - oldRadiusTime) / 1000.0;
        GLfloat stepR = radiusSpeed * timeR;
        this->radius = startR + stepR;
    } else {
        oldRadiusTime = currentTime;
    }
}

void Player::drawAirplane() {
    glPushMatrix();
    glTranslatef(this->cx, this->cy, 0);
    /* Rotacionar em theta para o aviao ficar reto na pista de decolagem */
    glRotatef(0, 0, 0, 1);
    drawSolidCircle(this->radius, this->color);
    glPopMatrix();
}