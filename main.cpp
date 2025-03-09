#include <iostream>
#include "database.h"
#include <vector>
using namespace std;

int main(){
    string cmd;
    string name;
    // getline(cin, name, '\n');
    cout<<"enter the name of database : ";
    getline(cin, name, '\n');
    vector<databases> record;
    databases r1(name);
    record.push_back(r1);
}