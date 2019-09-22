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

bool keyboard[256];

/* Arena Content */
Arena* arena;

int main(int argc, char** argv) {
    /* Abertura e tratamento do arquivo de configuracao */
    if (argc <= 1) {
        cout << "Nao esqueça de especificar o arquivo de configuracao." << endl;
        exit(EXIT_FAILURE);
    }

    string configPath = strcat(argv[1], "/config.xml");

    ConfigReader configReader(configPath);

    string svgPath = configReader.getCaminhoArquivoArena() + "/"
                    + configReader.getNomeArquivoArena() + "."
                    + configReader.getTipoArquivoArena();

    arena = new Arena(svgPath);

    // arena = svgReader.getArena();
    // player = svgReader.getPlayer();
    // airstrip = svgReader.getAirstrip();
    // groundEnemies = svgReader.getGroundEnemies();
    // airEnemies = svgReader.getAirEnemies();

    /* Glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ace Combat: Poor Edition");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);

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
    if (keyboard['w']) {
		// TODO anything
    }
    
    if (keyboard['a']) {
		// TODO anything
    }
    
    if (keyboard['s']) {
		// TODO anything
    }
    
    if (keyboard['d']) {
		// TODO anything
    }

    if (keyboard['u']) {
		// TODO anything
    }

    glutPostRedisplay();
}