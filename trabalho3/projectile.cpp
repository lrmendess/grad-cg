#include "projectile.h"

Projectile::Projectile() {

}

Projectile::~Projectile() {

}

void Projectile::draw() {
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
        GLfloat a, px, py;

        for (int i = 0; i < 360; i += 18) {
            a = (i * M_PI) / 180.0;
            px = this->cx + cos(a) * this->length;
            py = this->cy + sin(a) * this->length;
            glVertex2f(px, py);
        }
    glEnd();
}