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

class Arena : public Circle {
    private:
        Circle* player;
        Line* airstrip;
        list<Circle*> groundEnemies;
        list<Circle*> airEnemies;

    public:
        ~Arena();
        Arena(const string& path);

        Circle* getPlayer();
        void draw();
};

#endif