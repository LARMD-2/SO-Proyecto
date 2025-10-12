#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "core/builtins.h"
#include "core/parser.h"

using namespace std;

void probar_builtins() {
    cout << "PROBANDO COMANDOS INTERNOS - PERSONA 2" << endl;
    cout << "========================================" << endl;
    
    // Test cd
    cout << "1. Probando cd:" << endl;
    vector<string> args_cd = {"cd", "/tmp"};
    int resultado = Builtins::cd(args_cd);
    cout << "   Resultado: " << resultado << endl;
    
    // Test pwd
    cout << "2. Probando pwd:" << endl;
    vector<string> args_pwd = {"pwd"};
    resultado = Builtins::pwd(args_pwd);
    cout << "   Resultado: " << resultado << endl;
    
    // Test help
    cout << "3. Probando help:" << endl;
    vector<string> args_help = {"help"};
    resultado = Builtins::help(args_help);
    cout << "   Resultado: " << resultado << endl;
    
    // Test detección de comandos internos
    cout << "4. Probando detección:" << endl;
    cout << "   'cd' es interno: " << Builtins::es_comando_interno("cd") << endl;
    cout << "   'ls' es interno: " << Builtins::es_comando_interno("ls") << endl;
    
    // Test ejecutar_comando_interno
    cout << "5. Probando ejecución:" << endl;
    vector<string> comando = {"pwd"};
    resultado = Builtins::ejecutar_comando_interno(comando);
    cout << "   pwd → " << resultado << endl;
}

void probar_redirecciones() {
    cout << endl << "PROBANDO DETECCIÓN DE REDIRECCIONES" << endl;
    cout << "=====================================" << endl;
    
    vector<string> casos = {
        "ls > archivo.txt",
        "grep hola < entrada.txt",
        "echo texto >> salida.log",
        "cat < entrada.txt > salida.txt"
    };
    
    for (const auto& caso : casos) {
        cout << "Línea: " << caso << endl;
        
        vector<string> tokens = Parser::dividir_tokens(caso);
        string input_file, output_file;
        bool append;
        
        Parser::detectar_redirecciones(tokens, input_file, output_file, append);
        
        cout << "   Tokens: [";
        for (size_t i = 0; i < tokens.size(); ++i) {
            cout << "'" << tokens[i] << "'";
            if (i < tokens.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        
        cout << "   Input: '" << input_file << "'" << endl;
        cout << "   Output: '" << output_file << "'" << endl;
        cout << "   Append: " << append << endl;
        cout << "---" << endl;
    }
}

int main() {
    probar_builtins();
    probar_redirecciones();
    return 0;
}