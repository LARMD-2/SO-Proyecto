#include <iostream>
#include <vector>
#include <string>
#include "core/parser.h"

using namespace std;

void probar_parser() {
    cout << "🔧 PROBANDO PARSER - PERSONA 2" << endl;
    cout << "===============================" << endl;
    
    // Casos de prueba
    vector<string> casos_prueba = {
        "ls -la /home",
        "echo hola mundo",
        "  cd  /usr/local/bin  ",
        "pwd",
        "salir",
        "",  // línea vacía
        "   "  // solo espacios
    };
    
    for (const auto& caso : casos_prueba) {
        cout << "Línea: '" << caso << "'" << endl;
        
        vector<string> tokens = Parser::dividir_tokens(caso);
        
        cout << "Tokens: [";

        for (size_t i = 0; i < tokens.size(); ++i) {
            cout << "'" << tokens[i] << "'";
            if (i < tokens.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        
        cout << "Es interno: " << (Parser::es_comando_interno(tokens) ? "Sí" : "No") << endl;
        cout << "---" << endl;
    }
}

int main() {
    probar_parser();
    return 0;
}