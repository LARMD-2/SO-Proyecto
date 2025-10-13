#include "pipe_simple.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

using namespace std;

int PipeSimple::ejecutar_pipe_simple(
    const vector<string>& comando1,
    const vector<string>& comando2) {
    
    int pipefd[2];
    pid_t pid1, pid2;
    
    // Crear pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }
    
    // PRIMER PROCESO (cmd1)
    pid1 = fork();
    if (pid1 == 0) {
        // Proceso hijo 1 - escribirá en el pipe
        close(pipefd[0]);  // Cerrar extremo de lectura
        
        // Redirigir stdout al pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        // Preparar arguments para execvp
        vector<char*> args1;
        for (const auto& token : comando1) {
            args1.push_back(const_cast<char*>(token.c_str()));
        }
        args1.push_back(nullptr);
        
        // Ejecutar primer comando
        execvp(args1[0], args1.data());
        perror("execvp cmd1");
        exit(1);
    }
    
    // SEGUNDO PROCESO (cmd2)
    pid2 = fork();
    if (pid2 == 0) {
        // Proceso hijo 2 - leerá del pipe
        close(pipefd[1]);  // Cerrar extremo de escritura
        
        // Redirigir stdin desde el pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        // Preparar arguments para execvp
        vector<char*> args2;
        for (const auto& token : comando2) {
            args2.push_back(const_cast<char*>(token.c_str()));
        }
        args2.push_back(nullptr);
        
        // Ejecutar segundo comando
        execvp(args2[0], args2.data());
        perror("execvp cmd2");
        exit(1);
    }
    
    // PROCESO PADRE
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Esperar a que ambos procesos terminen
    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    
    return 0;
}