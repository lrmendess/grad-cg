#include "player.h"

Player::~Player() {
    for (Projectile* projectile : this->projectiles) {
        delete(projectile);
    }
}

Player::Player(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius) {
    this->cx = cx;
    this->cy = cy;
    this->cz = 10;
    this->radius = 2 * radius;
    this->arena = arena;
    this->startX = this->cx;
    this->startY = this->cy;
    this->startZ = this->cz;
    this->startR = radius;

    // Angulo inicial
    this->angle = (180 / M_PI) * atan2(
        arena->getAirstrip()->getY2() - arena->getAirstrip()->getY1(),
        arena->getAirstrip()->getX2() - arena->getAirstrip()->getX1()
    );

    calculatePhysics();
}

void Player::moveX(GLfloat angle, GLfloat dt) {
    this->angle += angle * dt;
}

void Player::moveZ(GLfloat angle, GLfloat dt) {
    this->angleTheta += angle * dt;

    if (this->angleTheta > 45) {
        this->angleTheta = 45;
    }

    if (this->angleTheta < -45) {
        this->angleTheta = -45;
    }
}

void Player::move(GLfloat mul, GLfloat dt) {
    GLfloat fiRad = this->angle * M_PI / 180;
    GLfloat thetaRad = this->angleTheta * M_PI / 180;

    GLfloat stepX = mul * this->speed * dt * cos(M_PI / 4) * cos(M_PI / 4);
    GLfloat stepY = mul * this->speed * dt * cos(M_PI / 4) * sin(M_PI / 4);
    GLfloat stepZ = mul * this->speed * dt * sin(M_PI / 4);

    GLfloat mx = this->cx + stepY * cos(thetaRad) * cos(fiRad);
    GLfloat my = this->cy + stepX * cos(thetaRad) * sin(fiRad);
    GLfloat mz = this->cz + stepZ * sin(thetaRad);

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if (distanceFromBorder > arena->getRadius()) {
        GLfloat alpha = atan2(this->cy, this->cx) * 180 / M_PI;
        GLfloat beta = (-2) * (alpha - this->angle - 90) * M_PI / 180;
        
        mx = cx * cos(beta) - cy * sin(beta);
        my = cx * sin(beta) + cy * cos(beta);
    }

    for (auto enemy : arena->getAirEnemies()) {
        if (!enemy->isDead()) {
            GLfloat distanceFromEnemy = d2p3d(mx, my, mz, enemy->getCx(), enemy->getCy(), enemy->getCz());

            GLfloat safetyDistance = enemy->getRadius() + this->radius;
            if ((distanceFromEnemy <= safetyDistance) && this->flying) {
                this->dead = true;
                return;
            }
        }
    }

    this->propellerAngle += this->speed / 8;
    this->cy = my;
    this->cx = mx;

    if (mz <= arena->getRadius() && mz >= 0) {
        this->cz = mz;
    }
}

void Player::drawProjectiles(GLuint projTexture) {
    for (auto p : this->projectiles) {
        glPushMatrix();
            glTranslatef(p->getCx(), p->getCy(), p->getCz());
            glRotatef(p->getAngle(), .0, .0, 1.0);
            glRotatef(-p->getAngleTheta(), .0, 1.0, .0);
            p->draw(projTexture);
        glPopMatrix();
    }
}

void Player::fire(GLfloat mul, GLfloat mulVelAirplane) {
    Projectile* projectile = new Projectile(this, mul, mulVelAirplane);
    projectiles.push_back(projectile);
}

