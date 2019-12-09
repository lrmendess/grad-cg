#include <iostream>
#include <string>
#include <cmath>
#include <GL/glut.h>
#include <ctime>
#include "tinyxml2.h"
#include "config_reader.h"
#include "arena.h"
#include "imageloader.h"

using namespace std;
using namespace tinyxml2;

void switchViewportCamera(int cam);
void cockpitCamera();
void cannonCamera();
void thirdPersonalCamera();
void minimap();

void printText(GLfloat x, GLfloat y, void *font, string text);
void renderBitmapString(GLfloat x, GLfloat y, void *font, string str);
GLuint loadTexture(const char* filename);

void init(void);
void display(void);
void reshape(int width, int height);
void idle(void);

void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseMovement(int x, int y);
void mouseAction(int button, int state, int x, int y);
void mouseClickAndMove(int x, int y);

bool keyboard[256];

ConfigReader* configReader;

Arena* arena;
Player* player;
list<Enemy*> airEnemies;
list<Circle*> groundEnemies;
GLfloat speedMultiplier;
GLfloat fireSpeedMultiplier;
GLfloat enemyFireSpeedMultiplier;
GLfloat enemySpeedMultiplier;
GLfloat enemyFireFreq;
GLfloat oldTimeTakeOff;
GLfloat oldTimeFlying;
GLfloat groundEnemyHeight;

GLfloat cameraEye[3];
GLfloat cameraLook[3];
GLfloat up[3];

GLfloat tpsAngleTheta = 60;
GLfloat tpsAngleFi = 0;

GLfloat cameraMouseX = 0;
GLfloat cameraMouseY = 0;

GLboolean lockTpsCamera = true;
GLboolean collisionEnabled = true;
GLboolean lightingEnabled = true;
GLboolean nightMode = false;

GLuint arenaTexture1;
GLuint arenaTexture1_night;
GLuint arenaTexture2;
GLuint arenaTexture2_night;
GLuint playerTexture;
GLuint airstripTexture;
GLuint airEnemiesTexture;
GLuint groundEnemiesTexture;
GLuint projTexture;
GLuint bombTexture;

int toggleCamera = 1;

int windowWidth = 500;
int windowHeight = 700;

