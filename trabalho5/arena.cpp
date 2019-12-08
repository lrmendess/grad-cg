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

void Arena::draw(GLuint arenaTexture1, GLuint arenaTexture1_night, GLuint arenaTexture2, GLuint arenaTexture2_night, GLuint playerTexture, GLuint airstripTexture, GLuint airEnemiesTexture, GLuint groundEnemiesTexture, GLuint projTexture, GLuint bombTexture, bool nightMode) {    
    // Corpo do Cilindro
    GLuint skyTexture;
    
    if (nightMode) {
        GLfloat skyEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, skyEmission);
        skyTexture = arenaTexture1_night;
    } else {
        GLfloat skyEmission[] = {1.0, 1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, skyEmission);
        skyTexture = arenaTexture1;
    }
    
    GLfloat skyColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skyColor);
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
            glScalef(8.0, 2.0, 1.0);
            
            glBindTexture(GL_TEXTURE_2D, skyTexture);   
            GLUquadricObj* sky  = gluNewQuadric();
                gluQuadricDrawStyle(sky, GLU_FILL);
                gluQuadricNormals(sky, GLU_SMOOTH);
                gluQuadricTexture(sky, GLU_TRUE);
                gluQuadricOrientation(sky, GLU_INSIDE);
                gluCylinder(sky, this->getRadius(), this->getRadius(), this->getRadius(), 180, 180);
            gluDeleteQuadric(sky);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glDisable(GL_TEXTURE_2D);
  
    // Tampa superior
    if (nightMode) {
        GLfloat roofEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, roofEmission);
    } else {
        GLfloat roofEmission[] = {1.0, 1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, roofEmission);
    }
    
    GLfloat roofColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, roofColor);
    glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();
        glTranslatef(.0, .0, this->getRadius());
        
        glEnable(GL_TEXTURE_2D);
            glMatrixMode(GL_TEXTURE);
            glPushMatrix();
                glScalef(4.0, 4.0, 1.0);
                
                glBindTexture(GL_TEXTURE_2D, skyTexture);
                GLUquadricObj* roof = gluNewQuadric();
                    gluQuadricDrawStyle(roof, GLU_FILL);
                    gluQuadricNormals(roof, GLU_SMOOTH);
                    gluQuadricTexture(roof, GLU_TRUE);
                    gluQuadricOrientation(roof, GLU_OUTSIDE);
                    gluDisk(roof, 0, this->getRadius(), 180, 180);
                gluDeleteQuadric(roof);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // Tampa inferior
    GLuint groundTexture;
    
    if (nightMode) {
        GLfloat groundEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, groundEmission);
        groundTexture = arenaTexture2_night;
    } else {
        GLfloat groundEmission[] = {1.0, 1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, groundEmission);
        groundTexture = arenaTexture2;
    }
    
    GLfloat groundColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundColor);
    glColor3f(1.0, 1.0, 1.0);
    
    glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
            glScalef(10.0, 10.0, 1.0);
            
            glBindTexture(GL_TEXTURE_2D, groundTexture);
            GLUquadricObj* ground = gluNewQuadric();
                gluQuadricDrawStyle(ground, GLU_FILL);
                gluQuadricNormals(ground, GLU_SMOOTH);
                gluQuadricTexture(ground, GLU_TRUE);
                gluQuadricOrientation(ground, GLU_OUTSIDE);
                gluDisk(ground, 0, this->getRadius(), 180, 180);
            gluDeleteQuadric(ground);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glDisable(GL_TEXTURE_2D);

    // Pista
    if (nightMode) {
        GLfloat airstripEmission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, airstripEmission);
    } else {
        GLfloat airstripEmission[] = {1.0, 1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, airstripEmission);
    }
    
    GLfloat airstripEmission[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat airstripColor[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, airstripEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, airstripColor);
    glColor3f(1.0, 1.0, 1.0);
    
    airstrip->drawSolidLine(airstripTexture);

    // Inimigos Terrestres
    for (auto ge : groundEnemies) {
        if (!ge->isDead()) {
            GLfloat geEmission[] = {0.0, 0.0, 0.0, 1.0};
            GLfloat geColor[] = {1.0, 1.0, 1.0, 1.0};
            glMaterialfv(GL_FRONT, GL_EMISSION, geEmission);
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, geColor);
            glColor3f(1.0, 1.0, 1.0);
            
            glPushMatrix();
                glTranslatef(ge->getCx(), ge->getCy(), .0);
                
                // Esfera da base
                glEnable(GL_TEXTURE_2D);
                    glMatrixMode(GL_TEXTURE);
                    glPushMatrix();
                        glScalef(1.0, 2.0, 1.0);

                        glBindTexture(GL_TEXTURE_2D, groundEnemiesTexture);
                        GLUquadricObj* base = gluNewQuadric();
                            gluQuadricDrawStyle(base, GLU_FILL);
                            gluQuadricNormals(base, GLU_SMOOTH);
                            gluQuadricTexture(base, GLU_TRUE);
                            gluQuadricOrientation(base, GLU_OUTSIDE);
                            gluSphere(base, ge->getRadius(), 16, 16);
                        gluDeleteQuadric(base);
                    glPopMatrix();
                    glMatrixMode(GL_MODELVIEW);
                glDisable(GL_TEXTURE_2D);
                
                // Haste da bandeira
                GLfloat stemEmission[] = {0.0, 0.0, 0.0, 1.0};
                GLfloat stemColor[] = {0.0, 0.0, 0.0, 1.0};
                glMaterialfv(GL_FRONT, GL_EMISSION, stemEmission);
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, stemColor);
                glColor3f(0.0, 0.0, 0.0);
                
                glPushMatrix();
                    glTranslatef(0.0, 0.0, ge->getRadius());
                    
                    GLUquadricObj* stem = gluNewQuadric();
                        gluQuadricDrawStyle(stem, GLU_FILL);
                        gluQuadricNormals(stem, GLU_SMOOTH);
                        gluQuadricTexture(stem, GLU_FALSE);
                        gluQuadricOrientation(stem, GLU_OUTSIDE);
                        gluCylinder(stem, ge->getRadius() / 20, ge->getRadius() / 20, ge->getRadius() / 2, 16, 16);
                    gluDeleteQuadric(stem);
                    
                    // Bandeira
                    GLfloat flagEmission[] = {0.0, 0.0, 0.0, 1.0};
                    GLfloat flagColor[] = {1.0, 0.0, 0.0, 1.0};
                    glMaterialfv(GL_FRONT, GL_EMISSION, flagEmission);
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, flagColor);
                    glColor3f(1.0, 0.0, 0.0);
                    
                    glPushMatrix();
                        glTranslatef(0.0, ge->getRadius() / 8, ge->getRadius() / 2);
                        glScalef(ge->getRadius() / 16, ge->getRadius() / 4, ge->getRadius() / 10);
                        
                        glutSolidCube(ge->getRadius() / 12);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        }
    }

    player->drawAirplane(playerTexture, projTexture, bombTexture);
    
    for (auto ae : airEnemies) {
        if (!ae->isDead()) {
            ae->drawAirplane(airEnemiesTexture, projTexture);
        }
    }
}
