#include "executor.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include <builtins.h>
#include "redirection.h"
using namespace std;

// Función principal que decide si es interno o externo
int Executor::ejecutar_comando(const vector<string>& tokens) {
    if (tokens.empty()) {
        return 0;  
    }

    string archivo_redireccion;
    bool redirigir = Redirection::tiene_redireccion_salida(tokens, archivo_redireccion);
    
    // Filtrar tokens (quitar > y archivo)
    vector<string> tokens_filtrados;
    for (const auto& token : tokens) {
        if (token != ">" && token.substr(0, 1) != ">" && token != archivo_redireccion) {
            tokens_filtrados.push_back(token);
        }
    }

    if (tokens_filtrados.empty()) return 0;
    
    if (es_comando_interno(tokens[0])) {
        return ejecutar_interno(tokens);
    } else {
        return ejecutar_externo(tokens);
    }
}

// Verificar comandos internos
bool Executor::es_comando_interno(const string& comando) {
    return Builtins::es_comando_interno(comando);
}

// Comandos internos (mínimo por ahora)
int Executor::ejecutar_interno(const vector<string>& tokens) {
    // Persona 2 implementará esto más detallado
    
    return Builtins::ejecutar_comando_interno(tokens);
}

// IMPLEMENTAR FORK + EXEC + redirección
int Executor::ejecutar_externo(const vector<string>& tokens, bool redirigir, const string& archivo) {
    cout << "Ejecutando comando externo: " << tokens[0] << endl;

    pid_t pid = fork();
    
    if (pid == 0) {
        // PROCESO HIJO

        if (redirigir) {
            if (Redirection::configurar_redireccion_salida(archivo) != 0) {
                exit(1);
            }
        }


        // Convertir vector<string> a char*[] para execvp
        char** argv = vector_a_argv(tokens);
        
        // Ejecutar el comando
        execvp(argv[0], argv);

        // Si llegamos aquí, execvp falló
        cerr << "Error: comando '" << tokens[0] << "' no encontrado" << endl;
        liberar_argv(argv);
        exit(1);
        
    } else if (pid > 0) {
        //  PROCESO PADRE - Esperar al hijo
        int estado;
        waitpid(pid, &estado, 0);
        
        if (WIFEXITED(estado)) {
            return WEXITSTATUS(estado);
        } else {
            cerr << "El proceso hijo terminó anormalmente" << endl;
            return -1;
        }
        
    } else {
        //  Error en fork
        cerr << "Error: no se pudo crear proceso hijo" << endl;
        return -1;
    }
}

int Executor::ejecutar_externo(const vector<string>& tokens) {
    return ejecutar_externo(tokens, false, "");
}

// Función auxiliar: convertir vector<string> a char*[]
char** Executor::vector_a_argv(const std::vector<std::string>& tokens) {
    char** argv = new char*[tokens.size() + 1];  // +1 para NULL al final
    
    for (size_t i = 0; i < tokens.size(); i++) {
        argv[i] = strdup(tokens[i].c_str());  // Copiar cada string
    }
    argv[tokens.size()] = NULL;  // NULL al final para execvp
    
    return argv;
}

// Liberar memoria de argv
void Executor::liberar_argv(char** argv) {
    for (int i = 0; argv[i] != NULL; i++) {
        free(argv[i]);
    }
    delete[] argv;
}