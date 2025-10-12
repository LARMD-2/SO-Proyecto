#include <iostream>
#include <vector>
#include "executor.h"
using namespace std;

int main() {
    cout << "=== Probando Executor (Persona 1) ===" << endl;

    // Probar comando externo
    vector<string> tokens = {"ls", "-la"};
    cout << "Probando: ls -la" << endl;
    Executor::ejecutar_comando(tokens);
    
    // Probar comando interno
    vector<string> tokens2 = {"pwd"};
    cout << "Probando: pwd" << endl;
    Executor::ejecutar_comando(tokens2);
    
    return 0;
}