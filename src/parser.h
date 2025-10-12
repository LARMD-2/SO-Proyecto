#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Parser {
public:
    //Divide una línea de comando en tokens individuales
    static vector<string> dividir_tokens(const string& linea);
    
    //Verifica si el comando es interno (cd, pwd, help, salir)
    static bool es_comando_interno(const vector<string>& tokens);
    
    //Limpia espacios en blanco al inicio y final
    static string trim(const string& str);
};

#endif