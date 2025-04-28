#include <iostream>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include "parsing.h"
#include "lexer.h"
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"

bool check_general(string &cmd){
    if(!(strcasecmp(cmd.c_str(),"CLEAR")) || !(strcasecmp(cmd.c_str(),"CLS"))){
        system("CLS");
        return true;
    }

    if(!(strcasecmp(cmd.c_str(),"SHOW DATABASES"))){

        // THESE ONES TAKEN FROM THE fetch.h HEADER FILE LOADED IN parsing.h
        show_tree(fetch_structure());
        return true;
    }

    return false;
}

int main(){
    while(1)
    {
        
        string currentDB = getCurrentDatabase();
        cout<<endl;
        cout << BOLD << RED << currentDB << " " << BOLD << GREEN << "> " << RESET;

        string cmd;
        getline(cin, cmd);
        cmd = normalizeSpaces(cmd);

        if (cmd.empty()) {
            continue;
        }
        
        if(!strcasecmp(cmd.c_str(),"EXIT")){
            cout << "Exiting the running instance ... \n\n";
            break;
        }

        if(check_general(cmd)) continue;

        lex l;
        vector<string> tokens = l.tokenize(cmd);

        Parser p(tokens);
        ASTNode ast = p.parse();
        
        ExecutionEngine engine;
        engine.execute(ast);
}
    
}