int main(int argc, char** argv) {
    srand(time(NULL));

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
    
    speedMultiplier = configReader.getVelocidadeJogador();
    fireSpeedMultiplier = configReader.getVelocidadeTiro();

    enemySpeedMultiplier = configReader.getInimigoVel();
    enemyFireSpeedMultiplier = configReader.getInimigoVelTiro();
    enemyFireFreq = configReader.getInimigoFreqTiro();

    arena = new Arena(svgPath);

    player = arena->getPlayer();

    /* Copia dos inimigos aereos para interacao */
    for (auto a : arena->getAirEnemies()) {
        airEnemies.push_back(a);
        a->setFireFreq(enemyFireFreq);
    }

    for (auto g : arena->getGroundEnemies()) {
        groundEnemies.push_back(g);
    }

    groundEnemyHeight = .0;
    if (arena->getGroundEnemies().size() > 0) {
        groundEnemyHeight = arena->getGroundEnemies().front()->getRadius() * 2;
    } else {
        groundEnemyHeight = player->getRadius() * 1.5;
    }
    
    /* Glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ace Combat: Poor Edition");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouseMovement);
    glutMouseFunc(mouseAction);
    glutMotionFunc(mouseClickAndMove);

    glutMainLoop();

    return 0;
}

void init(void) {
    /* Seleciona cor de fundo */
    glClearColor(0, 0, 0, 0);
    
    /* Inicializar matriz de texturas */
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    /* Inicializar sistema de viz */
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

//    gluPerspective(90, windowWidth / windowWidth, 1, arena->getRadius() * 5);
    
    /* Carrega todas as texturas */
    arenaTexture1 = loadTexture("textures/sky.bmp");
    arenaTexture1_night = loadTexture("textures/nightclouds.bmp");
    arenaTexture2 = loadTexture("textures/sand.bmp");
    arenaTexture2_night = loadTexture("textures/darksand.bmp");
    playerTexture = loadTexture("textures/greencamo.bmp");
    airstripTexture = loadTexture("textures/road.bmp");
    airEnemiesTexture = loadTexture("textures/redcamo.bmp");
    groundEnemiesTexture = loadTexture("textures/wood.bmp");
    projTexture = loadTexture("textures/metal.bmp");
    bombTexture = loadTexture("textures/hazard.bmp");
    
    /* Inicializa iluminacao */
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    GLfloat lightModel[] = {0.0, 0.0, 0.0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModel);
    
    glEnable(GL_LIGHT0);
    //glColorMaterial(GL_FRONT, GL_DIFFUSE);
    //glDisable(GL_COLOR_MATERIAL);
    
    GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light0_position[] = {arena->getRadius(), arena->getRadius(), arena->getRadius(), 0.0};
    GLfloat light1_position[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light1_direction[] = {0.0, 0.0, 1.0};
    GLfloat light1_angle[] = {25.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
    
    GLfloat materialSpecular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat materialShininess[] = {100.0};
 
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

    cockpitCamera();
}

void display(void) {
    /* Limpar todos os pixels */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*************************** Janela Principal *****************************/
    glViewport(0, 0, windowWidth, windowWidth);
    switchViewportCamera(0);
    
    /* Habilitar/Desabilitar iluminacao */
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Camera do Cockpit */
    if (toggleCamera == 1) {
        cockpitCamera();

    /* Camera do Canhao */
    } else if (toggleCamera == 2) {
        cannonCamera();
    
    /* Camera em Terceira Pessoa */    
    } else if (toggleCamera == 3) {
        thirdPersonalCamera();
    }
    
    /* Modo noturno */
    if (nightMode) {
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    } else {
        glEnable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    
    gluLookAt(
        cameraEye[0], cameraEye[1], cameraEye[2],
        cameraLook[0], cameraLook[1], cameraLook[2],
        up[0], up[1], up[2]
    );

    arena->draw(arenaTexture1, arenaTexture1_night, arenaTexture2, arenaTexture2_night, playerTexture, airstripTexture, airEnemiesTexture, groundEnemiesTexture, projTexture, bombTexture, nightMode);

    // Textos na tela e minimapa
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
        glPushMatrix();
            glLoadIdentity();
            
            if (player->getPoints() == arena->getGroundEnemies().size()) {
                string result("WIN");
                printText(-20.0, 0.0, GLUT_BITMAP_HELVETICA_18, result);
            } else if (player->isDead()) {
                string result("LOSE");
                printText(-25.0, 0.0, GLUT_BITMAP_HELVETICA_18, result);
            }
            
            string totalGroundEnemies(to_string(arena->getGroundEnemies().size() - player->getPoints()));
            string score("Destroyed: " 
                    + to_string(player->getPoints()) 
                    + " | Remaining: " 
                    + totalGroundEnemies);

            printText(85.0, 230.0, GLUT_BITMAP_HELVETICA_12, score);
            
            if (not lightingEnabled) {
                string lighting("LIGHTING DISABLED");
                printText(-240.0, 230.0, GLUT_BITMAP_HELVETICA_12, lighting);
            }
            
            if (not collisionEnabled) {
                string collision("COLLISION DISABLED");
                printText(-240.0, 210.0, GLUT_BITMAP_HELVETICA_12, collision);
            }
            
            minimap();
        glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    /**************************** Janela da bomba *****************************/
    glViewport(0, windowWidth, windowWidth, (windowHeight - windowWidth));
    switchViewportCamera(1);
    
    /* Habilitar/Desabilitar iluminacao */
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if(player->getBombs().size() > 0) {
        if(!nightMode) {
            Bomb* droppedBomb = player->getBombs().front();
            gluLookAt(
                droppedBomb->getCx(), droppedBomb->getCy(), droppedBomb->getCz(),
                droppedBomb->getCx(), droppedBomb->getCy(), 0.0,
                0.0, 1.0, 0.0
            );
            
            arena->draw(arenaTexture1, arenaTexture1_night, arenaTexture2, arenaTexture2_night, playerTexture, airstripTexture, airEnemiesTexture, groundEnemiesTexture, projTexture, bombTexture, nightMode);
        }
    }

    /* Nao esperar! */
    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(90, windowWidth / windowWidth, 1, arena->getRadius() * 5);
    
    glMatrixMode(GL_MODELVIEW);
}

void keyPress(unsigned char key, int x, int y) {
    keyboard[key] = true;
    
    /* Modo noturno */
    if (key == 'n') {
        nightMode = not nightMode;
    }
    
    /* Desabilitar colisoes */
    if (keyboard['k']) {
        collisionEnabled = not collisionEnabled;
    }
    
    /* Desabilitar iluminacao */
    if (keyboard['l']) {
        lightingEnabled = not lightingEnabled;
    }
}

void keyUp(unsigned char key, int x, int y) {
    keyboard[key] = false;
}

void idle(void) {
    /* Mudanca de camera */
    if (keyboard['1']) {
        toggleCamera = 1;
        lockTpsCamera = true;

    } else if (keyboard['2']) {
        toggleCamera = 2;
        lockTpsCamera = true;

    } else if (keyboard['3']) {
        tpsAngleTheta = 60;
        tpsAngleFi = 0;
        toggleCamera = 3;
    }

    // R
    if (keyboard['r']) {
        player->reset();

        for (auto a : airEnemies) {
            a->reset();
        }

        for (auto g : groundEnemies) {
            g->reset();
        }

        for (int i = 0; i < 256; i++) {
            keyboard[i] = false;
        }
    }

    if (player->getPoints() == arena->getGroundEnemies().size()) {
        return;
    }

    if (player->isDead()) {
        return;
    }

    GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    GLfloat diffTime = currentTime - oldTimeFlying;
    oldTimeFlying = currentTime;

    // MOVIMENTACAO DOS INIMIGOS
    for (auto a : airEnemies) {
        if (!a->isDead()) {
            if ((currentTime - a->getLastMovementTime()) >= 1.0) {
                a->setMovementType(rand() % 5);
                a->setLastMovementTime(currentTime);
            }
            
            GLfloat groundEnemyHeight = .0;
            if (arena->getGroundEnemies().size() > 0) {
                groundEnemyHeight = arena->getGroundEnemies().front()->getRadius() * 2;
            }

            if (a->getCz() >= arena->getRadius() * .9 - 10) {
                a->setMovementType(4);
                a->setLastMovementTime(currentTime);
            }
            
            if (a->getCz() <= groundEnemyHeight + 10) {
                a->setMovementType(3);
                a->setLastMovementTime(currentTime);
            }
            
            // Voa reto
            if (a->getMovementType() == 0) {
            // Vira para esquerda
            } else if (a->getMovementType() == 1) {
                a->moveX(120, diffTime);
            // Vira para direita
            } else if (a->getMovementType() == 2) {
                a->moveX(-120, diffTime);
            // Voa para cima
            } else if (a->getMovementType() == 3) {
                a->moveZ(10, diffTime);
            // Voa para baixo
            } else if (a->getMovementType() == 4) {
                a->moveZ(-10, diffTime);
            }

            a->move(enemySpeedMultiplier, diffTime);
            // atira só depois de x tempos
            a->fire(enemyFireSpeedMultiplier, enemySpeedMultiplier, diffTime);
            a->updateProjectiles(diffTime);
        }
    }

    if (player->isFlying()) {
        // A
        if (keyboard['a']) {
            player->moveX(120, diffTime);
        }

        // D
        if (keyboard['d']) {
            player->moveX(-120, diffTime);
        }

        if (!keyboard['s'] && !keyboard['w']) {
            if (player->getAngleTheta() > 5) {
                player->moveZ(-90, diffTime);
            } else if (player->getAngleTheta() < -5) {
                player->moveZ(90, diffTime);
            } else {
                player->setAngleTheta(.0);
            }
        } else {
            if (player->canTilt == true) {
                if (keyboard['w']) {
                    if ((player->getCz() + player->getRadius()) <= arena->getRadius()) {
                        player->moveZ(90, diffTime);
                    }
                }

                if (keyboard['s']) {                    
                    if ((player->getCz() - player->getRadius()) >= groundEnemyHeight) {
                        player->moveZ(-90, diffTime);
                    }
                }
            } else {
                if (player->getAngleTheta() > 5) {
                    player->moveZ(-120, diffTime);
                } else if (player->getAngleTheta() < -5) {
                    player->moveZ(120, diffTime);
                } else {
                    player->setAngleTheta(.0);
                    player->canTilt = true;
                }
            }
        }

        if (keyboard['='] || keyboard['+']) {
            player->setSpeed(player->getSpeed() + 200.0 * diffTime);
        }

        if (keyboard['-'] || keyboard['_']) {
            GLfloat speed = player->getSpeed() - 200.0 * diffTime;

            if (speed <= 0) {
                speed = 0;
            }

            player->setSpeed(speed);
        }

        player->updateProjectiles(diffTime);
        player->updateBombs(currentTime, diffTime);
        player->move(speedMultiplier, diffTime);

    } else {
        if (keyboard['u'] && !player->isTakeOff()) {
            player->setTakeOff(true);
            oldTimeTakeOff = glutGet(GLUT_ELAPSED_TIME);
        }

        if (player->isTakeOff()) {
            GLint currentTime = glutGet(GLUT_ELAPSED_TIME) - oldTimeTakeOff;

            player->takeOffAirplane(currentTime, diffTime);

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
    if (!player->isTakeOff() && player->isFlying() && !player->isDead()) {
        GLint distanceX = (player->getMouseX() - x) / 2;
        GLint distanceY = (player->getMouseY() - y) / 2;

        GLint newX = player->getCannonAngle() + distanceX;

        if (newX < -30) {
            player->setCannonAngle(-30);
        } else if (newX > 30) {
            player->setCannonAngle(30);
        } else {
            player->setCannonAngle(newX);
        }

        GLint newY = player->getCannonAngleTheta() + distanceY;

        if (newY < -30) {
            player->setCannonAngleTheta(-30);
        } else if (newY > 30) {
            player->setCannonAngleTheta(30);
        } else {
            player->setCannonAngleTheta(newY);
        }
    }

    player->setMouseX(x);
    player->setMouseY(y);

    cameraMouseX = x;
    cameraMouseY = y;
}

void mouseAction(int button, int state, int x, int y) {
    if (player->isFlying() && !player->isTakeOff() && !player->isDead()) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            player->fire(fireSpeedMultiplier, speedMultiplier);
        }

        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !keyboard[' ']
            && player->getAngleTheta() >= -2 && player->getAngleTheta() <= 2) {
                
            player->bomb(speedMultiplier);
        }
    }

    if (button == GLUT_RIGHT_BUTTON && toggleCamera == 3){
        if (state == GLUT_DOWN && keyboard[' '])
            lockTpsCamera = false;
        
        if(state == GLUT_UP)
            lockTpsCamera = true;
    }

    cameraMouseX = x;
    cameraMouseY = y;
}

void mouseClickAndMove(int x, int y) {
    if(lockTpsCamera == false) {
        GLint diffY = (y - cameraMouseY) / 2;
        GLint newTheta = tpsAngleTheta + diffY;
        
        if (newTheta <= 60 && newTheta >= -60) {
            tpsAngleTheta = newTheta;
        }

        tpsAngleFi -= (x - cameraMouseX) / 2;

        cameraMouseX = x;
        cameraMouseY = y;
    }
}

void switchViewportCamera(int cam) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (cam == 0) {
        gluPerspective(90, windowWidth / windowWidth, 1, arena->getRadius() * 5);
    } else if (cam == 1) {
        gluPerspective(60, windowWidth / (windowHeight - windowWidth), arena->getPlayer()->getRadius() / 4, arena->getRadius() * 5);
    }
}

void cockpitCamera() {
    GLfloat fiRad = player->getAngle() * M_PI / 180;
    GLfloat thetaRad = player->getAngleTheta() * M_PI / 180;

    cameraEye[0] = player->getCx() + player->getRadius() * cos(thetaRad + M_PI / 4) * cos(fiRad);
    cameraEye[1] = player->getCy() + player->getRadius() * cos(thetaRad + M_PI / 4) * sin(fiRad);
    cameraEye[2] = player->getCz() + player->getRadius() * sin(thetaRad + M_PI / 4);

    cameraLook[0] = player->getCx() + 2 * player->getRadius() * cos(thetaRad) * cos(fiRad);
    cameraLook[1] = player->getCy() + 2 * player->getRadius() * cos(thetaRad) * sin(fiRad);
    cameraLook[2] = player->getCz() + 2 * player->getRadius() * sin(thetaRad);

    up[0] = 0;
    up[1] = 0;
    up[2] = 1;
}

void cannonCamera() {
    GLfloat airplaneFiRad = player->getAngle() * M_PI / 180;
    GLfloat airplaneThetaRad = player->getAngleTheta() * M_PI / 180;

    GLfloat cannonFiRad = player->getCannonAngle() * M_PI / 180;
    GLfloat cannonThetaRad = player->getCannonAngleTheta() * M_PI / 180;

    GLfloat px = player->getCx();
    px += (player->getRadius()) * cos(airplaneThetaRad) * cos(airplaneFiRad);
    // cameraEye[0] = px + player->getRadius() / 2 * cos(cannonThetaRad + airplaneThetaRad) * cos(cannonFiRad + airplaneFiRad);
    px += (player->getRadius() / 2 + 1000) * cos(cannonThetaRad + airplaneThetaRad) * cos(cannonFiRad + airplaneFiRad);
    cameraLook[0] = px;
    
    GLfloat py = player->getCy();
    py += (player->getRadius()) * cos(airplaneThetaRad) * sin(airplaneFiRad);
    // cameraEye[1] = py + player->getRadius() / 2 * cos(cannonThetaRad + airplaneThetaRad) * sin(cannonFiRad + airplaneFiRad);
    py += (player->getRadius() / 2 + 1000) * cos(cannonThetaRad + airplaneThetaRad) * sin(cannonFiRad + airplaneFiRad);
    cameraLook[1] = py;

    GLfloat pz = player->getCz();
    pz += (player->getRadius()) * sin(airplaneThetaRad);
    // cameraEye[2] = pz + player->getRadius() / 2 * sin(cannonThetaRad + airplaneThetaRad);
    pz += (player->getRadius() / 2 + 1000) * sin(cannonThetaRad + airplaneThetaRad);
    cameraLook[2] = pz;

    up[0] = 0;
    up[1] = 0;
    up[2] = 1;

    GLfloat fiRad = player->getAngle() * M_PI / 180;
    GLfloat thetaRad = player->getAngleTheta() * M_PI / 180;

    cameraEye[0] = player->getCx() + player->getRadius() * cos(thetaRad + M_PI / 24) * cos(fiRad);
    cameraEye[1] = player->getCy() + player->getRadius() * cos(thetaRad + M_PI / 24) * sin(fiRad);
    cameraEye[2] = player->getCz() + player->getRadius() * sin(thetaRad + M_PI / 24);

    //cameraLook[0] = player->getCx() + 2 * player->getRadius() * cos(thetaRad) * cos(fiRad);
    //cameraLook[1] = player->getCy() + 2 * player->getRadius() * cos(thetaRad) * sin(fiRad);
    //cameraLook[2] = player->getCz() + 2 * player->getRadius() * sin(thetaRad);

    // up[0] = 0;
    // up[1] = 0;
    // up[2] = 1;
}

void thirdPersonalCamera() {
    // GLfloat fiRad = player->getAngle() * M_PI / 180;
    // GLfloat thetaRad = player->getAngleTheta() * M_PI / 180;
// 
    // cameraEye[0] = player->getCx() + player->getRadius() * cos(thetaRad + M_PI / 4) * cos(fiRad);
    // cameraEye[1] = player->getCy() + player->getRadius() * cos(thetaRad + M_PI / 4) * sin(fiRad);
    // cameraEye[2] = player->getCz() + player->getRadius() * sin(thetaRad + M_PI / 4);
// 
    // cameraLook[0] = player->getCx();
    // cameraLook[1] = player->getCy();
    // cameraLook[2] = player->getCz();
// 
    // up[0] = 0;
    // up[1] = 0;
    // up[2] = 1;

    GLfloat distance = player->getRadius() * 5;
    GLfloat playerFi = player->getAngle() * M_PI/180;

    GLfloat fi = tpsAngleFi * M_PI / 180 + playerFi + M_PI;
    GLfloat theta = tpsAngleTheta * M_PI / 180;

    GLfloat cameraStepX = distance * cos((theta)) * cos(fi);
    GLfloat cameraStepY = distance * cos((theta)) * sin(fi);
    GLfloat cameraStepZ = distance * sin((theta));

    cameraEye[0] = player->getCx() + cameraStepX;
    cameraEye[1] = player->getCy() + cameraStepY;
    cameraEye[2] = player->getCz() + cameraStepZ;
    
    cameraLook[0] = player->getCx();
    cameraLook[1] = player->getCy();
    cameraLook[2] = player->getCz();
    
    up[0] = 0;
    up[1] = 0;
    up[2] = 1;
}

void minimap() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(-250, 250, -250, 250, -1, 1);
        glScalef(0.25, 0.25, 0.0);
        glTranslatef(arena->getRadius() * 2, -arena->getRadius() * 2, 0.0);
        
        // Arena
        GLfloat minimapArenaColor[] = {1.0, 1.0, 1.0};
        Circle* minimapArena = new Circle(arena->getCx(), arena->getCy(), arena->getRadius(), minimapArenaColor);
        minimapArena->drawSolidRing();
        delete minimapArena;
        
        // Inimigos terrestres
        for (auto ge : arena->getGroundEnemies()) {
            if (!ge->isDead()) {
                GLfloat minimapGroundEnemyColor[] = {1.0, 0.5, 0.0};
                Circle* minimapGroundEnemy = new Circle(ge->getCx(), ge->getCy(), ge->getRadius(), minimapGroundEnemyColor);
                minimapGroundEnemy->drawSolidCircle();
                delete minimapGroundEnemy;
            }
        }
        
        // Inimigos aereos
        for (auto ae : arena->getAirEnemies()) {
            if (!ae->isDead()) {
                GLfloat minimapAirEnemyColor[] = {1.0, 0.0, 0.0};
                Circle* minimapAirEnemy = new Circle(ae->getCx(), ae->getCy(), ae->getRadius(), minimapAirEnemyColor);
                minimapAirEnemy->drawSolidCircle();
                delete minimapAirEnemy;
            }
        }
        
        // Jogador
        GLfloat minimapPlayerColor[] = {0.0, 1.0, 0.0};
        Circle* minimapPlayer = new Circle(arena->getPlayer()->getCx(), arena->getPlayer()->getCy(), arena->getPlayer()->getRadius(), minimapPlayerColor);
        minimapPlayer->drawSolidCircle();
        delete minimapPlayer;
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void printText(GLfloat x, GLfloat y, void *font, string text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(-250, 250, -250, 250, -1, 1);
        glColor3f(1.0, 1.0, 1.0);
        renderBitmapString(x, y, font, text);    
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(GLfloat x, GLfloat y, void *font, string str) {
    glRasterPos2f(x,y);

    for (string::iterator c = (&str)->begin(); c != (&str)->end(); ++c)  {
        glutBitmapCharacter(font, *c);
    }
}

GLuint loadTexture(const char* filename) {
    GLuint texture;
    
    Image* image = loadBMP(filename);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,                // Always GL_TEXTURE_2D
                 0,                            // 0 for now
                 GL_RGB,                       // Format OpenGL uses for image
                 image->width, image->height,  // Width and height
                 0,                            // The border of the image
                 GL_RGB,                       // GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE,             // GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
                 image->pixels);               // The actual pixel data
                 
    delete image;

    return texture;
}
