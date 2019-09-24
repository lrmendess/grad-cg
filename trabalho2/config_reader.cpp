#include "config_reader.h"

ConfigReader::ConfigReader(const string& path) {
    if (doc.LoadFile(path.c_str()) != XML_SUCCESS) {
        cout << "Erro ao tentar abrir o arquivo '" << path << "'" << endl;
        exit(EXIT_FAILURE);
    }

    root = doc.RootElement();
}

string ConfigReader::getNomeArquivoArena() {
    string nomeArquivoArena = root
        ->FirstChildElement("arquivoDaArena")
        ->FirstChildElement("nome")
        ->GetText();

    return nomeArquivoArena;
}

string ConfigReader::getTipoArquivoArena() {
    string tipoArquivoArena = root
        ->FirstChildElement("arquivoDaArena")
        ->FirstChildElement("tipo")
        ->GetText();

    return tipoArquivoArena;
}

string ConfigReader::getCaminhoArquivoArena() {
    string caminhoArquivoArena = root
        ->FirstChildElement("arquivoDaArena")
        ->FirstChildElement("caminho")
        ->GetText();

    return caminhoArquivoArena;
}

GLfloat ConfigReader::getVelocidadeJogador() {
    string caminhoArquivoArena = root
        ->FirstChildElement("jogador")
        ->Attribute("vel");

    return stof(caminhoArquivoArena);
}
