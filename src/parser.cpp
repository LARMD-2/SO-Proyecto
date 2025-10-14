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

void Parser::detectar_redirecciones(vector<string>& tokens, string& input_file, string& output_file, bool& append_output) {
    
    // Reiniciar valores
    input_file.clear();
    output_file.clear();
    append_output = false;
    
    for (auto it = tokens.begin(); it != tokens.end(); ) {
        if (*it == ">") {
            // Redirección simple
            it = tokens.erase(it);
            if (it != tokens.end()) {
                output_file = *it;
                it = tokens.erase(it);
            }
        }
        else if (*it == ">>") {
            // Redirección append
            it = tokens.erase(it);
            if (it != tokens.end()) {
                output_file = *it;
                append_output = true;
                it = tokens.erase(it);
            }
        }
        else if (*it == "<") {
            // Redirección entrada
            it = tokens.erase(it);
            if (it != tokens.end()) {
                input_file = *it;
                it = tokens.erase(it);
            }
        }
        else {
            ++it;
        }
    }
}

int Parser::detectar_pipe(const vector<string>& tokens) {
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "|") {
            return i;
        }
    }
    return -1;
}

vector<vector<string>> Parser::dividir_en_dos_comandos(const vector<string>& tokens) {
    vector<vector<string>> comandos(2);
    int pos_pipe = detectar_pipe(tokens);
    
    if (pos_pipe == -1) {
        // No hay pipe, solo un comando
        comandos[0] = tokens;
        return comandos;
    }
    
    // Primer comando (antes del pipe)
    for (int i = 0; i < pos_pipe; i++) {
        comandos[0].push_back(tokens[i]);
    }
    
    // Segundo comando (después del pipe)
    for (size_t i = pos_pipe + 1; i < tokens.size(); i++) {
        comandos[1].push_back(tokens[i]);
    }
    
    return comandos;
}

bool Parser::detectar_background(vector<string>& tokens) {
    if (!tokens.empty() && tokens.back() == "&") {
        tokens.pop_back();  // Remover el "&" del final
        return true;
    }
    return false;
}