#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <string>
using namespace std;

class LineReader; 

class Shell {
private:
    bool ejecutando;
    string prompt;
    LineReader* reader;
    
public:
    Shell();
    void ejecutar();
    void mostrar_prompt();
    string leer_comando();
};

#endif