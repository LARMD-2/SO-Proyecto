#include "builtins.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <sstream>

using namespace std;

// Definiciones de estáticos
vector<string> Builtins::historial;
unordered_map<string, vector<string>> Builtins::aliases;

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
    else if (comando == "history") {
        return history(tokens);
    }
    else if (comando == "alias") {
        return alias(tokens);
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
    return (comando == "cd" || comando == "pwd" || comando == "help" || comando == "exit" || comando == "salir" || comando == "history" || comando == "alias");
}

void Builtins::mostrar_ayuda_completa() {
    cout << "MINI-SHELL - COMANDOS DISPONIBLES" << endl;
    cout << "====================================" << endl;
    cout << "COMANDOS INTERNOS:" << endl;
    cout << "  cd [directorio]      - Cambiar directorio" << endl;
    cout << "  pwd                  - Mostrar directorio actual" << endl;
    cout << "  help                 - Mostrar esta ayuda" << endl;
    cout << "  history              - Mostrar historial de comandos" << endl;
    cout << "  alias nombre=valor   - Definir alias (valor puede ser varios tokens)" << endl;
    cout << "  exit o salir         - Salir del shell" << endl;
    cout << endl;
    cout << "COMANDOS EXTERNOS:" << endl;
    cout << "  ls, cat, echo, etc. - Cualquier comando del sistema" << endl;
    cout << endl;
    cout << "REDIRECCIONES:" << endl;
    cout << "  comando > archivo  - Redirigir salida a archivo" << endl;
    cout << endl;
}

int Builtins::history(const vector<string>& args) {
    if (args.size() > 1) {
        cerr << "history: no acepta argumentos" << endl;
        return -1;
    }
    for (size_t i = 0; i < historial.size(); ++i) {
        cout << i + 1 << ": " << historial[i] << endl;
    }
    return 0;
}

void Builtins::registrar_historial(const string& linea) {
    if (!linea.empty()) historial.push_back(linea);
}

int Builtins::alias(const vector<string>& args) {
    // Formato simple: alias nombre=valor
    if (args.size() < 2) {
        // Listar aliases actuales
        for (const auto& kv : aliases) {
            cout << "alias " << kv.first << "='";
            for (size_t i = 0; i < kv.second.size(); ++i) {
                cout << kv.second[i] << (i + 1 < kv.second.size() ? " " : "");
            }
            cout << "'" << endl;
        }
        return 0;
    }
    // Unir args[1..] por espacios para permitir nombre=valor con espacios
    string def;
    for (size_t i = 1; i < args.size(); ++i) {
        if (i > 1) def += " ";
        def += args[i];
    }
    auto pos = def.find('=');
    if (pos == string::npos || pos == 0 || pos == def.size() - 1) {
        cerr << "alias: formato inválido. Use: alias nombre=valor" << endl;
        return -1;
    }
    string nombre = def.substr(0, pos);
    string valor = def.substr(pos + 1);
    // Remover comillas simples/dobles exteriores si existen
    if (valor.size() >= 2 && ((valor.front() == '\'' && valor.back() == '\'') || (valor.front() == '"' && valor.back() == '"'))) {
        valor = valor.substr(1, valor.size() - 2);
    }
    // Tokenizar valor en vector<string>
    istringstream iss(valor);
    vector<string> val_tokens;
    string tok;
    while (iss >> tok) val_tokens.push_back(tok);
    aliases[nombre] = val_tokens;
    return 0;
}

void Builtins::expandir_alias(vector<string>& tokens) {
    if (tokens.empty()) return;
    auto it = aliases.find(tokens[0]);
    if (it == aliases.end()) return;
    // Reemplazar primer token por el alias expandido conservando argumentos
    vector<string> nuevos;
    nuevos.insert(nuevos.end(), it->second.begin(), it->second.end());
    if (tokens.size() > 1) {
        nuevos.insert(nuevos.end(), tokens.begin() + 1, tokens.end());
    }
    tokens.swap(nuevos);
}