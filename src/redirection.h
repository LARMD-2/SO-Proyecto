#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <vector>
#include <string>

using namespace std;

class Redirection {
public:
    static bool tiene_redireccion_salida(const vector<string>& tokens, string& archivo);
    static int configurar_redireccion_salida(const string& archivo);
};

#endif