#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <string>
using namespace std;

class Shell {
private:
    bool ejecutando;
    string prompt;
    
public:
    Shell();
    void ejecutar();
    void mostrar_prompt();
    string leer_comando();
};

#endif