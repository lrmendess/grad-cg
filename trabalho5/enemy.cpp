#include "enemy.h"

Enemy::~Enemy() {
    for (Projectile* projectile : this->projectiles) {
        delete(projectile);
    }
}

Enemy::Enemy(Arena* arena, GLfloat cx, GLfloat cy, GLfloat radius) {
    this->cx = cx;
    this->cy = cy;
    this->cz = arena->getRadius() / 2.5;
    this->radius = radius;
    this->arena = arena;

    this->startX = cx;
    this->startY = cy;
    this->startZ = this->cz;
    this->cannonLength = CANNON_LENGTH;

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

    GLfloat step = mul * this->speed * dt;

    GLfloat mx = this->cx + step * cos(thetaRad) * cos(fiRad);
    GLfloat my = this->cy + step * cos(thetaRad) * sin(fiRad);
    GLfloat mz = this->cz + step * sin(thetaRad);

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if (distanceFromBorder > arena->getRadius()) {
        GLfloat alpha = atan2(this->cy, this->cx) * 180 / M_PI;
        GLfloat beta = (-2) * (alpha - this->angle - 90) * M_PI / 180;
        
        mx = cx * cos(beta) - cy * sin(beta);
        my = cx * sin(beta) + cy * cos(beta);
    }

    this->propellerAngle += this->speed / 8;
    this->cy = my;
    this->cx = mx;

    GLfloat groundEnemyHeight = .0;
    if (arena->getGroundEnemies().size() > 0) {
        groundEnemyHeight = arena->getGroundEnemies().front()->getRadius() * 2;
    }

    if (mz <= arena->getRadius() * .9 && mz > groundEnemyHeight) {
        this->cz = mz;
    }
}

void Enemy::drawProjectiles(GLuint projTexture) {
    for (auto p : this->projectiles) {
        glPushMatrix();
            glTranslatef(p->getCx(), p->getCy(), p->getCz());
            // glRotatef(p->getAngle(), .0, .0, 1.0);
            // glRotatef(-p->getAngleTheta(), .0, 1.0, .0);
            p->draw(projTexture);
        glPopMatrix();
    }
}

void Enemy::fire(GLfloat mul, GLfloat mulVelAirplane, GLfloat dt) {
    currentTimeFire += dt;

    if (currentTimeFire >= maxTimeFire) {
        Projectile* projectile = new Projectile(this, mul, mulVelAirplane);
        projectiles.push_back(projectile);

        currentTimeFire = 0.0;
    }
}

void Enemy::updateProjectiles(GLfloat dt) {
    list<Projectile*> forRemove;

    for (auto p : this->projectiles) {
        bool canUpdate = true;
        GLfloat projectileFiRad = p->getAngle() * M_PI / 180;
        GLfloat projectileThetaRad = p->getAngleTheta() * M_PI / 180;
        
        GLfloat step = p->getSpeed() * dt;

        GLfloat my = p->getCy() + step * cos(projectileThetaRad) * sin(projectileFiRad);
        GLfloat mx = p->getCx() + step * cos(projectileThetaRad) * cos(projectileFiRad);
        GLfloat mz = p->getCz() + step * sin(projectileThetaRad);

        // Se o projetil encostar na borda, ele sera removido da lista de projeteis
        // disparados pelo player em questao
        GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

        Player* player = arena->getPlayer();

        GLfloat distanceFromPlayer = d2p3d(mx, my, mz, player->getCx(), player->getCy(), player->getCz());

        if (distanceFromBorder > arena->getRadius()) {
            forRemove.push_back(p);
        } else if(distanceFromPlayer <= player->getRadius() && player->isFlying()) {
            player->kill();
        } else {
            p->setCx(mx);
            p->setCy(my);
            p->setCz(mz);
        }
    }

    // Sendo removido da lista e tendo sua memoria liberada, ele nao sera mais desenhado
    for (auto p : forRemove) {
        this->projectiles.remove(p);
        delete(p);
    }
}

void Enemy::drawWings() {
    GLfloat wingsEmission[] = {0.0, 0.0, 0.0, 1.0};
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

void Enemy::drawCannon() {
    GLfloat cannonEmission[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat cannonColor[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, cannonEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cannonColor);
    glColor3f(0.0, 0.0, 0.0);
    
    glPushMatrix();
		glTranslatef(this->radius, 0.0, 0);
        glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(-this->cannonAngle, 1.0, 0.0, 0.0);

        GLUquadricObj* cannon = gluNewQuadric();
            gluQuadricDrawStyle(cannon, GLU_FILL);
            gluQuadricNormals(cannon, GLU_SMOOTH);
            gluQuadricTexture(cannon, GLU_FALSE);
            gluQuadricOrientation(cannon, GLU_OUTSIDE);
            gluCylinder(cannon, this->radius / 12, this->radius / 12, this->cannonLength, 16, 16);
        gluDeleteQuadric(cannon);
    glPopMatrix();
}

void Enemy::drawFuselage(GLuint airEnemiesTexture) {
    GLfloat fuselageEmission[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat fuselageColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, fuselageEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, fuselageColor);
    glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();
        glScalef(1.0, .25, .25);
        
        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, airEnemiesTexture);
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

void Enemy::drawHourglass(GLfloat length) {
    GLfloat hourglassEmission[] = {0.0, 0.0, 0.0, 1.0};
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

void Enemy::drawLeftPropeller() {
    GLfloat stemEmission[] = {0.0, 0.0, 0.0, 1.0};
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

void Enemy::drawRightPropeller() {
    GLfloat stemEmission[] = {0.0, 0.0, 0.0, 1.0};
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

void Enemy::drawFin() {
    GLfloat finEmission[] = {0.0, 0.0, 0.0, 1.0};
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

void Enemy::drawCockpit() {
    GLfloat cockpitEmission[] = {0.0, 0.0, 0.0, 1.0};
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
void Enemy::drawAirplane(GLuint airEnemiesTexture, GLuint projTexture) {
    drawProjectiles(projTexture);

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