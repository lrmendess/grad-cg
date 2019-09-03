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
void cursorMovement(int x, int y);
void cursorClick(int button, int state, int x, int y);
void dragAndDropCircle(int button, int state, int x, int y);
float d2p(float ax, float ay, float bx, float by);
bool fitsCircle(float cx, float cy);

Screen* screen = nullptr;
Cursor* cursor = nullptr;
Circle* baseCircle = nullptr;
vector<Circle*> circles;

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

    float radius = std::stof(circleNode->Attribute("raio")) / screen->getWidth();

    Color circleColor (
        std::stof(circleNode->Attribute("corR")),
        std::stof(circleNode->Attribute("corG")),
        std::stof(circleNode->Attribute("corB"))
    );

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
    baseCircle = new Circle(radius, 0, 0, circleColor);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(screen->getWidth(), screen->getHeight());
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title.c_str());

    init();

    glutDisplayFunc(display);
    glutPassiveMotionFunc(cursorMovement);
    glutMouseFunc(cursorClick);

    glutMainLoop();

    /* C ANSI requer que main retorne um inteiro */
    return 0;
}

void display(void) {
    /* Limpar todos os pixels */
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto circle : circles) {
        circle->drawSolid();
    }

    if (fitsCircle(cursor->getX(), cursor->getY())) {
        cursor->draw();   
    } else {
        cursor->draw(cursor->getOverlapColor());
    }

    /* Nao esperar! */
    glFlush();
}

void init(void) {
    /* Seleciona cor de fundo */
    Color backgroundColor = screen->getColor();

    glClearColor (
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

void cursorMovement(int x, int y) {
    float cursorPositionX = (float) x / screen->getWidth();
    float cursorPositionY = 1 - (float) y / screen->getHeight();

    cursor->setX(cursorPositionX);
    cursor->setY(cursorPositionY);

    glutPostRedisplay();
}

void cursorClick(int button, int state, int x, int y) {
    float cursorClickPositionX = (float) x / screen->getWidth();
    float cursorClickPositionY = 1 - (float) y / screen->getHeight();

    if (button == GLUT_LEFT_BUTTON && fitsCircle(cursorClickPositionX, cursorClickPositionY)) {
        Circle* newCircle = new Circle (
            baseCircle->getRadius(),
            cursorClickPositionX,
            cursorClickPositionY,
            baseCircle->getColor()
        );

        circles.push_back(newCircle);
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        printf("Down\n");
    }

    glutPostRedisplay();
}

float d2p(float ax, float ay, float bx, float by) {
    return sqrt(pow(bx - ax, 2) + pow(ay - by, 2));
}

bool fitsCircle(float cx, float cy) {
    for (auto circle : circles) {
        if (d2p(cx, cy, circle->getX(), circle->getY()) <= (2 * circle->getRadius())) {
            return false;
        }
    }

    return true;
}
