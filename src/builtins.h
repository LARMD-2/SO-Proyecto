#ifndef BUILTINS_H
#define BUILTINS_H

#include <string>
#include <vector>

using namespace std;

class Builtins {
public:
    // Ejecuta un comando interno
    // Retorna: 0 si éxito, -1 si error, -2 si no es comando interno
    static int ejecutar_comando_interno(const vector<string>& tokens);
    
    // Cambiar directorio
    static int cd(const vector<string>& args);
    
    // Mostrar directorio actual
    static int pwd(const vector<string>& args);

    // Mostrar ayuda de comandos
    static int help(const vector<string>& args);

    // Salir del shell
    static int exit_shell(const vector<string>& args);

    // Verifica si un comando es interno
    static bool es_comando_interno(const string& comando);

private:
    static void mostrar_ayuda_completa();
};

#endif