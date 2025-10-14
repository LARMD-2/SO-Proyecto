#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <string>

using namespace std;    

class Executor {
public:
    static int ejecutar_comando(const vector<string>& tokens);
    static bool es_comando_interno(const string& comando);
    static int ejecutar_interno(const vector<string>& tokens);

    static int ejecutar_externo(const vector<string>& tokens, bool redirigir, const string& archivo);
    static int ejecutar_externo(const vector<string>& tokens); // Overload sin redirección
    static int procesar_comando_con_pipes(const vector<string>& tokens);
    static int ejecutar_externo_background(const vector<string>& tokens);

private:
    static char** vector_a_argv(const vector<string>& tokens);
    static void liberar_argv(char** argv);
};

#endif