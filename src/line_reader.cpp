#include "line_reader.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

using namespace std;

string StdinReader::read(const string& prompt) {
#ifdef USE_READLINE
    // Si está disponible Readline, preferirla
    char* input = readline(prompt.c_str());
    if (!input) return string();
    string cmd(input);
    if (!cmd.empty()) add_history(input);
    free(input);
    return cmd;
#else
    cout << prompt;
    string cmd;
    if (!getline(cin, cmd)) return string();
    return cmd;
#endif
}

#ifdef USE_READLINE
string ReadlineReader::read(const string& prompt) {
    char* input = readline(prompt.c_str());
    if (!input) return string();
    string cmd(input);
    if (!cmd.empty()) add_history(input);
    free(input);
    return cmd;
}
#endif
