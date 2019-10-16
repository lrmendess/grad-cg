#include "player.h"

Player::~Player() {
    for (Projectile* projectile : this->projectiles) {
        delete(projectile);
    }
}

Player::Player(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius) {
    this->cx = cx;
    this->cy = cy;
    this->radius = radius;
    this->arena = arena;
    this->startX = this->cx;
    this->startY = this->cy;
    this->startR = radius;

    // Angulo inicial
    this->angle = (180 / M_PI) * atan2(
        arena->getAirstrip()->getY2() - arena->getAirstrip()->getY1(),
        arena->getAirstrip()->getX2() - arena->getAirstrip()->getX1()
    );
}

void Player::moveX(GLfloat angle, GLfloat dt) {
    this->angle += angle * dt;
}

void Player::move(GLfloat mul, GLfloat dt) {
    GLfloat my = this->cy + sin(this->angle * M_PI / 180) * (mul * sin(M_PI / 4) * this->speed * dt);
    GLfloat mx = this->cx + cos(this->angle * M_PI / 180) * (mul * cos(M_PI / 4) * this->speed * dt);

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if (distanceFromBorder > arena->getRadius()) {
        GLfloat alpha = atan2(this->cy, this->cx) * 180 / M_PI;
        GLfloat theta = (-2) * (alpha - this->angle - 90) * M_PI / 180;
        
        mx = cx * cos(theta) - cy * sin(theta);
        my = cx * sin(theta) + cy * cos(theta);
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(mx, my, enemy->getCx(), enemy->getCy());

        GLfloat safetyDistance = enemy->getRadius() + this->radius;
        if ((distanceFromEnemy <= safetyDistance) && this->flying) {
            this->dead = true;
            return;
        }
    }

    this->propellerAngle += this->speed / 8;
    this->cy = my;
    this->cx = mx;
}

void Player::updateProjectiles(GLfloat mul, GLfloat dt) {
    list<Projectile*> forRemove;

    for (auto p : this->projectiles) {
        GLfloat projectileAngle = p->getAngle() * M_PI / 180;

        GLfloat my = p->getCy() + sin(projectileAngle) * (mul * sin(M_PI / 4) * p->getSpeed() * dt);
        GLfloat mx = p->getCx() + cos(projectileAngle) * (mul * cos(M_PI / 4) * p->getSpeed() * dt);

        // Se o projetil encostar na borda, ele sera removido da lista de projeteis
        // disparados pelo player em questao
        GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

        if (distanceFromBorder > arena->getRadius()) {
            forRemove.push_back(p);
        } else {
            p->setCy(my);
            p->setCx(mx);
        }
    }

    // Sendo removido da lista e tendo sua memoria liberada, ele nao sera mais desenhado
    for (auto p : forRemove) {
        this->projectiles.remove(p);
        delete(p);
    }
}

/* Calcula a fisica a ser utilizada no aviao */
void Player::calculatePhysics() {
    // S = So + Vo * t + (a * t^2)/2
    Line* strip = arena->getAirstrip();

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
    this->midAirstripTime = sqrt(2 * (distance - startDistance) / acc);

    // V = S / t
    radiusSpeed = this->radius / (t - midAirstripTime);
}

/* Decolagem do aviao */
void Player::takeOffAirplane(GLint currentTime) {
    GLfloat stepY = (ay * pow(currentTime / 1000.0, 2)) / 2;
    this->cy = startY + stepY;

    GLfloat stepX = (ax * pow(currentTime / 1000.0, 2)) / 2;
    this->cx = startX + stepX;

    // S = So + V * t
    if ((currentTime / 1000.0) >= midAirstripTime) {
        GLfloat timeR = (currentTime - oldRadiusTime) / 1000.0;
        GLfloat stepR = radiusSpeed * timeR;
        this->radius = startR + stepR;
    } else {
        oldRadiusTime = currentTime;
    }

    this->propellerAngle += this->speed / 8;
}

void Player::drawWings() {
    glColor3f(0.0, 0.0, 0.0);

    // Direita
    glPushMatrix();
        glBegin(GL_POLYGON);
            glVertex3f(-this->radius / 4, 0.0, 0.0);
            glVertex3f(-this->radius / 24, this->radius, 0.0);
            glVertex3f( this->radius / 4, this->radius, 0.0);
            glVertex3f( this->radius / 4, 0, 0.0);
        glEnd();
    glPopMatrix();

    //Esquerda
    glPushMatrix();
        glBegin(GL_POLYGON);
            glVertex3f(-this->radius / 4,   0.0, 0.0);
            glVertex3f(-this->radius / 24, -this->radius, 0.0);
            glVertex3f( this->radius / 4,  -this->radius, 0.0);
            glVertex3f( this->radius / 4,   0, 0.0);
        glEnd();
    glPopMatrix();
}

void Player::drawCannon() {
    glColor3f(0.0, 0.0, 0.0);

    glPushMatrix();
		glTranslatef(this->radius, 0.0, 0);
		glRotatef(this->cannonAngle, 0.0, 0.0, 1.0);

        glBegin(GL_POLYGON);
            glVertex3f(0.0,             -this->radius / 12, 0.0);
            glVertex3f(this->radius / 2,-this->radius / 12, 0.0);
            glVertex3f(this->radius / 2, this->radius / 12, 0.0);
            glVertex3f(0.0,              this->radius / 12, 0.0);
        glEnd();
    glPopMatrix();
}

