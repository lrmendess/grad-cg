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

void Enemy::moveZ(GLfloat angle, GLfloat dt) {
    this->angleTheta += angle * dt;

    if (this->angleTheta > 45) {
        this->angleTheta = 45;
    }

    if (this->angleTheta < -45) {
        this->angleTheta = -45;
    }
}

void Enemy::move(GLfloat mul, GLfloat dt) {
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
    this->cz = mz;
}

void Enemy::drawProjectiles(GLuint projTexture) {
    for (auto p : this->projectiles) {
        glPushMatrix();
            glTranslatef(p->getCx(), p->getCy(), 0.0);
            p->draw(projTexture);
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
    glPushMatrix();
        glScalef(.5, 2.0, .0625);
        
        glColor3f(0.0, 0.0, 0.0);
        glutSolidCube(this->radius);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-this->radius * .65, .0, .0);
        glScalef(.25, 1.25, .0625);
        
        glColor3f(0.0, 0.0, 0.0);
        glutSolidCube(this->radius);
    glPopMatrix();
}

void Enemy::drawCannon() {
    glPushMatrix();
		glTranslatef(this->radius, 0.0, 0);
        glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(-this->cannonAngle, 1.0, 0.0, 0.0);

        GLUquadricObj* cannon = gluNewQuadric();
            glColor3f(0.0, 0.0, 0.0);
            gluQuadricDrawStyle(cannon, GLU_FILL);
            gluQuadricNormals(cannon, GLU_SMOOTH);
            gluQuadricTexture(cannon, GLU_FALSE);
            gluQuadricOrientation(cannon, GLU_OUTSIDE);
            gluCylinder(cannon, this->radius / 12, this->radius / 12, this->radius / 2, 16, 16);
        gluDeleteQuadric(cannon);
    glPopMatrix();
}

void Enemy::drawFuselage(GLuint airEnemiesTexture) {
    glPushMatrix();
        glScalef(1.0, .25, .25);
        
        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, airEnemiesTexture);
            GLUquadricObj* fuselage = gluNewQuadric();
                glColor3f(1.0, 1.0, 1.0);
                gluQuadricDrawStyle(fuselage, GLU_FILL);
                gluQuadricNormals(fuselage, GLU_SMOOTH);
                gluQuadricTexture(fuselage, GLU_TRUE);
                gluQuadricOrientation(fuselage, GLU_OUTSIDE);
                gluSphere(fuselage, this->radius, 16, 16);
            gluDeleteQuadric(fuselage);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Enemy::drawHourglass(GLfloat length) {
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

void Enemy::drawLeftPropeller() {
    glPushMatrix();
        glTranslatef(.0, this->radius * .5, .0);
        glRotatef(90, .0, 1.0, .0);

        GLUquadricObj* stem = gluNewQuadric();
            glColor3f(0.0, 0.0, 0.0);
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

void Enemy::drawRightPropeller() {
    glPushMatrix();
        glTranslatef(.0, - this->radius * .5, .0);
        glRotatef(90, .0, 1.0, .0);

        GLUquadricObj* stem = gluNewQuadric();
            glColor3f(0.0, 0.0, 0.0);
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

void Enemy::drawFin() {
    glPushMatrix();
        glTranslatef(-this->radius * .65, .0, .0);
        glScalef(.25, .0625, .5);
        glTranslatef(.0, .0, this->radius * .5);

        glColor3f(0.0, 0.0, 0.0);
        glutSolidCube(this->radius);
    glPopMatrix();
}

void Enemy::drawCockpit() {
    glPushMatrix();
        glTranslatef(this->radius * .5, .0, .0);
        glScalef(.375, .125, .25);

        glColor3f(0.0, 0.0, 0.0);
        glutSolidSphere(this->radius, 20, 20);
    glPopMatrix();
}

/* Desenha todo o corpo do aviao */
void Enemy::drawAirplane(GLuint airEnemiesTexture, GLuint projTexture) {
    drawProjectiles(projTexture);

    glPushMatrix();
        glTranslatef(this->cx, this->cy, this->cz);
        glRotatef(this->angle, .0, .0, 1.0);
        glRotatef(-this->angleTheta, .0, 1.0, .0);

        GLfloat materialEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        
        drawLeftPropeller();
        drawRightPropeller();

        drawWings();
        drawCannon();
        drawCockpit();
        drawFin();
        
        GLfloat materialEmission2[] = {0.1, 0.1, 0.1, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission2);
    
        drawFuselage(airEnemiesTexture);
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

    angleTheta = 0.0;

    cx = startX;
    cy = startY;
    cz = startZ;

    projectiles.clear();
}

void Enemy::kill() {
    this->dead = true;

    projectiles.clear();
}