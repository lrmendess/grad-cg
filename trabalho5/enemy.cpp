#include "enemy.h"

Enemy::~Enemy() {
    for (Projectile* projectile : this->projectiles) {
        delete(projectile);
    }
}

Enemy::Enemy(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius, GLuint projTexture) {
    this->cx = cx;
    this->cy = cy;
    this->radius = radius;
    this->arena = arena;

    this->startX = cx;
    this->startY = cy;

    // Angulo inicial
    this->angle = (180 / M_PI) * atan2(
        arena->getAirstrip()->getY2() - arena->getAirstrip()->getY1(),
        arena->getAirstrip()->getX2() - arena->getAirstrip()->getX1()
    );
    
    this->projTexture = projTexture;
}

void Enemy::moveX(GLfloat angle, GLfloat dt) {
    this->angle += angle * dt;
}

void Enemy::move(GLfloat mul, GLfloat dt) {
    GLfloat my = this->cy + sin(this->angle * M_PI / 180) * (mul * sin(M_PI / 4) * this->speed * dt);
    GLfloat mx = this->cx + cos(this->angle * M_PI / 180) * (mul * cos(M_PI / 4) * this->speed * dt);

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if (distanceFromBorder > arena->getRadius()) {
        GLfloat alpha = atan2(this->cy, this->cx) * 180 / M_PI;
        GLfloat beta = (-2) * (alpha - this->angle - 90) * M_PI / 180;
        
        mx = cx * cos(beta) - cy * sin(beta);
        my = cx * sin(beta) + cy * cos(beta);
    }

    // for (auto enemy : arena->getAirEnemies()) {
    //     GLfloat distanceFromEnemy = d2p(mx, my, enemy->getCx(), enemy->getCy());
    // 
    //     GLfloat safetyDistance = enemy->getRadius() + this->radius;
    //     if (distanceFromEnemy <= safetyDistance) {
    //         this->dead = true;
    //         return;
    //     }
    // }

    this->propellerAngle += this->speed / 8;
    this->cy = my;
    this->cx = mx;
}

void Enemy::drawProjectiles() {
    for (auto p : this->projectiles) {
        glPushMatrix();
            glTranslatef(p->getCx(), p->getCy(), 0.0);
            p->draw();
        glPopMatrix();
    }
}

void Enemy::fire(GLfloat mul, GLfloat mulVelAirplane, GLfloat dt) {
    currentTimeFire += dt;

    if (currentTimeFire >= maxTimeFire) {
        Projectile* projectile = new Projectile(this, mul, mulVelAirplane, this->projTexture);
        projectiles.push_back(projectile);

        // currentTimeFire = currentTimeFire - maxTimeFire;
        currentTimeFire = 0.0;
    }
}

void Enemy::updateProjectiles(GLfloat dt) {
    list<Projectile*> forRemove;

    for (auto p : this->projectiles) {
        GLfloat projectileAngle = p->getAngle() * M_PI / 180;

        GLfloat my = p->getCy() + sin(projectileAngle) * (sin(M_PI / 4) * p->getSpeed() * dt);
        GLfloat mx = p->getCx() + cos(projectileAngle) * (cos(M_PI / 4) * p->getSpeed() * dt);

        // Se o projetil encostar na borda, ele sera removido da lista de projeteis
        // disparados pelo player em questao
        GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

        Player* player = arena->getPlayer();

        GLfloat distanceFromPlayer = d2p(mx, my, player->getCx(), player->getCy());

        if (distanceFromBorder > arena->getRadius()) {
            forRemove.push_back(p);
        } else if(distanceFromPlayer <= player->getRadius() && player->isFlying()) {
            player->kill();
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

void Enemy::drawWings() {
    glPushMatrix();
        glColor3f(.0, .0, .0);
        
        glScalef(.5, 2.0, .0625);
        glutSolidCube(this->radius);
    glPopMatrix();

    glPushMatrix();
        glColor3f(.0, .0, .0);
        glTranslatef(-this->radius * .65, .0, .0);

        glScalef(.25, 1.25, .0625);
        glutSolidCube(this->radius);
    glPopMatrix();
}

void Enemy::drawCannon() {
    glColor3f(0.0, 0.0, 0.0);

    glPushMatrix();
		glTranslatef(this->radius, 0.0, 0);
        glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(-this->cannonAngle, 1.0, 0.0, 0.0);

        GLUquadricObj* obj = gluNewQuadric();
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluCylinder(obj, this->radius / 12, this->radius / 12, this->radius / 2, 10, 10);
        gluDeleteQuadric(obj);
    glPopMatrix();
}

void Enemy::drawFuselage() {
    glColor3f(1.0, .0, .0);

    glPushMatrix();
        glScalef(1.0, .25, .25);
        glutSolidSphere(this->radius, 30, 30);
    glPopMatrix();
}

void Enemy::drawHourglass(GLfloat length) {
    glColor3f(1.0, 1.0, .0);

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

void Enemy::drawLeftPropeller() {
    glPushMatrix();
        glColor3f(.0, .0, .0);
        glTranslatef(.0, this->radius * .5, .0);
        glRotatef(90, .0, 1.0, .0);

        GLUquadricObj* obj = gluNewQuadric();
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluCylinder(obj, this->radius * .0625, this->radius * .0625, this->radius * .5, 10, 10);
        gluDeleteQuadric(obj);
    glPopMatrix();

    glPushMatrix();
        glColor3f(.0, .0, .0);
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

void Enemy::drawRightPropeller() {
    glPushMatrix();
        glColor3f(.0, .0, .0);
        glTranslatef(.0, - this->radius * .5, .0);
        glRotatef(90, .0, 1.0, .0);

        GLUquadricObj* obj = gluNewQuadric();
        gluQuadricNormals(obj, GLU_SMOOTH);
        gluCylinder(obj, this->radius * .0625, this->radius * .0625, this->radius * .5, 10, 10);
        gluDeleteQuadric(obj);
    glPopMatrix();

    glPushMatrix();
        glColor3f(.0, .0, .0);
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

void Enemy::drawFin() {
    glColor3f(.0, .0, .0);

    glPushMatrix();
        glTranslatef(-this->radius * .65, .0, .0);
        glScalef(.25, .0625, .5);
        glTranslatef(.0, .0, this->radius * .5);

        glutSolidCube(this->radius);
    glPopMatrix();
}

void Enemy::drawCockpit() {
    glColor3f(.0, .0, .0);

    glPushMatrix();
        glTranslatef(this->radius * .5, .0, .0);
        glScalef(.375, .125, .25);

        glutSolidSphere(this->radius, 20, 20);
    glPopMatrix();
}

/* Desenha todo o corpo do aviao */
void Enemy::drawAirplane() {
    drawProjectiles();

    glPushMatrix();
        glTranslatef(this->cx, this->cy, .0);
        glRotatef(this->angle, .0, .0, 1.0);

        drawWings();
        drawLeftPropeller();
        drawRightPropeller();
        drawCannon();
        drawCockpit();
        drawFuselage();
        drawFin();
    glPopMatrix();
}

/* Reseta o player para as condicoes iniciais */
void Enemy::reset() {
    angle = 1 / M_PI * 180 * atan2(
        arena->getAirstrip()->getY2() - arena->getAirstrip()->getY1(),
        arena->getAirstrip()->getX2() - arena->getAirstrip()->getX1()
    );

    dead = false;

    // speed = 0.0;
    cannonAngle = 0.0;

    this->cx = this->startX;
    this->cy = this->startY;

    projectiles.clear();
}

void Enemy::kill() {
    this->dead = true;

    projectiles.clear();
}