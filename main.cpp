#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <GL/glut.h>
#include "cursor.h"
#include "circle.h"
#include "color.h"
#include "screen.h"
#include "tinyxml/tinyxml.h"

void init(void);
void display(void);
void mouse(int x, int y);

Screen* screen = nullptr;
Cursor* cursor = nullptr;

vector<Circle> circles;

int main(int argc, char** argv) {
    TiXmlDocument doc("test1/config.xml");
    doc.LoadFile();

    TiXmlElement* root = doc.RootElement();

    /* Janela */
    TiXmlElement* windowNode = root->FirstChildElement("janela");

    TiXmlElement* dimensionNode = windowNode->FirstChildElement("dimensao");
    TiXmlElement* backgroundColorNode = windowNode->FirstChildElement("fundo");
    TiXmlElement* titleNode = windowNode->FirstChildElement("titulo");

    int width = std::stoi(dimensionNode->Attribute("largura"));
    int height = std::stoi(dimensionNode->Attribute("altura"));

    Color backgroundColor (
        std::stof(backgroundColorNode->Attribute("corR")),
        std::stof(backgroundColorNode->Attribute("corG")),
        std::stof(backgroundColorNode->Attribute("corB"))
    );

    std::string title = titleNode->GetText();

    screen = new Screen(width, height, backgroundColor, title);
    
    /* Circulos e Cursor */
    TiXmlElement* circleNode = root->FirstChildElement("circulo");
    TiXmlElement* modelCircleNode = root->FirstChildElement("circuloModelo");

    float radius = 1 / std::stof(circleNode->Attribute("raio"));

    Color cursorColor (
        std::stof(modelCircleNode->Attribute("corR")),
        std::stof(modelCircleNode->Attribute("corG")),
        std::stof(modelCircleNode->Attribute("corB"))
    );

    Color cursorOverlapColor (
        std::stof(modelCircleNode->Attribute("corSobreposicaoR")),
        std::stof(modelCircleNode->Attribute("corSobreposicaoG")),
        std::stof(modelCircleNode->Attribute("corSobreposicaoB"))
    );

    cursor = new Cursor(radius, 0, 0, cursorColor, cursorOverlapColor);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(screen->getWidth(), screen->getHeight());
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title.c_str());

    init();

    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouse);

    glutMainLoop();

    /* C ANSI requer que main retorne um inteiro */
    return 0;
}

void display(void) {
    /* Limpar todos os pixels */
    glClear(GL_COLOR_BUFFER_BIT);

    cursor->draw();

    /* Nao esperar! */
    glFlush();
}

void init(void) {
    /* Seleciona cor de fundo */
    Color backgroundColor = screen->getColor();

    glClearColor(
        backgroundColor.getRed(),
        backgroundColor.getGreen(),
        backgroundColor.getBlue(),
        backgroundColor.getAlpha()
    );

    /* Inicializar sistema de viz */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void mouse(int x, int y) {
    float newX = (float) x / screen->getWidth();
    float newY = (float) y / screen->getHeight();

    cursor->setX(newX);
    cursor->setY(1 - newY);

    glutPostRedisplay();

    // printf("(%.2f, %.2f)\n", cursor->getX(), cursor->getY());
}
