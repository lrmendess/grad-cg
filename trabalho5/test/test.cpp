#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

double camDist=50;
double camXYAngle=0;
double camXZAngle=0;

void init (void) {
    glEnable(GL_DEPTH_TEST);
//    glEnable( GL_TEXTURE_2D );
//    glEnable(GL_LIGHTING);
//    glShadeModel (GL_FLAT);
//    glShadeModel (GL_SMOOTH);

    glDepthFunc(GL_LEQUAL);

    //textureEarth = LoadTextureRAW( "earth.bmp" );
    //textureSun = LoadTextureRAW( "sun1.bmp" );
    //texturePlane = LoadTextureRAW( "stars1.bmp" );

    //glEnable(GL_LIGHT0);
}

void display (void) {
    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0,0,-camDist);
    glRotatef(camXZAngle,1,0,0);
    glRotatef(camXYAngle,0,1,0);

    //GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // glEnable(GL_LIGHT1);
    // GLfloat light_position1[] = { 70.0, 70.0, 0.0, 1.0 };
    // GLfloat light1[] = {1,1,1,1};
    // glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    // glLightfv(GL_LIGHT1, GL_DIFFUSE, light1);
    
    glutSolidSphere(3, 10, 10);
    
    glutSwapBuffers();
}

int main (int argc, char **argv) {

    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize (500, 500);

    glutInitWindowPosition (100, 100);

    glutCreateWindow ("A basic OpenGL Window");

    init();

    glutDisplayFunc (display);
/*
    glutIdleFunc (display);

    glutReshapeFunc (reshape);
    
    glutKeyboardFunc(keyboard);
    
    glutMotionFunc(mouse_motion);
    glutMouseFunc(mouse_callback);
*/
    glutMainLoop ();

    return 0;
}
