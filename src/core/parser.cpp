#include "parser.h"
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

vector<string> Parser::dividir_tokens(const string& linea) {
    vector<string> tokens;
    string token;
    istringstream stream_linea(linea);
    
    // Caso especial: línea vacía
    if (linea.empty()) {
        return tokens;
    }
    
    // Dividir por espacios
    while (stream_linea >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool Parser::es_comando_interno(const vector<string>& tokens) {
    if (tokens.empty()) {
        return false;
    }
    
    string comando = tokens[0];
    return (comando == "cd" || comando == "pwd" || comando == "help" || comando == "salir");
}

string Parser::trim(const string& str) {
    size_t inicio = str.find_first_not_of(" \t\n\r");
    size_t fin = str.find_last_not_of(" \t\n\r");
    
    if (inicio == string::npos) {
        return ""; // String vacío o solo espacios
    }
    
    return str.substr(inicio, fin - inicio + 1);
}