#include "arena.h"

Arena::~Arena() {
    delete(player);
    
    delete(airstrip);

    for (Circle* c : groundEnemies) {
        delete(c);
    }

    for (Enemy* c : airEnemies) {
        delete(c);
    }
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

    /* 
     * Iremos compensar em X e Y os elementos da arena para que a mesma fique
     * na origem (0, 0)
     */
    GLfloat arenaX;
    GLfloat arenaY;

    while (findBlueCircle != NULL) {
        string colorName = findBlueCircle->Attribute("fill");

        if (!colorName.compare("blue")) {
            GLfloat radius = stof(findBlueCircle->Attribute("r"));
            arenaX = stoi(findBlueCircle->Attribute("cx"));
            arenaY = 2 * radius - stoi(findBlueCircle->Attribute("cy"));

            this->setCx(0.0);
            this->setCy(0.0);
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

    GLfloat x1 = stoi(l->Attribute("x1")) - arenaX;
    GLfloat y1 = 2 * this->getRadius() - stoi(l->Attribute("y1")) - arenaY;
    GLfloat x2 = stoi(l->Attribute("x2")) - arenaX;
    GLfloat y2 = 2 * this->getRadius() - stoi(l->Attribute("y2")) - arenaY;

    airstrip = new Line(x1, y1, x2, y2);

    /* Player */
    XMLElement* findGreenCircle = root->FirstChildElement("circle");

    while (findGreenCircle != NULL) {
        string colorName = findGreenCircle->Attribute("fill");
        GLfloat radius = stof(findGreenCircle->Attribute("r"));
        GLfloat cx = stoi(findGreenCircle->Attribute("cx")) - arenaX;
        GLfloat cy = 2 * this->getRadius() - stoi(findGreenCircle->Attribute("cy")) - arenaY;

        if (!colorName.compare("green")) {
            player = new Player(this, cx, cy, radius);
            break;
        }

        findGreenCircle = findGreenCircle->NextSiblingElement("circle");
    }

    /* Caso o player nao seja encontrada o programa eh finalizado */
    if (findGreenCircle == NULL) {
        cout << "Nao foi possivel configurar a Arena" << endl;
        exit(EXIT_FAILURE);
    }

    /* Circulos */
    XMLElement* c = root->FirstChildElement("circle");

    while (c != NULL) {
        GLfloat radius = stof(c->Attribute("r"));
        GLfloat cx = stoi(c->Attribute("cx")) - arenaX;
        GLfloat cy = 2 * this->getRadius() - stoi(c->Attribute("cy")) - arenaY;

        string colorName = c->Attribute("fill");

        if (!colorName.compare("red")) {
            Enemy* enemy = new Enemy(this, cx, cy, radius);
            enemy->setSpeed(player->getSpeed());

            airEnemies.push_back(enemy);
        }

        if (!colorName.compare("orange")) {
            Circle* circle = new Circle(cx, cy, radius, ORANGE);

            groundEnemies.push_back(circle);
        }

        /* Next */
        c = c->NextSiblingElement("circle");
    }
}

void Arena::draw(GLuint arenaTexture, GLuint groundEnemiesTexture, GLuint projTexture, GLuint bombTexture) {
    glColor3f(1.0, 1.0, 1.0);
    
    // Corpo do Cilindro
    glPushMatrix();
        glTranslatef(.0, .0, -this->getRadius() * .5);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, arenaTexture);
        
        GLUquadricObj* obj = gluNewQuadric();
            gluQuadricDrawStyle(obj, GLU_FILL);
            gluQuadricNormals(obj, GLU_SMOOTH);
            gluQuadricTexture(obj, GLU_TRUE);
            gluQuadricOrientation(obj, GLU_INSIDE);
            gluCylinder(obj, this->getRadius(), this->getRadius(), this->getRadius(), 360, 1);
            gluDeleteQuadric(obj);
            
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glColor3f(0.0, 0.0, 1.0);
  
    // Tampa superior
    glPushMatrix();
        glTranslatef(.0, .0, this->getRadius() * .5);

        this->drawSolidCircle();
    glPopMatrix();
    
    glColor3f(0.0, 0.0, 1.0);

    // Tampa inferior
    glPushMatrix();
        glTranslatef(.0, .0, -this->getRadius() * .5);

        this->drawSolidCircle();
    glPopMatrix();
    
    glColor3f(1.0, 1.0, 1.0);

    // Pista e inimigos terrestres no chao
    glPushMatrix();
        glTranslatef(.0, .0, -this->getRadius() * .5);
        
        airstrip->drawSolidLine();

        for (auto ge : groundEnemies) {
            if (!ge->isDead()) {
                glPushMatrix();
                    glTranslatef(ge->getCx(), ge->getCy(), .0);
                    
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, groundEnemiesTexture);
                    
                    GLUquadricObj* obj = gluNewQuadric();
                        gluQuadricDrawStyle(obj, GLU_FILL);
                        gluQuadricNormals(obj, GLU_SMOOTH);
                        gluQuadricTexture(obj, GLU_TRUE);
                        gluQuadricOrientation(obj, GLU_OUTSIDE);
                        gluSphere(obj, ge->getRadius(), 16, 16);
                        gluDeleteQuadric(obj);
                        
                    glDisable(GL_TEXTURE_2D);
                glPopMatrix();
            }
        }
    glPopMatrix();

    player->drawAirplane(projTexture, bombTexture);
    
    for (auto ae : airEnemies) {
        if (!ae->isDead()) {
            ae->drawAirplane(projTexture);
        }
    }
}
