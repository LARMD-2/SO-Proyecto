#include "shell.h"
#include "parser.h"
#include "executor.h"
#include <iostream>
#include <vector>
using namespace std;

// Constructor
Shell::Shell() {
    ejecutando = true;
    prompt = "mi_shell> ";  // Prompt personalizado
}

// Mostrar el prompt
void Shell::mostrar_prompt() {
    cout << prompt;
}

// Leer comando del usuario
string Shell::leer_comando() {
    string comando;
    getline(cin, comando);
    return comando;
}

// Loop principal de la shell
void Shell::ejecutar() {
    cout << "Bienvenido a Mini-Shell (Persona 1)" << endl;
    while (ejecutando) {
        mostrar_prompt();
        string comando = leer_comando();

        if (comando == "salir") {
            ejecutando = false;
            cout << "¡Hasta luego!" << endl;
        } else if (!comando.empty()) {
            // INTEGRACIÓN: Usar parser 
            vector<string> tokens = Parser::dividir_tokens(comando);

            // EJECUCIÓN BÁSICA (solo para demostrar funcionamiento)
            if (!tokens.empty()) {
                Executor::ejecutar_comando(tokens);
            }
        }
    }
}