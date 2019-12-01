#include <iostream>
#include <string>
#include <cmath>
#include <GL/glut.h>
#include <ctime>
#include "tinyxml2.h"
#include "config_reader.h"
#include "arena.h"
#include "imageloader.h"

using namespace std;
using namespace tinyxml2;

void renderBitmapString(float x, float y, void *font, string str);

void init(void);
void display(void);
void idle(void);

void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseMovement(int x, int y);
void mouseAction(int button, int state, int x, int y);

bool keyboard[256];

ConfigReader* configReader;

Arena* arena;
Player* player;
list<Enemy*> airEnemies;
list<Circle*> groundEnemies;
GLfloat speedMultiplier;
GLfloat fireSpeedMultiplier;
GLfloat enemyFireSpeedMultiplier;
GLfloat enemySpeedMultiplier;
GLfloat enemyFireFreq;
GLfloat oldTimeTakeOff;
GLfloat oldTimeFlying;

int main(int argc, char** argv) {
    srand(time(NULL));

    /* Abertura e tratamento do arquivo de configuracao */
    if (argc <= 1) {
        cout << "Nao esqueça de especificar o arquivo de configuracao." << endl;
        exit(EXIT_FAILURE);
    }

    string configPath = strcat(argv[1], "/config.xml");

    ConfigReader& configReader = *(new ConfigReader(configPath));

    string svgPath = configReader.getCaminhoArquivoArena() + "/"
                    + configReader.getNomeArquivoArena() + "."
                    + configReader.getTipoArquivoArena();
    
    speedMultiplier = configReader.getVelocidadeJogador();
    fireSpeedMultiplier = configReader.getVelocidadeTiro();

    enemySpeedMultiplier = configReader.getInimigoVel();
    enemyFireSpeedMultiplier = configReader.getInimigoVelTiro();
    enemyFireFreq = configReader.getInimigoFreqTiro();

    arena = new Arena(svgPath);

    player = arena->getPlayer();

    /* Copia dos inimigos aereos para interacao */
    for (auto a : arena->getAirEnemies()) {
        airEnemies.push_back(a);
        a->setFireFreq(enemyFireFreq);
    }

    for (auto g : arena->getGroundEnemies()) {
        groundEnemies.push_back(g);
    }
    
    /* Glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(2 * arena->getRadius(), 2 * arena->getRadius());
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ace Combat: Poor Edition");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouseMovement);
    glutMouseFunc(mouseAction);

    glutMainLoop();

    return 0;
}

void init(void) {
    /* Seleciona cor de fundo */
    glClearColor(1, 1, 1, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glDepthFunc(GL_LEQUAL);

    /* Inicializar sistema de viz */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspectRatio = ((GLfloat) arena->getRadius() * 2) / ((GLfloat) arena->getRadius() * 2);
    gluPerspective(90, aspectRatio, 1, arena->getRadius() * 2);
}

void display(void) {
    /* Limpar todos os pixels */
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        player->getCx(), player->getCy(), player->getRadius(),

        player->getCx() + (player->getRadius()) * cos(player->getAngle() * M_PI / 180),   
        player->getCy() + (player->getRadius()) * sin(player->getAngle() * M_PI / 180), 
        3,
        
        0, 0, 1
    );

    arena->draw();

    if (player->getPoints() == arena->getGroundEnemies().size()) {
        string result("WIN");
        renderBitmapString(-20.0, -5.0, GLUT_BITMAP_TIMES_ROMAN_24, result);
    } else if (player->isDead()) {
        string result("LOSE");
        renderBitmapString(-20.0, -5.0, GLUT_BITMAP_TIMES_ROMAN_24, result);
    }
    
    string totalGroundEnemies(to_string(arena->getGroundEnemies().size() - player->getPoints()));
    string score("Destroyed: " 
            + to_string(player->getPoints()) 
            + " | Remaining: " 
            + totalGroundEnemies);

    renderBitmapString(0.0, arena->getRadius() - 30, GLUT_BITMAP_TIMES_ROMAN_24, score);

    /* Nao esperar! */
    glutSwapBuffers();
}

