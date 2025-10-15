#include "shell.h"
#include "parser.h"
#include "executor.h"
#include <iostream>
#include <vector>
#include "line_reader.h"
#include "builtins.h"
using namespace std;

// Constructor
Shell::Shell() {
    ejecutando = true;
    prompt = "mi_shell> ";  // Prompt personalizado
    reader = new StdinReader();
}

// Mostrar el prompt
void Shell::mostrar_prompt() { /* ya lo maneja reader en StdinReader */ }

// Leer comando del usuario
string Shell::leer_comando() {
    return reader->read(prompt);
}

// Loop principal de la shell
void Shell::ejecutar() {
    cout << "Bienvenido a Mini-Shell" << endl;
    while (ejecutando) {
    // StdinReader gestiona el prompt; ReadlineReader también
    string comando = leer_comando();

        if (comando == "salir") {
            ejecutando = false;
            cout << "¡Hasta luego!" << endl;
        } else if (!comando.empty()) {
            // INTEGRACIÓN: Usar parser 
            Builtins::registrar_historial(comando);
            vector<string> tokens = Parser::dividir_tokens(comando);
            Builtins::expandir_alias(tokens);

            if (!tokens.empty()) {
                Executor::ejecutar_comando(tokens);
            }
        }
    }
}
