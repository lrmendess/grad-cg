#include <GL/glut.h>
#include <iostream>
#include <string>
#include "circle.h"

int main(int argc, char* argv[]) {
    Circle circle(5, 0, 0);
    circle.setX(8);

    std::cout << circle.getX() << std::endl;

    return 0;
}
