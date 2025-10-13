#ifndef PIPE_SIMPLE_H
#define PIPE_SIMPLE_H

#include <vector>
#include <string>
using namespace std;

class PipeSimple {
public:
    //Ejecutar dos comandos conectados con pipe: cmd1 | cmd2
    //Retorna: 0 si éxito, -1 si error
    
    static int ejecutar_pipe_simple(
        const vector<string>& comando1,
        const vector<string>& comando2);
};

#endif