void Player::updateProjectiles(GLfloat dt) {
    list<Projectile*> forRemove;

    for (auto p : this->projectiles) {
        bool canUpdate = true;
        GLfloat projectileFiRad = p->getAngle() * M_PI / 180;
        GLfloat projectileThetaRad = p->getAngleTheta() * M_PI / 180;

        GLfloat stepX = p->getSpeed() * dt * cos(M_PI / 4) * cos(M_PI / 4);
        GLfloat stepY = p->getSpeed() * dt * cos(M_PI / 4) * sin(M_PI / 4);
        GLfloat stepZ = p->getSpeed() * dt * sin(M_PI / 4);

        GLfloat mx = p->getCx() + stepY * cos(projectileThetaRad) * cos(projectileFiRad);
        GLfloat my = p->getCy() + stepX * cos(projectileThetaRad) * sin(projectileFiRad);
        GLfloat mz = p->getCz() + stepZ * sin(projectileThetaRad);

        // Se o projetil encostar na borda, ele sera removido da lista de projeteis
        // disparados pelo player em questao
        GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

        if (distanceFromBorder > arena->getRadius()) {
            forRemove.push_back(p);
        } else {
            for (auto enemy : arena->getAirEnemies()) {
                if (!enemy->isDead()) {
                    GLfloat distanceFromEnemy = d2p3d(mx, my, mz, enemy->getCx(), enemy->getCy(), enemy->getCz());

                    if (distanceFromEnemy <= enemy->getRadius()) {
                        enemy->kill();
                        forRemove.push_back(p);
                        canUpdate = false;
                        break;
                    }
                }
            }

            if (canUpdate) {
                p->setCx(mx);
                p->setCy(my);
                p->setCz(mz);
            }
        }
    }

    // Sendo removido da lista e tendo sua memoria liberada, ele nao sera mais desenhado
    for (auto p : forRemove) {
        this->projectiles.remove(p);
        // delete(p);
    }
}

void Player::drawBombs(GLuint bombTexture) {
    for (auto b : this->bombs) {
        glPushMatrix();
            glTranslatef(b->getCx(), b->getCy(), b->getCz());
            b->draw(bombTexture);
        glPopMatrix();
    }
}

void Player::bomb(GLfloat mulVelAirplane) {
    Bomb* bomb = new Bomb(this, mulVelAirplane);
    bombs.push_back(bomb);
}

void Player::updateBombs(GLfloat currentTime, GLfloat dt) {
    list<Bomb*> forRemove;

    for (auto b : this->bombs) {
        bool canUpdate = true;
        GLfloat bombFiRad = b->getAngle() * M_PI / 180;
        GLfloat bombThetaRad = .0;

        GLfloat stepX = b->getSpeed() * dt * cos(M_PI / 4) * cos(M_PI / 4);
        GLfloat stepY = b->getSpeed() * dt * cos(M_PI / 4) * sin(M_PI / 4);
        // GLfloat stepZ = b->getSpeed() * dt * sin(M_PI / 4);

        GLfloat mx = b->getCx() + stepY * cos(bombThetaRad) * cos(bombFiRad);
        GLfloat my = b->getCy() + stepX * cos(bombThetaRad) * sin(bombFiRad);
        // GLfloat mz = b->getCz() + stepZ * sin(bombThetaRad);

        // Se a bomba encostar na borda, ela sera removido da lista de bombas
        // lancadas pelo player em questao
        GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

        GLfloat t = currentTime - b->getTimeReleased();

        if (distanceFromBorder > arena->getRadius()) {
            forRemove.push_back(b);
        } if(b->getCz() <= 0.0) {
            forRemove.push_back(b);
            
            for (auto enemy : arena->getGroundEnemies()) {
                if (!enemy->isDead()) {
                    GLfloat distanceFromEnemy = d2p(mx, my, enemy->getCx(), enemy->getCy());

                    if (distanceFromEnemy <= enemy->getRadius()) {
                        enemy->kill();
                        points++;
                        canUpdate = false;
                        break;
                    }
                }
            }
        } else {
            for (auto enemy : arena->getGroundEnemies()) {
                if (!enemy->isDead()) {
                    GLfloat distanceFromEnemy = d2p3d(mx, my, b->getCz() - (9.8 * t) / 2, enemy->getCx(), enemy->getCy(), .0);

                    if (distanceFromEnemy <= enemy->getRadius()) {
                        enemy->kill();
                        points++;
                        canUpdate = false;
                        break;
                    }
                }
            }

            if (canUpdate) {
                b->setCx(mx);
                b->setCy(my);
                b->setCz(b->getCz() - (9.8 * t) / 2);
            }
        }
    }

    for (auto b : forRemove) {
        this->bombs.remove(b);
        // delete(b);
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

    GLfloat dz = 4 * this->radius;
    az = 2 * dz / pow(t - midAirstripTime, 2);
}

/* Decolagem do aviao */
void Player::takeOffAirplane(GLint currentTime) {
    GLfloat stepY = (ay * pow(currentTime / 1000.0, 2)) / 2;
    this->cy = startY + stepY;

    GLfloat stepX = (ax * pow(currentTime / 1000.0, 2)) / 2;
    this->cx = startX + stepX;

    // S = So + V * t
    if ((currentTime / 1000.0) >= midAirstripTime) {
        GLfloat time = (currentTime - oldTime) / 1000.0;
        GLfloat stepZ = (az * pow(time, 2)) / 2;
        this->cz = startZ + stepZ;
    } else {
        oldTime = currentTime;
    }

    this->propellerAngle += this->speed / 8;
}

void Player::drawWings() {
    GLfloat wingsEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat wingsColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, wingsEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wingsColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
        glScalef(.5, 2.0, .0625);
        
        glutSolidCube(this->radius);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-this->radius * .65, .0, .0);
        glScalef(.25, 1.25, .0625);
        
        glutSolidCube(this->radius);
    glPopMatrix();
}

