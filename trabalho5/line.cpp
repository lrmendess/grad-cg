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
    GLfloat dist = abs(x1-x2);
    GLfloat angle = (180 / M_PI) * atan2(y2 - y1, x2 - x1);
    
    glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glTranslatef(x1, y1, 0.0);
            glRotatef(angle, 0.0, 0.0, 1.0);
            
            glMatrixMode(GL_TEXTURE);
            glPushMatrix();
                glScalef(0.1, 1.0, 1.0);

                glBindTexture(GL_TEXTURE_2D, airstripTexture);
                glColor3f(1.0, 1.0, 1.0);

                glBegin(GL_QUAD_STRIP);
                    glNormal3f(0.0, 0.0, 1.0);
                    glTexCoord2f(0.0, 0.0);
                    glVertex3f(-6 * dist / 50, -30.0, 0.1);
                    
                    glNormal3f(0.0, 0.0, 1.0);
                    glTexCoord2f(0.0, 1.0);
                    glVertex3f(-6 * dist / 50, 30.0, 0.1);
                        
                    int tex = 0;
                        
                    for (int i = -5 * dist / 50; i <= dist; i += dist / 50) {
                        glNormal3f(0.0, 0.0, 1.0);
                        glTexCoord2f(1.0 + tex, 0.0);
                        glVertex3f(i, -30.0, 0.1);
                        
                        glNormal3f(0.0, 0.0, 1.0);
                        glTexCoord2f(1.0 + tex, 1.0);
                        glVertex3f(i, 30.0, 0.1);
                        
                        tex++;
                    }
                glEnd();
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
