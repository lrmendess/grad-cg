#include "line.h"

Line::~Line() {

}

Line::Line() {

}

Line::Line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

void Line::drawSolidLine(GLuint airstripTexture) {
    glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
            glScalef(2.0, 1.0, 1.0);

            glBindTexture(GL_TEXTURE_2D, airstripTexture);
            glColor3f(1.0, 1.0, 1.0);

            glBegin(GL_QUADS);
                glNormal3f(0.0, 0.0, 1.0);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(x1, y1 - 30.0, 0.1);
                glNormal3f(0.0, 0.0, 1.0);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(x2, y2 - 30.0, 0.1);
                glNormal3f(0.0, 0.0, 1.0);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(x2, y2 + 30.0, 0.1);
                glNormal3f(0.0, 0.0, 1.0);
                glTexCoord2f(0.0, 1.0);
                glVertex3f(x1, y1 + 30.0, 0.1);
            glEnd();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glDisable(GL_TEXTURE_2D);
}
