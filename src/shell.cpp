#include "shell.h"
#include <iostream>
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

        // Por ahora solo manejar "salir"
        if (comando == "salir") {
            ejecutando = false;
            cout << "Saliendo de la shell..." << endl;
        } else if (!comando.empty()) {
            cout << "Comando recibido: " << comando << endl;
            // TODO: Aquí luego se integrará con el parser y executor
        }
    }
}