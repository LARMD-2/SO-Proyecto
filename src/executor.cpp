#include "executor.h"
#include "parser.h"        
#include "pipe_simple.h"   
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include <builtins.h>
#include "redirection.h"
using namespace std;

// Resolver ruta del ejecutable según especificación:
// - Absoluta: usar tal cual
// - Con '/': tratar como ruta dada (relativa o absoluta)
// - Sin '/': intentar /bin/<cmd>
static std::string resolver_ruta_ejecutable(const std::string& cmd) {
    if (cmd.empty()) return cmd;
    if (cmd[0] == '/' || cmd.find('/') != std::string::npos) {
        return cmd; // absoluta o con ruta relativa explícita
    }
    return std::string("/bin/") + cmd; // fallback requerido
}

int Executor::procesar_comando_con_pipes(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return 0;

    // HACER COPIA para no modificar original
    std::vector<std::string> tokens_temp = tokens;
    
    // 1. DETECTAR BACKGROUND primero (quita "&" si existe)
    bool es_background = Parser::detectar_background(tokens_temp);
    
    // 2. Detectar PIPE (con tokens sin "&")
    int pos_pipe = Parser::detectar_pipe(tokens_temp);
    
    if (pos_pipe != -1) {
        // CASO PIPE (con o sin background)
        std::vector<std::vector<std::string>> comandos = Parser::dividir_en_dos_comandos(tokens_temp);
        
        if (es_background) {
            cout << "  Pipe en background - ejecutando en foreground por ahora" << endl;
            // Por simplicidad, pipe en foreground primero
        }
        
        return PipeSimple::ejecutar_pipe_simple(comandos[0], comandos[1]);
        
    } else {
        // CASO SIN PIPE 
        if (es_background) {
            // BACKGROUND simple
            return ejecutar_externo_background(tokens_temp);
        } else {
            // FOREGROUND normal con redirecciones
            std::string archivo_redireccion;
            bool redirigir = Redirection::tiene_redireccion_salida(tokens_temp, archivo_redireccion);
            
            // Filtrar tokens de redirección
            std::vector<std::string> tokens_filtrados;
            for (const auto& token : tokens_temp) {
                if (token != ">" && token.substr(0, 1) != ">" && token != archivo_redireccion) {
                    tokens_filtrados.push_back(token);
                }
            }

            if (tokens_filtrados.empty()) return 0;
            
            if (es_comando_interno(tokens_filtrados[0])) {
                return ejecutar_interno(tokens_filtrados);
            } else {
                if (redirigir) {
                    return ejecutar_externo(tokens_filtrados, true, archivo_redireccion);
                } else {
                    return ejecutar_externo(tokens_filtrados);
                }
            }
        }
    }
}

// MODIFICAR la función principal para usar el nuevo método
int Executor::ejecutar_comando(const vector<string>& tokens) {
    return procesar_comando_con_pipes(tokens);
}

// Verificar comandos internos
bool Executor::es_comando_interno(const string& comando) {
    return Builtins::es_comando_interno(comando);
}

// Comandos internos 
int Executor::ejecutar_interno(const vector<string>& tokens) {
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

        // Convertir vector<string> a char*[] para exec
        char** argv = vector_a_argv(tokens);
        
        // Resolver ruta acorde a la especificación (/bin fallback)
        std::string ruta = resolver_ruta_ejecutable(tokens[0]);

        if (tokens[0].find('/') == std::string::npos) {
            // Caso "cmd" sin '/': verificar /bin/cmd ejecutable
            if (access(ruta.c_str(), X_OK) != 0) {
                perror((std::string("no se puede ejecutar ") + ruta).c_str());
                liberar_argv(argv);
                exit(127);
            }
        }

        // Ejecutar el comando usando execv con ruta explícita
        execv(ruta.c_str(), argv);

        // Si llegamos aquí, execv falló
        perror((std::string("execv: ") + ruta).c_str());
        liberar_argv(argv);
        exit(1);
        
    } else if (pid > 0) {
        //  PROCESO PADRE - Esperar al hijo
        int estado;
        waitpid(pid, &estado, 0);
        
        if (WIFEXITED(estado)) {
            return WEXITSTATUS(estado);
        } else if (WIFSIGNALED(estado)) {
            int sig = WTERMSIG(estado);
            cerr << "El proceso hijo fue terminado por señal: " << strsignal(sig) << " (" << sig << ")" << endl;
            return 128 + sig; // convención de retorno por señales
        } else {
            cerr << "El proceso hijo terminó anormalmente (estado=" << estado << ")" << endl;
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

int Executor::ejecutar_externo_background(const std::vector<std::string>& tokens) {
    cout << "Ejecutando en background: ";
    for (const auto& t : tokens) cout << t << " ";
    cout << endl;

    pid_t pid = fork();
    
    if (pid == 0) {
        // PROCESO HIJO (background)
        char** argv = vector_a_argv(tokens);
        // Resolver ruta acorde a la especificación
        std::string ruta = resolver_ruta_ejecutable(tokens[0]);
        if (tokens[0].find('/') == std::string::npos) {
            if (access(ruta.c_str(), X_OK) != 0) {
                perror((std::string("no se puede ejecutar ") + ruta).c_str());
                liberar_argv(argv);
                exit(127);
            }
        }
        execv(ruta.c_str(), argv);

        // Si falla execv
        perror((std::string("execv: ") + ruta).c_str());
        liberar_argv(argv);
        exit(1);
        
    } else if (pid > 0) {
        // PROCESO PADRE - NO ESPERAR (background)
        cout << "[" << pid << "] proceso en background" << endl;
        return 0;
        
    } else {
    perror("fork");
    cerr << "Error: no se pudo crear proceso hijo" << endl;
        return -1;
    }
}