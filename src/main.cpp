#include <iostream>
#include "shell.h"

using namespace std;

int main() {
    cout << "=== Iniciando Mini-Shell ===" << endl;

    try {
        Shell mi_shell;
        mi_shell.ejecutar();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    cout << "=== Shell finalizada ===" << endl;
    return 0;
}