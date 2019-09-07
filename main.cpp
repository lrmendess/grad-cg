#include <iostream>
#include <string>
#include <list>
#include <math.h>
#include <GL/glut.h>
#include "cursor.h"
#include "circle.h"
#include "color.h"
#include "screen.h"
#include "tinyxml2.h"

/* FUNCTIONS HEADER */
void init(void);
void display(void);
void cursorMovement(int x, int y);
void cursorClick(int button, int state, int x, int y);
void dragAndDropCircle(int x, int y);

float d2p(float ax, float ay, float bx, float by);
bool fitsCircle(float cx, float cy);
Circle* getSelectedCircle(int cx, int cy);

Screen* screen = nullptr;
Cursor* cursor = nullptr;
Circle* circle = nullptr;

std::list<Circle*> circles;

Circle* selectedCircle = nullptr;
int dsx = 0; /* Distancia X entre o centro do circulo selecionado e o cursor */
int dsy = 0; /* Distancia Y entre o centro do circulo selecionado e o cursor */

int main(int argc, char** argv) {
    /* Abertura e tratamento do arquivo de configuracao */
    if (argc <= 1) {
        std::cout << "Nao esquece de especificar o arquivo de configuracao, meu guerreiro." << std::endl;
        exit(EXIT_FAILURE);
    }

    tinyxml2::XMLDocument doc;

    string configPath = strcat(argv[1], "/config.xml");

    if (doc.LoadFile(configPath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cout << "Erro ao tentar abrir o arquivo config.xml." << std::endl;
        exit(EXIT_FAILURE);
    }


    /* Leitura da raiz do XML */
    tinyxml2::XMLElement* root = doc.RootElement();


    /* Leitura da Janela */
    tinyxml2::XMLElement* windowNode = root->FirstChildElement("janela");

    tinyxml2::XMLElement* dimensionNode = windowNode->FirstChildElement("dimensao");
    tinyxml2::XMLElement* backgroundColorNode = windowNode->FirstChildElement("fundo");
    tinyxml2::XMLElement* titleNode = windowNode->FirstChildElement("titulo");

    int width = std::stoi(dimensionNode->Attribute("largura"));
    int height = std::stoi(dimensionNode->Attribute("altura"));

    Color backgroundColor (
        std::stof(backgroundColorNode->Attribute("corR")),
        std::stof(backgroundColorNode->Attribute("corG")),
        std::stof(backgroundColorNode->Attribute("corB"))
    );

    std::string title = titleNode->GetText();

    screen = new Screen(width, height, backgroundColor, title);
    

    /* Leitura do Circulo */
    tinyxml2::XMLElement* circleNode = root->FirstChildElement("circulo");

    float radius = std::stof(circleNode->Attribute("raio"));

    Color circleColor (
        std::stof(circleNode->Attribute("corR")),
        std::stof(circleNode->Attribute("corG")),
        std::stof(circleNode->Attribute("corB"))
    );

    circle = new Circle(radius, 0, 0, circleColor);


    /* Leitura do Cursor */
    tinyxml2::XMLElement* modelCircleNode = root->FirstChildElement("circuloModelo");

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


    /* Glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(screen->getWidth(), screen->getHeight());
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title.c_str());

    init();

    glutDisplayFunc(display);
    glutPassiveMotionFunc(cursorMovement);
    glutMotionFunc(dragAndDropCircle);
    glutMouseFunc(cursorClick);

    glutMainLoop();

    delete(screen);
    delete(cursor);
    delete(circle);

    for (Circle* c : circles) {
        delete(c);
    }

    return 0;
}

void display(void) {
    /* Limpar todos os pixels */
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto c : circles) {
        c->drawSolid();
    }

    if (cursor->isVisible() && (!cursor->rightButtonIsPressed() || selectedCircle == nullptr)) {
        if (fitsCircle(cursor->getX(), cursor->getY())) {
            cursor->draw();   
        } else {
            cursor->draw(cursor->getOverlapColor());
        }
    }

    /* Nao esperar! */
    glFlush();
}

void init(void) {
    /* Seleciona cor de fundo */
    Color& backgroundColor = screen->getColor();

    glClearColor (
        backgroundColor.getRed(),
        backgroundColor.getGreen(),
        backgroundColor.getBlue(),
        backgroundColor.getAlpha()
    );

    /* Inicializar sistema de viz */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, screen->getWidth(), 0.0, screen->getHeight(), -1.0, 1.0);
}

void cursorMovement(int x, int y) {
    cursor->makeVisible();

    y = screen->getHeight() - y;

    cursor->setX(x);
    cursor->setY(y);

    glutPostRedisplay();
}

void cursorClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = screen->getHeight() - y;

        if (fitsCircle(x, y)) {
            Circle* newCircle = new Circle (
                circle->getRadius(), x, y, circle->getColor()
            );

            circles.push_front(newCircle);
        }
    }
    
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            selectedCircle = getSelectedCircle(x, y);
            cursor->pressRightButton();
        }
        
        if (state == GLUT_UP) {
            selectedCircle = nullptr;
            cursor->unpressRightButton();
        }
    }

    glutPostRedisplay();
}

void dragAndDropCircle(int x, int y) {
    y = screen->getHeight() - y;

    cursor->setX(x);
    cursor->setY(y);

    if (cursor->rightButtonIsPressed()) {
        if (selectedCircle != nullptr) {
            selectedCircle->setX(x - dsx);
            selectedCircle->setY(y - dsy);
        }
    }

    glutPostRedisplay();
}

float d2p(float ax, float ay, float bx, float by) {
    return sqrt(pow(bx - ax, 2) + pow(ay - by, 2));
}

bool fitsCircle(float cx, float cy) {
    for (auto c : circles) {
        float distance = d2p(cx, cy, c->getX(), c->getY());

        if (distance <= (c->getRadius() * 2)) {
            return false;
        }
    }

    return true;
}

Circle* getSelectedCircle(int x, int y) {
    y = screen->getHeight() - y;

    for (auto c : circles) {
        float distance = d2p(x, y, c->getX(), c->getY());

        dsx = x - c->getX();
        dsy = y - c->getY();

        if (distance <= c->getRadius()) {
            return c;
        }
    }

    return nullptr;
}
