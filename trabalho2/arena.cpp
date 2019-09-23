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

    /* Busca exclusivamente a arena antes de todos os outros objetos do jogo */
    XMLElement* findBlueCircle = root->FirstChildElement("circle");

    while (findBlueCircle != NULL) {
        string colorName = findBlueCircle->Attribute("fill");

        if (!colorName.compare("blue")) {
            int cx = stoi(findBlueCircle->Attribute("cx"));
            int cy = stoi(findBlueCircle->Attribute("cy"));
            GLfloat radius = stof(findBlueCircle->Attribute("r"));

            this->setCx(cx);
            this->setCy(cy);
            this->setRadius(radius);
            this->setColor(BLUE);

            break;
        }
    }

    /* Caso a arena nao seja encontrada o programa eh finalizado */
    if (findBlueCircle == NULL) {
        cout << "Nao foi possivel configurar a Arena" << endl;
        exit(EXIT_FAILURE);
    }

    /* Circulos */
    XMLElement* c = root->FirstChildElement("circle");

    while (c != NULL) {
        int cx = stoi(c->Attribute("cx"));
        int cy = stoi(c->Attribute("cy"));
        GLfloat radius = stof(c->Attribute("r"));

        string colorName = c->Attribute("fill");

        if (!colorName.compare("red"))
            airEnemies.push_back(new Circle(cx, cy, radius, RED));
        
        if (!colorName.compare("green"))
            player = new Circle(cx, cy, radius, GREEN);
        
        if (!colorName.compare("orange"))
            groundEnemies.push_back(new Circle(cx, cy, radius, ORANGE));

        /* Next */
        c = c->NextSiblingElement("circle");
    }

    /* Linha */
    XMLElement* l = root->FirstChildElement("line");

    int x1 = stoi(l->Attribute("x1"));
    int y1 = stoi(l->Attribute("y1"));
    int x2 = stoi(l->Attribute("x2"));
    int y2 = stoi(l->Attribute("y2"));
    GLfloat color[3] = { 0, 0, 0 };

    airstrip = new Line(x1, y1, x2, y2, color);
}

Circle* Arena::getPlayer() {
    return player;
}

void Arena::draw() {
    this->drawSolidCircle();

    airstrip->drawSolidLine();
    player->drawSolidCircle();
    
    for (auto ge : groundEnemies) {
        ge->drawSolidCircle();
    }

    for (auto ae : airEnemies) {
        ae->drawSolidCircle();
    }
}