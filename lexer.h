#include<iostream>
#include<vector>
#include<cctype>
using namespace std;

enum state{
    START,
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    STRING
};

class lex {
public: 
    vector<string> tokenize(string &cmd){
        vector<string> tokens_vector;
        state current = START;
        string token = "";
    }

    for(char c:cmd){
        switch(current){
            case START:
                if(isalpha(c)){current = KEYWORD; token+=c}
                else if(isdigit(c)){current = NUMBER; token+=c}
                else if(c == '\''){current = STRING; token+=c}
                else if(isspace(c)){continue;}
                else{current = OPERATOR; token+=c;}
                break;

            case KEYWORD:
            case IDENTIFIER:
                    if(isalnum(c)||c=='_'){token+=c}
                    else{tokens_vector.push_back(token);token=c;current=START}

            case STRING:
                    token += c;
                    if (c == '\'') { tokens.push_back(token); token = ""; current = START; }
                    break;

            case OPERATOR:
                    if (token == ">" && c == '=') { token += c; }
                    tokens.push_back(token); token = ""; current = START;
                    break;
        }
    }
};


