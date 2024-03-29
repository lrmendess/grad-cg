#include "enemy.h"

Enemy::~Enemy() {
    for (Projectile* projectile : this->projectiles) {
        delete(projectile);
    }
}

Enemy::Enemy(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius) {
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
        Projectile* projectile = new Projectile(this, mul, mulVelAirplane);
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
    glColor3f(0.0, 0.0, 0.0);

    // Direita
    glBegin(GL_POLYGON);
        glVertex3f(-this->radius / 4, 0.0, 0.0);
        glVertex3f(-this->radius / 24, this->radius, 0.0);
        glVertex3f( this->radius / 4, this->radius, 0.0);
        glVertex3f( this->radius / 4, 0, 0.0);
    glEnd();

    //Esquerda
    glBegin(GL_POLYGON);
        glVertex3f(-this->radius / 4,   0.0, 0.0);
        glVertex3f(-this->radius / 24, -this->radius, 0.0);
        glVertex3f( this->radius / 4,  -this->radius, 0.0);
        glVertex3f( this->radius / 4,   0, 0.0);
    glEnd();

    /* Asinhas de tras */
    glPushMatrix();
        glTranslatef(-this->radius * 0.9, 0.0, 0.0);
        glColor3f(0.0, 0.0, 0.0);

        // Direita
        glBegin(GL_POLYGON);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, this->radius * 0.5, 0.0);
            glVertex3f(this->radius * 0.25, this->radius / 3, 0.0);
            glVertex3f(this->radius * 0.25, 0.0, 0.0);
        glEnd();

        //Esquerda
        glBegin(GL_POLYGON);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, -this->radius * 0.5, 0.0);
            glVertex3f(this->radius * 0.25, -this->radius / 3, 0.0);
            glVertex3f(this->radius * 0.25, 0.0, 0.0);
        glEnd();
    glPopMatrix();
}

void Enemy::drawCannon() {
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

void Enemy::drawFuselage() {
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
        GLfloat angle, px, py;

        for (int i = 0; i < 360; i++) {
            angle = (i * M_PI) / 180.0;
            px = cos(angle) * this->radius;
            py = sin(angle) * this->radius / 4;
            glVertex2f(px, py);
        }
    glEnd();
}

void Enemy::drawHourglass(GLfloat length) {
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

void Enemy::drawLeftPropeller() {
    glPushMatrix();
        /* [INICIO] Haste das helices */
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_POLYGON);
		    glVertex3f(this->radius / 2, -this->radius / 2.50, 0.0);
		    glVertex3f(this->radius / 2, -this->radius / 1.75, 0.0);
		    glVertex3f(0.0, -this->radius / 1.75, 0.0);
		    glVertex3f(0.0, -this->radius / 2.50, 0.0);
	    glEnd();

        /* [FIM] Haste das helices */
        glTranslatef(this->radius / 2, -this->radius / 2, 0);
		glPushMatrix();
			glRotatef(this->propellerAngle, 1.0, 0.0, 0.0);
			drawHourglass(this->radius / 4);
		glPopMatrix();
            glPushMatrix();
			glRotatef(this->propellerAngle + 90, 1.0, 0.0, 0.0);
			drawHourglass(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 180, 1.0, 0.0, 0.0);
			drawHourglass(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 270, 1.0, 0.0, 0.0);
            drawHourglass(this->radius / 4);
		glPopMatrix();
    glPopMatrix();
}

void Enemy::drawRightPropeller() {
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
			drawHourglass(this->radius / 4);
		glPopMatrix();
            glPushMatrix();
			glRotatef(this->propellerAngle + 90, 1.0, 0.0, 0.0);
			drawHourglass(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 180, 1.0, 0.0, 0.0);
			drawHourglass(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(this->propellerAngle + 270, 1.0, 0.0, 0.0);
            drawHourglass(this->radius / 4);
		glPopMatrix();
    glPopMatrix();
}

void Enemy::drawFin() {
    glColor3f(0.0, 0.0, 0.0);

    glPushMatrix();
        glTranslatef(-this->radius * 0.9, 0.0, 0.0);

        glBegin(GL_POLYGON);
            glVertex3f(0.0,-this->radius / 12, 0.0);
            glVertex3f(this->radius / 2,-this->radius / 12, 0.0);
            glVertex3f(this->radius / 2, this->radius / 12, 0.0);
            glVertex3f(0.0, this->radius / 12, 0.0);
        glEnd();
    glPopMatrix();
}

void Enemy::drawCockpit() {
    glColor3f(0.0, 0.0, 0.0);

    GLfloat length = this->radius / 2;
    
    glPushMatrix();
        glTranslatef(this->radius / 2, 0.0, 0.0);
        
        glBegin(GL_POLYGON);
            GLfloat angle, px, py;

            for (int i = 0; i < 360; i++) {
                angle = (i * M_PI) / 180.0;
                px = cos(angle) * length / 1.5;
                py = sin(angle) * length / 4;
                glVertex2f(px, py);
            }
        glEnd();
    glPopMatrix();
}

/* Desenha todo o corpo do aviao */
void Enemy::drawAirplane() {
    drawProjectiles();

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