void Player::drawFuselage() {
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
        GLfloat a, px, py;

        for (int i = 0; i < 360; i++) {
            a = (i * M_PI) / 180.0;
            px = cos(a) * this->radius;
            py = sin(a) * this->radius / 4;
            glVertex2f(px, py);
        }
    glEnd();
}

void Player::drawTriangles(GLfloat length) {
    glColor3f(1.0, 1.0, 0.0);

	glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(length, length, 0.0);
		glVertex3f(-length, length, 0.0);
	glEnd();

	glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(length, -length, 0.0);
		glVertex3f(-length,  -length, 0.0);
	glEnd();
}

void Player::drawLeftPropeller() {
    glPushMatrix();
        /* [INICIO] Haste das helices */
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_POLYGON);
		    glVertex3f(this->radius / 2, -this->radius / 2.5, 0.0);
		    glVertex3f(this->radius / 2, -this->radius / 1.75, 0.0);
		    glVertex3f(0.0, -this->radius / 1.75, 0.0);
		    glVertex3f(0.0, -this->radius / 2.5, 0.0);
	    glEnd();

        /* [FIM] Haste das helices */
        glTranslatef(this->radius / 2, -this->radius / 2, 0);
		glPushMatrix();
			glRotatef(this->propellerAngle, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
            glPushMatrix();
			glRotatef(this->propellerAngle + 90, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 180, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 270, 1.0, 0.0, 0.0);
            drawTriangles(this->radius / 4);
		glPopMatrix();
    glPopMatrix();
}

void Player::drawRightPropeller() {
    glPushMatrix();
        /* [INICIO] Haste das helices */
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_POLYGON);
		    glVertex3f(this->radius / 2, this->radius / 2.50, 0.0);
		    glVertex3f(this->radius / 2, this->radius / 1.75, 0.0);
		    glVertex3f(0.0, this->radius / 1.75, 0.0);
		    glVertex3f(0.0, this->radius / 2.50, 0.0);
	    glEnd();

        /* [FIM] Haste das helices */
        glTranslatef(this->radius / 2, this->radius / 2, 0);
		glPushMatrix();
			glRotatef(this->propellerAngle, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
            glPushMatrix();
			glRotatef(this->propellerAngle + 90, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 180, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 270, 1.0, 0.0, 0.0);
            drawTriangles(this->radius / 4);
		glPopMatrix();
    glPopMatrix();
}

void Player::drawFin() {
    glColor3f(0.0, 0.0, 0.0);

    glPushMatrix();
        glBegin(GL_POLYGON);
            glVertex3f(-this->radius * 0.9, -this->radius / 12, 0.0);
            glVertex3f(-this->radius / 3,   -this->radius / 12, 0.0);
            glVertex3f(-this->radius / 3,    this->radius / 12, 0.0);
            glVertex3f(-this->radius * 0.9,  this->radius / 12, 0.0);
        glEnd();
    glPopMatrix();
}

void Player::drawCockpit() {
    glColor3f(0.0, 0.0, 0.0);

    glPushMatrix();
        glTranslatef(this->radius / 2, 0.0, 0.0);
        glBegin(GL_POLYGON);
            GLfloat a, px, py;
            GLfloat r = this->radius / 2;

            for (int i = 0; i < 360; i++) {
                a = (i * M_PI) / 180.0;
                px = cos(a) * r / 1.5;
                py = sin(a) * r / 4;
                glVertex2f(px, py);
            }
        glEnd();
    glPopMatrix();
}

void Player::drawProjectiles() {
    for (auto p : this->projectiles) {
        p->draw();
    }
}

/* Desenha todo o corpo do aviao */
void Player::drawAirplane() {    
    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0);
        glRotatef(this->angle, 0, 0, 1.0);

        drawWings();
        drawLeftPropeller();
        drawRightPropeller();
        drawFuselage();
        drawFin();
        drawCockpit();
        drawCannon();
    glPopMatrix();

    drawProjectiles();
}

void Player::fire(GLfloat mul) {
    Projectile* projectile = new Projectile();

    projectile->setPlayer(this);
    projectile->setSpeed(this->speed * mul);
    projectile->setLength(this->radius / 8);

    GLfloat airplaneAngleInRadians = this->angle * M_PI / 180;
    GLfloat cannonAngleInRadians = this->cannonAngle * M_PI / 180;

    GLfloat px = this->cx;
    px += this->radius * cos(airplaneAngleInRadians);
    px += this->radius / 2 * cos(cannonAngleInRadians + airplaneAngleInRadians);
    
    GLfloat py = this->cy;
    py += this->radius * sin(airplaneAngleInRadians);
    py += this->radius / 2 * sin(cannonAngleInRadians + airplaneAngleInRadians);

    projectile->setCx(px);
    projectile->setCy(py);
    projectile->setAngle(this->angle + this->cannonAngle);

    projectiles.push_back(projectile);
}

/* Reseta o player para as condicoes iniciais */
void Player::reset() {
    this->cx = this->startX;
    this->cy = this->startY;
    this->radius = this->startR;

    this->angle = 1 / M_PI * 180 * atan2(
        arena->getAirstrip()->getY2() - arena->getAirstrip()->getY1(),
        arena->getAirstrip()->getX2() - arena->getAirstrip()->getX1()
    );

    this->dead          = false;
    this->flying        = false;
    this->takeOff       = false;

    this->speed         = 0.0;
    this->cannonAngle   = 0.0;

    this->projectiles.clear();    

    this->calculatePhysics();
}