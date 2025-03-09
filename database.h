#include <stdio.h>
#include <filesystem>
#include <string.h>

using std::string;

class databases{
private:
    string name;
public:

    databases(string name){
        string path = "./data/"+name; 
        std::filesystem::create_directories(path);
        this->name = name;
    }

};