void Player::drawCannon() {
    GLfloat cannonEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat cannonColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, cannonEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cannonColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
		glTranslatef(this->radius, 0.0, 0);
        glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(-this->cannonAngle, 1.0, 0.0, 0.0);
        glRotatef(-this->cannonAngleTheta, .0, 1.0, .0);

        GLUquadricObj* cannon = gluNewQuadric();
            gluQuadricDrawStyle(cannon, GLU_FILL);
            gluQuadricNormals(cannon, GLU_SMOOTH);
            gluQuadricTexture(cannon, GLU_FALSE);
            gluQuadricOrientation(cannon, GLU_OUTSIDE);
            gluCylinder(cannon, this->radius / 12, this->radius / 12, this->radius / 2, 16, 16);
        gluDeleteQuadric(cannon);
    glPopMatrix();
}

void Player::drawFuselage(GLuint playerTexture) {
    GLfloat fuselageEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat fuselageColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, fuselageEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, fuselageColor);
    glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();
        glScalef(1.0, .25, .25);
        
        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, playerTexture);
            GLUquadricObj* fuselage = gluNewQuadric();
                gluQuadricDrawStyle(fuselage, GLU_FILL);
                gluQuadricNormals(fuselage, GLU_SMOOTH);
                gluQuadricTexture(fuselage, GLU_TRUE);
                gluQuadricOrientation(fuselage, GLU_OUTSIDE);
                gluSphere(fuselage, this->radius, 32, 32);
            gluDeleteQuadric(fuselage);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Player::drawHourglass(GLfloat length) {
    GLfloat hourglassEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat hourglassColor[] = {1.0, 1.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, hourglassEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, hourglassColor);
    glColor3f(1.0, 1.0, 0.0);
    
	glBegin(GL_TRIANGLES);
        glVertex3f(.0, .0, .0);
        glVertex3f(length, length, .0);
		glVertex3f(-length, length, .0);
	glEnd();

	glBegin(GL_TRIANGLES);
        glVertex3f(.0, .0, .0);
		glVertex3f(length, -length, .0);
		glVertex3f(-length,  -length, .0);
	glEnd();
}

