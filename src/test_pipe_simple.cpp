#include <iostream>
#include <vector>
#include <string>
#include "parser.h"
#include "pipe_simple.h"


using namespace std;

void probar_deteccion_pipes() {
    cout << "PROBANDO DETECCIÓN DE PIPES SIMPLES" << endl;
    cout << "======================================" << endl;
    
    vector<string> casos = {
        "ls | grep mini",
        "cat archivo.txt | grep hola",
        "ps aux | grep bash",
        "echo hola | cat",
        "ls -la | head -5",
        "ls -la"  // sin pipe
    };
    
    for (const auto& caso : casos) {
        cout << "Línea: '" << caso << "'" << endl;
        
        vector<string> tokens = Parser::dividir_tokens(caso);
        int pos_pipe = Parser::detectar_pipe(tokens);
        
        cout << "   ¿Tiene pipe?: " << (pos_pipe != -1 ? "Sí" : "No") << endl;
        cout << "   Posición pipe: " << pos_pipe << endl;
        
        if (pos_pipe != -1) {
            vector<vector<string>> comandos = Parser::dividir_en_dos_comandos(tokens);
            
            cout << "   Comando 1: [";
            for (size_t i = 0; i < comandos[0].size(); i++) {
                cout << "'" << comandos[0][i] << "'";
                if (i < comandos[0].size() - 1) cout << ", ";
            }
            cout << "]" << endl;
            
            cout << "   Comando 2: [";
            for (size_t i = 0; i < comandos[1].size(); i++) {
                cout << "'" << comandos[1][i] << "'";
                if (i < comandos[1].size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
        cout << "---" << endl;
    }
}

void probar_ejecucion_pipes_simple() {
    cout << endl << "PROBANDO EJECUCIÓN DE PIPES SIMPLES" << endl;
    cout << "=======================================" << endl;
    
    // Casos de prueba
    vector<pair<vector<string>, vector<string>>> casos = {
        {{"echo", "hola", "mundo"}, {"cat"}},
        {{"ls"}, {"head", "-3"}},
        {{"echo", "prueba"}, {"wc", "-c"}}
    };
    
    for (size_t i = 0; i < casos.size(); i++) {
        cout << "Probando caso " << (i + 1) << ":" << endl;
        cout << "  Comando 1: ";
        for (const auto& token : casos[i].first) cout << token << " ";
        cout << endl;
        cout << "  Comando 2: ";
        for (const auto& token : casos[i].second) cout << token << " ";
        cout << endl;
        
        int resultado = PipeSimple::ejecutar_pipe_simple(casos[i].first, casos[i].second);
        cout << "  Resultado: " << resultado << endl;
        cout << "---" << endl;
    }
}

int main() {
    probar_deteccion_pipes();
    probar_ejecucion_pipes_simple();
    return 0;
}