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
    this->dead = false;

    // Armazenamento do estado inicial antes da decolagem
    this->startX = this->cx;
    this->startY = this->cy;
    this->startR = radius;

    // Angulo inicial
    this->angle = 1 / M_PI * 180 * atan2(
        arena->getAirstrip().getY2() - arena->getAirstrip().getY1(),
        arena->getAirstrip().getX2() - arena->getAirstrip().getX1()
    );
}

void Player::moveX(const GLfloat& angle, const GLfloat& dt) {
    if (this->dead) {
        return;
    }
    
    this->angle += angle * dt;
}

void Player::move(const GLfloat& mulX, const GLfloat& mulY, const GLfloat& dt) {
    GLfloat my = this->cy + sin(this->angle * M_PI / 180) * (mulY * sin(M_PI / 4) * this->speed * dt);
    GLfloat mx = this->cx + cos(this->angle * M_PI / 180) * (mulX * cos(M_PI / 4) * this->speed * dt);

    GLfloat distanceFromBorder = d2p(mx, my, arena->getCx(), arena->getCy());

    if (distanceFromBorder > arena->getRadius()) {
        //GLfloat xa = this->cx;
        //GLfloat ya = this->cy;
        //GLfloat m = tan(this->angle);
        //GLfloat r = arena->getRadius();

        //GLfloat x1 = -(2 * m * (ya - m * xa)) + sqrt(pow((2 * m * (ya - m * xa)), 2) - 4 * (pow(m, 2) + 1) * (pow((ya - m * xa), 2) - pow(r, 2))) / (2 * (pow(m, 2) + 1));
        //GLfloat x2 = -(2 * m * (ya - m * xa)) - sqrt(pow((2 * m * (ya - m * xa)), 2) - 4 * (pow(m, 2) + 1) * (pow((ya - m * xa), 2) - pow(r, 2))) / (2 * (pow(m, 2) + 1));

        //cout << "Parametros: " << xa << ", " << ya << ", " << m << ", " << r << endl;
        //cout << "Resultados: " << x1 << ", " << x2 << endl;

        return;
    }

    for (auto enemy : arena->getAirEnemies()) {
        GLfloat distanceFromEnemy = d2p(mx, my, enemy->getCx(), enemy->getCy());

        GLfloat safetyDistance = enemy->getRadius() + this->radius;
        if ((distanceFromEnemy <= safetyDistance) && this->flying) {
            this->dead = true;
            return;
        }
    }

    this->propellerAngle += 5;
    this->cy = my;
    this->cx = mx;
}

/* Calcula a fisica a ser utilizada no aviao */
void Player::calculatePhysics() {
    // S = So + Vo * t + (a * t^2)/2
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
    this->midAirstripTime = sqrt(2 * (distance - startDistance) / acc);

    // V = S / t
    radiusSpeed = this->radius / (t - midAirstripTime);
}

/* Decolagem do aviao */
void Player::takeOffAirplane(GLint& currentTime) {
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
        glBegin(GL_POLYGON);
            glVertex3f(this->radius,        -this->radius / 12, 0.0);
            glVertex3f(this->radius * 1.5,  -this->radius / 12, 0.0);
            glVertex3f(this->radius * 1.5,   this->radius / 12, 0.0);
            glVertex3f(this->radius,         this->radius / 12, 0.0);
        glEnd();
    glPopMatrix();
}

void Player::drawFuselage() {
    drawEllipse(this->radius, this->color);
}

void Player::drawTriangles(GLfloat length) {
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(length, length, 0.0);
		glVertex3f(-length, length, 0.0);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(length, -length, 0.0);
		glVertex3f(-length,  -length, 0.0);
	glEnd();
}

void Player::drawLeftPropeller() {
    GLfloat propAngle = this->propellerAngle;

    glPushMatrix();
        glTranslatef(this->radius / 2, -this->radius / 2, 0);
		glPushMatrix();
			glRotatef(propAngle, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
            glPushMatrix();
			glRotatef(propAngle + 90, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(propAngle + 180, 1.0, 0.0, 0.0);
			drawTriangles(this->radius / 4);
		glPopMatrix();
		glPushMatrix();
			glRotatef(propAngle + 270, 1.0, 0.0, 0.0);
            drawTriangles(this->radius / 4);
		glPopMatrix();
    glPopMatrix();
}

void Player::drawRightPropeller() {
    glPushMatrix();
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
            glVertex3f(-this->radius * 0.9,       -this->radius / 12, 0.0);
            glVertex3f(-this->radius / 3,   -this->radius / 12, 0.0);
            glVertex3f(-this->radius / 3,    this->radius / 12, 0.0);
            glVertex3f(-this->radius * 0.9,        this->radius / 12, 0.0);
        glEnd();
    glPopMatrix();
}

void Player::drawCockpit() {
    glPushMatrix();
        glTranslatef(this->radius / 2, 0.0, 0.0);
        GLfloat black[3] = { 0.0, 0.0, 0.0 };
        drawEllipse(this->radius / 3, black);
    glPopMatrix();
}

/* Desenha todo o corpo do aviao */
void Player::drawAirplane() {    
    glPushMatrix();
        glTranslatef(this->cx, this->cy, 0);
        glRotatef(this->angle, 0, 0, 1);

        drawWings();
        drawLeftPropeller();
        drawRightPropeller();
        drawFuselage();
        drawFin();
        drawCockpit();
        drawCannon();

        /* [INICIO] Circulo de Colisao (TEMP) */
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
            GLfloat angle, px, py;

            for (int i = 0; i < 360; i++) {
                angle = (i * M_PI) / 180.0;
                px = cos(angle) * this->radius;
                py = sin(angle) * this->radius;
                glVertex2f(px, py);
            }
        glEnd();
        /* [FIM] Circulo de Colisao (TEMP) */

    glPopMatrix();
}

/* Reseta o player para as condicoes iniciais */
void Player::reset() {
    this->cx = this->startX;
    this->cy = this->startY;
    this->radius = this->startR;

    this->angle = 1 / M_PI * 180 * atan2(
        arena->getAirstrip().getY2() - arena->getAirstrip().getY1(),
        arena->getAirstrip().getX2() - arena->getAirstrip().getX1()
    );

    this->dead = false;
    this->speed = 0;
    this->flying = false;
    this->takeOff = false;

    this->calculatePhysics();
}