#include<iostream>
#include<bits/stdc++.h>
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void show_tree(const map<string, vector<string>>& tree)
{
    cout << endl;
    for (const auto& p : tree) {
        cout << BOLD << BLUE << "[DB] " << RESET << p.first << endl;

        for (size_t i = 0; i < p.second.size(); i++) {
            string indent = "   "; 
            cout << indent << CYAN << "|-- " << RESET << p.second[i] << endl;
        }
        cout << endl;
    }
}

map<string,vector<string>> fetch_structure(){
        string path = "./src/structure.csv";
        map<string,vector<string>> temp;
        ifstream file(path);
    
        if (!file.is_open()) {
            cerr << "Failed to open file: " << path << endl;
            return temp;
        }
    
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string word;
            vector<string> parts;
    
            while (getline(ss, word, ',')) {
                parts.push_back(word);
            }
    
            if (!parts.empty()) {
                string db = parts[0];
                vector<string> tables(parts.begin() + 1, parts.end());
                temp[db] = tables;
            }
        }
    
        file.close();
        return temp;
    }

// THE TASKS BELOW WORK TO UPDATE THE SRC FOLDER -- 

void save_structure(const map<string, vector<string>> tree) {
    string path = "./src/structure.csv";
    ofstream file(path);
    
    if (!file.is_open()) {
        cerr << "Failed to write to file: " << path << endl;
        return;
    }

    for (const auto& [db, tables] : tree) {
        file << db;
        for (const string& table : tables) {
            file << "," << table;
        }
        file << "\n";
    }

    file.close();
}

void add_database(map<string, vector<string>> tree,string db_name) {
    transform(db_name.begin(), db_name.end(), db_name.begin(), ::toupper);
    if (tree.find(db_name) != tree.end()) {
        cout << "Database already exists.\n";
        return;
    }

    tree[db_name] = {}; 
    save_structure(tree);
    cout << "Created Database '" << db_name << "'..."<<endl;
}

void add_table(map<string, vector<string>> tree,string db_name,string table_name) {
    transform(db_name.begin(), db_name.end(), db_name.begin(), ::toupper);
    transform(table_name.begin(), table_name.end(), table_name.begin(), ::toupper);
    
    tree[db_name].push_back(table_name);

    save_structure(tree);
    cout << "Created table '" << table_name << "'..." << endl;

}

void remove_database(map<string, vector<string>> tree, string db_name) {
    transform(db_name.begin(), db_name.end(), db_name.begin(), ::toupper);
    
    if (tree.find(db_name) == tree.end()) {
        return;
    }

    tree.erase(db_name);
    save_structure(tree);
}

void remove_table(map<string, vector<string>> tree, string db_name, string table_name) {
    transform(table_name.begin(), table_name.end(), table_name.begin(), ::toupper);
    transform(db_name.begin(), db_name.end(), db_name.begin(), ::toupper);
    
    vector<string> &tables = tree[db_name];
    auto it = find(tables.begin(), tables.end(), table_name);
    if (it != tables.end()) {
        tables.erase(it);
        save_structure(tree);
    } 
}




