#include <iostream>
#include <string>
#include <cmath>
#include <GL/glut.h>
#include "tinyxml2.h"
#include "config_reader.h"
#include "arena.h"

using namespace std;
using namespace tinyxml2;

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
GLfloat speedMultiplier;
GLfloat oldTimeTakeOff;
GLfloat oldTimeFlying;

int main(int argc, char** argv) {
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

    arena = new Arena(svgPath);

    player = arena->getPlayer();
    player->calculatePhysics();
    
    speedMultiplier = configReader.getVelocidadeJogador();

    /* Glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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

    /* Inicializar sistema de viz */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
        arena->getCx() - arena->getRadius(),
        arena->getCx() + arena->getRadius(),
        arena->getCy() - arena->getRadius(),
        arena->getCy() + arena->getRadius(), -1.0, 1.0);
}

void display(void) {
    /* Limpar todos os pixels */
    glClear(GL_COLOR_BUFFER_BIT);

    arena->draw();

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
    }

    if (player->isDead()) {
        return;
    }

    if (player->isFlying()) {
        GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
        GLfloat diffTime = currentTime - oldTimeFlying;
        oldTimeFlying = currentTime;

        // A
        if (keyboard['a']) {
            player->moveX(90, diffTime);
        }

        // D
        if (keyboard['d']) {
            player->moveX(-90, diffTime);
        }

        player->updateProjectiles(speedMultiplier, diffTime);
        player->move(speedMultiplier, diffTime);

    } else {
        if (keyboard['u'] && !player->isTakeOff()) {
            player->setTakeOff(true);
            oldTimeTakeOff = glutGet(GLUT_ELAPSED_TIME);
        }

        if (player->isTakeOff()) {
            int currentTime = glutGet(GLUT_ELAPSED_TIME) - oldTimeTakeOff;

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
    x += 200;

    if (!player->isTakeOff() && player->isFlying() && !player->isDead()) {
        GLfloat distance = player->getMouseX() - x;

        if (player->getCannonAngle() + distance >= -45 && player->getCannonAngle() + distance <= 45) {
            player->setCannonAngle(player->getCannonAngle() + distance);
        }
    }

    player->setMouseX(x);
}

void mouseAction(int button, int state, int x, int y) {
    if (player->isFlying() && !player->isTakeOff() && !player->isDead()) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            player->fire(2.0);
        }

        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        }
    }
}