#include "arena.h"

Arena::~Arena() {
    
}

Arena::Arena(string path) : Circle() {
    /* Abertura SVG */
    XMLDocument doc;

    if (doc.LoadFile(path.c_str()) != XML_SUCCESS) {
        cout << "Erro ao tentar abrir o arquivo " << path << endl;
        exit(EXIT_FAILURE);
    }

    /* Raiz SVG */
    XMLElement* root = doc.RootElement();

    /* Circulos */
    XMLElement* c = root->FirstChildElement("circle");

    while (c != NULL) {
        int cx = stoi(c->Attribute("cx"));
        int cy = stoi(c->Attribute("cy"));
        GLfloat radius = stof(c->Attribute("r"));

        string colorName = c->Attribute("fill");

        if (colorName.compare("red") == 0) {
            airEnemies.push_back(new Circle(cx, cy, radius, RED));
            
        } else if (colorName.compare("green") == 0) {
            player = new Circle(cx, cy, radius, GREEN);

        } else if (colorName.compare("blue") == 0) {
            this->setCx(cx);
            this->setCy(cy);
            this->setRadius(radius);
            this->setColor(BLUE);

        } else if (colorName.compare("orange") == 0) {
            groundEnemies.push_back(new Circle(cx, cy, radius, ORANGE));
        }

        /* Next */
        c = c->NextSiblingElement("circle");
    }

    /* Linha */
    XMLElement* l = root->FirstChildElement("line");

    int x1 = stoi(l->Attribute("x1"));
    int y1 = stoi(l->Attribute("y1"));
    int x2 = stoi(l->Attribute("x2"));
    int y2 = stoi(l->Attribute("y2"));
    float color[3] = { 0, 0, 0 };

    airstrip = new Line(x1, y1, x2, y2, color);
}

Circle* Arena::getPlayer() {
    return player;
}

void Arena::draw() {
    this->drawSolidCircle();
}