void keyPress(unsigned char key, int x, int y) {
    keyboard[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyboard[key] = false;
}

void idle(void) {
    // R
    if (keyboard['r']) {
        player->reset();

        for (auto a : airEnemies) {
            a->reset();
        }

        for (auto g : groundEnemies) {
            g->reset();
        }

        for (int i = 0; i < 256; i++) {
            keyboard[i] = false;
        }
    }

    if (player->getPoints() == arena->getGroundEnemies().size()) {
        return;
    }

    if (player->isDead()) {
        return;
    }

    GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    GLfloat diffTime = currentTime - oldTimeFlying;
    oldTimeFlying = currentTime;

    for (auto a : airEnemies) {
        if (!a->isDead()) {
            GLint turn = rand() % 18;

            if (turn <= 2) {
                a->moveX(120, diffTime);
            } else if (turn <= 6) {
                a->moveX(-120, diffTime);
            }

            a->move(enemySpeedMultiplier, diffTime);
            // atira só depois de x tempos
            a->fire(enemyFireSpeedMultiplier, enemySpeedMultiplier, diffTime);
            a->updateProjectiles(diffTime);
        }
    }

    if (player->isFlying()) {
        // A
        if (keyboard['a']) {
            player->moveX(120, diffTime);
        }

        // D
        if (keyboard['d']) {
            player->moveX(-120, diffTime);
        }

        if (keyboard['='] || keyboard['+']) {
            player->setSpeed(player->getSpeed() + 200.0 * diffTime);
        }

        if (keyboard['-'] || keyboard['_']) {
            GLfloat speed = player->getSpeed() - 200.0 * diffTime;

            if (speed <= 0) {
                speed = 0;
            }

            player->setSpeed(speed);
        }

        player->updateProjectiles(diffTime);
        player->updateBombs(currentTime, diffTime);
        player->move(speedMultiplier, diffTime);

    } else {
        if (keyboard['u'] && !player->isTakeOff()) {
            player->setTakeOff(true);
            oldTimeTakeOff = glutGet(GLUT_ELAPSED_TIME);
        }

        if (player->isTakeOff()) {
            GLint currentTime = glutGet(GLUT_ELAPSED_TIME) - oldTimeTakeOff;

            player->takeOffAirplane(currentTime);

            if (currentTime > 4000) {
                player->setTakeOff(false);
                player->setFlying(true);
                oldTimeFlying = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
            }
        }
    }

    glutPostRedisplay();
}

void mouseMovement(int x, int y) {
    if (!player->isTakeOff() && player->isFlying() && !player->isDead()) {
        GLfloat distance = player->getMouseX() - x;

        if (player->getCannonAngle() + distance >= -45 && player->getCannonAngle() + distance <= 45) {
            player->setCannonAngle(player->getCannonAngle() + distance / 3);
        }
    }

    player->setMouseX(x);
}

void mouseAction(int button, int state, int x, int y) {
    if (player->isFlying() && !player->isTakeOff() && !player->isDead()) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            player->fire(fireSpeedMultiplier, speedMultiplier);
        }

        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
            player->bomb();
        }
    }
}

void renderBitmapString(float x, float y, void *font, string str) {
    glRasterPos2f(x,y);

    for (string::iterator c = (&str)->begin(); c != (&str)->end(); ++c)  {
        glutBitmapCharacter(font, *c);
    }
}

GLuint LoadTextureRAW(const char* filename) {
    GLuint texture;
    
    Image* image = loadBMP(filename);

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                            // Always GL_TEXTURE_2D
                             0,                            // 0 for now
                             GL_RGB,                       // Format OpenGL uses for image
                             image->width, image->height,  // Width and height
                             0,                            // The border of the image
                             GL_RGB,                       // GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE,             // GL_UNSIGNED_BYTE, because pixels are stored
                                                           // as unsigned numbers
                             image->pixels);               // The actual pixel data
    delete image;

    return texture;
}