void Player::drawLeftPropeller() {
    GLfloat stemEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat stemColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, stemEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, stemColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
        glTranslatef(.0, this->radius * .5, .0);
        glRotatef(90, .0, 1.0, .0);

        GLUquadricObj* stem = gluNewQuadric();
            gluQuadricDrawStyle(stem, GLU_FILL);
            gluQuadricNormals(stem, GLU_SMOOTH);
            gluQuadricTexture(stem, GLU_FALSE);
            gluQuadricOrientation(stem, GLU_OUTSIDE);
            gluCylinder(stem, this->radius * .0625, this->radius * .0625, this->radius * .5, 10, 10);
        gluDeleteQuadric(stem);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(this->radius * .5, -this->radius * .5, .0);

		glPushMatrix();
			glRotatef(this->propellerAngle, 1.0, .0, .0);
			drawHourglass(this->radius * .25);
		glPopMatrix();
            glPushMatrix();
			glRotatef(this->propellerAngle + 90, 1.0, .0, .0);
			drawHourglass(this->radius * .25);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 180, 1.0, .0, .0);
			drawHourglass(this->radius * .25);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 270, 1.0, .0, .0);
            drawHourglass(this->radius * .25);
		glPopMatrix();
    glPopMatrix();
}

void Player::drawRightPropeller() {
    GLfloat stemEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat stemColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, stemEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, stemColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
        glTranslatef(.0, - this->radius * .5, .0);
        glRotatef(90, .0, 1.0, .0);

        GLUquadricObj* stem = gluNewQuadric();
            gluQuadricDrawStyle(stem, GLU_FILL);
            gluQuadricNormals(stem, GLU_SMOOTH);
            gluQuadricTexture(stem, GLU_FALSE);
            gluQuadricOrientation(stem, GLU_OUTSIDE);
            gluCylinder(stem, this->radius * .0625, this->radius * .0625, this->radius * .5, 16, 16);
        gluDeleteQuadric(stem);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(this->radius * .5, this->radius * .5, 0);
		
        glPushMatrix();
			glRotatef(this->propellerAngle, 1.0, .0, .0);
			drawHourglass(this->radius * .25);
		glPopMatrix();
            glPushMatrix();
			glRotatef(this->propellerAngle + 90, 1.0, .0, .0);
			drawHourglass(this->radius * .25);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 180, 1.0, .0, .0);
			drawHourglass(this->radius * .25);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 270, 1.0, .0, .0);
            drawHourglass(this->radius * .25);
		glPopMatrix();
    glPopMatrix();
}

void Player::drawFin() {
    GLfloat finEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat finColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, finEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, finColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
        glTranslatef(-this->radius * .65, .0, .0);
        glScalef(.25, .0625, .5);
        glTranslatef(.0, .0, this->radius * .5);

        glutSolidCube(this->radius);
    glPopMatrix();
}

void Player::drawCockpit() {
    GLfloat cockpitEmission[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat cockpitColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, cockpitEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cockpitColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
        glTranslatef(this->radius * .5, .0, .0);
        glScalef(.375, .125, .25);

        glutSolidSphere(this->radius, 20, 20);
    glPopMatrix();
}

/* Desenha todo o corpo do aviao */
void Player::drawAirplane(GLuint playerTexture, GLuint projTexture, GLuint bombTexture) {
    drawProjectiles(projTexture);
    drawBombs(bombTexture);

    glPushMatrix();
        glTranslatef(this->cx, this->cy, this->cz);
        glRotatef(this->angle, .0, .0, 1.0);
        glRotatef(-this->angleTheta, .0, 1.0, .0);
        
        drawLeftPropeller();
        drawRightPropeller();
        drawWings();
        drawCannon();
        drawCockpit();
        drawFin();
        drawFuselage(playerTexture);
    glPopMatrix();
}

/* Reseta o player para as condicoes iniciais */
void Player::reset() {
    points = 0;
    cx = startX;
    cy = startY;
    cz = startZ;

    angle = 1 / M_PI * 180 * atan2(
        arena->getAirstrip()->getY2() - arena->getAirstrip()->getY1(),
        arena->getAirstrip()->getX2() - arena->getAirstrip()->getX1()
    );

    angleTheta = 0.0;

    dead = false;
    flying = false;
    takeOff = false;

    speed = 0.0;
    cannonAngle = 0.0;
    cannonAngleTheta = 0.0;

    projectiles.clear();
    bombs.clear();

    calculatePhysics();
}

void Player::kill() {
    this->dead = true;
}
