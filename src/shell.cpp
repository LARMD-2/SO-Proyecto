#include "shell.h"
#include "parser.h"
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
                cout << "Ejecutando: " << tokens[0];
                if (tokens.size() > 1) {
                    cout << " con argumentos: ";
                    for (size_t i = 1; i < tokens.size(); i++) {
                        cout << tokens[i] << " ";
                    }
                }
                cout << endl;
                
                // Comandos internos básicos
                if (tokens[0] == "pwd") {
                    // Simular pwd por ahora
                    cout << "/home/usuario/proyectos" << endl;
                }
                else if (tokens[0] == "help") {
                    cout << "Comandos disponibles: pwd, help, salir" << endl;
                }
            }
        }
    }
}