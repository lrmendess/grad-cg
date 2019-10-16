#include "arena.h"

Arena::~Arena() {
    delete(player);
    
    delete(airstrip);

    for (Circle* c : groundEnemies)
        delete(c);

    for (Circle* c : airEnemies)
        delete(c);
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
            GLfloat radius = stof(findBlueCircle->Attribute("r"));
            GLfloat cx = stoi(findBlueCircle->Attribute("cx"));
            GLfloat cy = 2 * radius - stoi(findBlueCircle->Attribute("cy"));

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

    /* Linha */
    XMLElement* l = root->FirstChildElement("line");

    GLfloat x1 = stoi(l->Attribute("x1"));
    GLfloat y1 = 2 * this->getRadius() - stoi(l->Attribute("y1"));
    GLfloat x2 = stoi(l->Attribute("x2"));
    GLfloat y2 = 2 * this->getRadius() - stoi(l->Attribute("y2"));
    GLfloat color[3] = { 0, 0, 0 };

    airstrip = new Line(x1, y1, x2, y2, color);

    /* Circulos */
    XMLElement* c = root->FirstChildElement("circle");

    while (c != NULL) {
        GLfloat radius = stof(c->Attribute("r"));
        GLfloat cx = stoi(c->Attribute("cx"));
        GLfloat cy = 2 * this->getRadius() - stoi(c->Attribute("cy"));

        string colorName = c->Attribute("fill");

        if (!colorName.compare("red"))
            airEnemies.push_back(new Circle(cx, cy, radius, RED));
        
        if (!colorName.compare("green"))
            player = new Player(this, cx, cy, radius);
        
        if (!colorName.compare("orange"))
            groundEnemies.push_back(new Circle(cx, cy, radius, ORANGE));

        /* Next */
        c = c->NextSiblingElement("circle");
    }
}

void Arena::draw() {
    drawSolidCircle();

    airstrip->drawSolidLine();
    
    for (auto ge : groundEnemies) {
        ge->drawSolidCircle();
    }

    for (auto ae : airEnemies) {
        ae->drawSolidCircle();
    }

    player->drawAirplane();
}
