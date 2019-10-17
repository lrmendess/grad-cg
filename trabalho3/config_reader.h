#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <iostream>
#include <string>
#include <GL/glut.h>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class ConfigReader {
    private:
        XMLDocument doc;
        XMLElement* root;

    public:
        ConfigReader(string path);

        string getNomeArquivoArena();
        string getTipoArquivoArena();
        string getCaminhoArquivoArena();

        GLfloat getVelocidadeJogador();
        GLfloat getVelocidadeTiro();
};

#endif