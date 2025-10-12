#include "builtins.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

int Builtins::ejecutar_comando_interno(const vector<string>& tokens) {
    if (tokens.empty()) {
        return -2; // No es comando interno
    }
    
    string comando = tokens[0];
    
    if (comando == "cd") {
        return cd(tokens);
    }
    else if (comando == "pwd") {
        return pwd(tokens);
    }
    else if (comando == "help") {
        return help(tokens);
    }
    else if (comando == "exit" || comando == "salir") {
        return exit_shell(tokens);
    }
    
    return -2; // No es comando interno
}

int Builtins::cd(const vector<string>& args) {
    if (args.size() > 2) {
        cerr << "cd: demasiados argumentos" << endl;
        return -1;
    }
    
    const char* directorio;
    if (args.size() == 1) {
        // cd sin argumentos - ir a HOME
        directorio = getenv("HOME");
        if (!directorio) {
            cerr << "cd: variable HOME no está definida" << endl;
            return -1;
        }
    }
    else {
        // cd con directorio
        directorio = args[1].c_str();
    }
    
    if (chdir(directorio) != 0) {
        perror("cd");
        return -1;
    }
    
    return 0; // Éxito
}

int Builtins::pwd(const vector<string>& args) {
    if (args.size() > 1) {
        cerr << "pwd: no acepta argumentos" << endl;
        return -1;
    }
    
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        cout << buffer << endl;
        return 0;
    }
    else {
        perror("pwd");
        return -1;
    }
}

int Builtins::help(const vector<string>& args) {
    if (args.size() > 1) {
        cerr << "help: no acepta argumentos" << endl;
        return -1;
    }
    
    mostrar_ayuda_completa();
    return 0;
}

int Builtins::exit_shell(const vector<string>& args) {
    if (args.size() > 1) {
        cerr << "exit: no acepta argumentos. Use 'exit' o 'salir' sin argumentos" << endl;
        return -1;
    }
    
    cout << "Saliendo de la mini-shell..." << endl;
    exit(0); // Termina el programa
}

bool Builtins::es_comando_interno(const string& comando) {
    return (comando == "cd" || comando == "pwd" || comando == "help" || comando == "exit" || comando == "salir");
}

void Builtins::mostrar_ayuda_completa() {
    cout << "MINI-SHELL - COMANDOS DISPONIBLES" << endl;
    cout << "====================================" << endl;
    cout << "COMANDOS INTERNOS:" << endl;
    cout << "  cd [directorio]    - Cambiar directorio" << endl;
    cout << "  pwd                - Mostrar directorio actual" << endl;
    cout << "  help               - Mostrar esta ayuda" << endl;
    cout << "  exit o salir       - Salir del shell" << endl;
    cout << endl;
    cout << "COMANDOS EXTERNOS:" << endl;
    cout << "  ls, cat, echo, etc. - Cualquier comando del sistema" << endl;
    cout << endl;
    cout << "REDIRECCIONES:" << endl;
    cout << "  comando > archivo  - Redirigir salida a archivo" << endl;
    cout << endl;
}