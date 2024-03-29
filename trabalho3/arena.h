#ifndef SVGREADER_H
#define SVGREADER_H

#include <iostream>
#include <string>
#include <list>
#include <GL/glut.h>
#include "tinyxml2.h"
#include "circle.h"
#include "line.h"
#include "player.h"

#define RED     (const GLfloat[3]) { 1, 0, 0 }
#define GREEN   (const GLfloat[3]) { 0, 1, 0 }
#define BLUE    (const GLfloat[3]) { 0, 0, 1 }
#define ORANGE  (const GLfloat[3]) { 1,.5, 0 }

using namespace std;
using namespace tinyxml2;

class Player;

class Arena : public Circle {
    private:
        Player* player  = nullptr;
        Line* airstrip  = nullptr;
        list<Circle*> groundEnemies;
        list<Circle*> airEnemies;

    public:
        ~Arena();
        Arena(string path);

        Player* getPlayer() { return player; }
        Line* getAirstrip() { return airstrip; }
        
        list<Circle*>& getGroundEnemies() { return groundEnemies; }
        list<Circle*>& getAirEnemies() { return airEnemies; }

        void draw();
};

#endif