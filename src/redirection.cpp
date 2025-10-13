#include "redirection.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

// Detectar si hay redirección > y extraer el archivo
bool Redirection::tiene_redireccion_salida(const vector<string>& tokens, string& archivo) {
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == ">") {
            // Formato: comando > archivo
            if (i + 1 < tokens.size()) {
                archivo = tokens[i + 1];
                return true;
            }
        } else if (tokens[i][0] == '>' && tokens[i].size() > 1) {
            // Formato: comando >archivo
            archivo = tokens[i].substr(1);
            return true;
        }
    }
    return false;
}

// Configurar redirección de stdout a archivo
int Redirection::configurar_redireccion_salida(const string& archivo) {
    // Abrir archivo para escritura (crear/truncar)
    int fd = open(archivo.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error abriendo archivo para redirección");
        return -1;
    }
    
    // Redirigir stdout al archivo
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Error en dup2");
        close(fd);
        return -1;
    }
    
    close(fd); // Cerrar el descriptor original
    return 0;
}