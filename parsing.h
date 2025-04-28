#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <cctype>
#include <sstream>
#include "database.h"
#include "fetch.h"

string normalizeSpaces(const std::string& input) {
    stringstream ss(input);
    string word, result;

    while (ss >> word) {
        if (!result.empty())
            result += " ";
        result += word;
    }

    return result;
}

using namespace std;

enum QueryType {
    SELECT,
    INSERT,
    DELETE,
    UPDATE,
    UNKNOWN,
    CREATE,
    DROP,
    OPEN,
};

/// THE SORT ONE IS OF THE LEAST PRIORITY
/// SHOW , CLEAR , EXIT ----> ARE THE GENERAL COMMANDS

struct SelectQuery {
    string all_operator;
    unordered_set<string> columns;
    string table;
    string where_column;
    string where_operator;
    string where_value;
    string sort_by;
    string sort_type;
};

struct DropQuery{
    string table;
    string database;
};

struct DeleteQuery{
    string table;
    vector<string> rows;
    vector<string> cols;
};

struct InsertQuery {
    string table;
    vector<string> values;
};


struct CreateQuery{
    string table;
    string database;
};

struct UpdateQuery {
    string table;
    string set_column;
    string set_value;
    string where_column;
    string where_operator;
    string where_value;
};

struct OpenQuery {
    string database_name;
};

struct ASTNode {
    QueryType type;
    SelectQuery selectQuery;
    InsertQuery insertQuery;
    DeleteQuery deleteQuery;
    UpdateQuery updateQuery;
    CreateQuery createQuery;
    OpenQuery openQuery;
    DropQuery dropQuery;
};


class DatabaseManager {
    private:
        static DatabaseManager* instance;
        string current_database;
        
        DatabaseManager() {} 
    
    public:
        static DatabaseManager* getInstance() {
            if (!instance) {
                instance = new DatabaseManager();
            }
            return instance;
        }
    
        void setCurrentDatabase(const string& dbName) {
            current_database = dbName;
        }
    
        string getCurrentDatabase() const {
            return current_database;
        }
    };
    
    DatabaseManager* DatabaseManager::instance = nullptr;

    inline string getCurrentDatabase() {

        string path = "./data/" + DatabaseManager::getInstance()->getCurrentDatabase();
        if (filesystem::exists(path)){
            return DatabaseManager::getInstance()->getCurrentDatabase();
        }
        
        return "";
    }

    


class Parser {
    vector<string> tokens;
    int current = 0;

public:
    Parser(const vector<string>& toks) : tokens(toks) {}

    string peek() {
        return current < tokens.size() ? tokens[current] : "";
    }

    string advance() {
        return current < tokens.size() ? tokens[current++] : "";
    }

    bool match(const string& expected) {
        if (peek() == expected) {
            advance();
            return true;
        }
        return false;
    }

    ASTNode parse() {
        if (match("SELECT")) return parseSelect();
        if (match("INSERT")) return parseInsert();
        if (match("DELETE")) return parseDelete();
        if (match("UPDATE")) return parseUpdate();
        if (match("CREATE")) return parseCreate();
        if (match("OPEN")) return parseOpen();
        if (match("DROP")) return parseDrop();
        return { UNKNOWN };
    }

private:
    ASTNode parseSelect() {
        SelectQuery query;

        if (peek() == "*") {
            query.all_operator = advance(); 
            query.columns = {};            

            if (!match("FROM")) {
                cout << "Syntax Error: Expected 'FROM' after '*'" << endl;
                return { UNKNOWN };
            }

            query.table = advance(); 
        } 
        else {
           
            while (!match("FROM")) {
                string col = advance();
                if (col != ",") query.columns.insert(col);
            }

            query.table = advance(); 
        }

        if (match("WHERE")) {
            query.where_column = advance();
            query.where_operator = advance();
            query.where_value = advance();
        }

 
        if (match("SORT")) {
            if (!match("BY")) {
                cout << "Syntax Error: Expected 'BY' after 'SORT'" << endl;
            } else {
                query.sort_by = advance();
                query.sort_type = advance();
            }
        }

        return { SELECT, query };
    }

    ASTNode parseInsert() {
        InsertQuery query;
        match("INTO"); // optional
        query.table = advance();
        match("VALUES");
        match("(");
        while (!match(")")) {
            string val = advance();
            if (val != ",") query.values.push_back(val);
        }
        return { INSERT, {}, query };
    }

    ASTNode parseDelete() {
        
    }

    ASTNode parseUpdate() {
        UpdateQuery query;
        match("FROM"); // optional
        query.table = advance();
        match("SET");
        query.set_column = advance();
        match("="); // expected
        query.set_value = advance();

        if (match("WHERE")) {
            query.where_column = advance();
            query.where_operator = advance();
            query.where_value = advance();
        }
        return { UPDATE, {}, {}, {}, query };
    }

    ASTNode parseCreate() {
        CreateQuery query;
        if(match("TABLE")){
            query.table = advance();

        }
        else if(match("DATABASE")){
            query.database = advance();
            
        }

        return {CREATE, {}, {}, {}, {}, query};

    }

    ASTNode parseOpen(){
        OpenQuery query;
        query.database_name = advance();
        return {OPEN,{},{},{},{},{},query};
    }

    ASTNode parseDrop(){
        DropQuery query;
        if(match("TABLE")){
            query.table = advance();
        }

        if(match("DATABASE")){
            query.database = advance();
        }

        return {DROP,{},{},{},{},{},{},query};

    }

};


class ExecutionEngine {
private:
    DatabaseManager* dbManager;
public:
    ExecutionEngine() {
        dbManager = DatabaseManager::getInstance();
    }

    void execute(const ASTNode& ast) {
        switch (ast.type) {
            case SELECT:  runSelect(ast.selectQuery); break;
            case INSERT:  runInsert(ast.insertQuery); break;
            case DELETE:  runDelete(ast.deleteQuery); break;
            case UPDATE:  runUpdate(ast.updateQuery); break;
            case CREATE:  runCreate(ast.createQuery); break;
            case OPEN:    runOpen(ast.openQuery); break;
            case DROP:    runDrop(ast.dropQuery); break;
            default: cout << "Unknown query type\n";
        }
    }

private:
    void runSelect(const SelectQuery& q);
    void runInsert(const InsertQuery& q);
    void runDelete(const DeleteQuery& q);
    void runUpdate(const UpdateQuery& q);
    void runCreate(const CreateQuery& q);
    void runOpen(const OpenQuery& q);
    void runDrop(const DropQuery& q);
};

void ExecutionEngine::runOpen(const OpenQuery& q){
    dbManager->setCurrentDatabase(q.database_name);
    string path = "./data/" + dbManager->getCurrentDatabase();
    if (filesystem::exists(path)){
        cout << "Using database: " << dbManager->getCurrentDatabase() << endl;
    }
    else{
        cout << "Database not found" << endl;
        dbManager->setCurrentDatabase("");
    }
}

void ExecutionEngine::runDrop(const DropQuery& q){
    tables t;
    databases d;
    if(!q.database.empty()){
        d.delete_database(q.database);
        remove_database(fetch_structure(),q.database);
    }

    if(!q.table.empty()){
        if(dbManager->getCurrentDatabase() == ""){
            cout << "Please Open a Database to drop a Table ..." << endl;
        }
        else{
            t.delete_table(dbManager->getCurrentDatabase(),q.table);
            remove_table(fetch_structure(),dbManager->getCurrentDatabase(),q.table);
        }
        
    }

    else if(q.database.empty() && q.table.empty()){
        cout << "Specify what to delete ... " << endl;
    }
}

void ExecutionEngine::runInsert(const InsertQuery& q) {
    tables t;
    vector<vector<string>> data = t.read(dbManager->getCurrentDatabase(),q.table);
    vector<string> columnHeaders = data[0];
    // Add validation to ensure values match expected column count
    
}

void ExecutionEngine::runSelect(const SelectQuery& q) {
    tables t;

    // OPEN
    if (dbManager->getCurrentDatabase().empty()) {
        cout << "Please open a database to select a table ..." << endl;
        return;
    }

    // CHECK TABLE EXIST
    vector<vector<string>> data = t.read(dbManager->getCurrentDatabase(), q.table);
    if (data.empty()) {
        cout << "Table is empty or does not exist ..." << endl;
        return;
    }

    // CHECK WHERE
    if (!q.where_column.empty() && !q.where_operator.empty() && !q.where_value.empty()) {
        data = t.where(data, q.where_column, q.where_operator, q.where_value);
        if (data.size() <= 1) { // only headers left
            cout << "No rows match the WHERE condition ..." << endl;
            return;
        }
    }

    // CHECK SORT BY
    if (!q.sort_by.empty() && !q.sort_type.empty()) {
        if (q.sort_type == "ASC") {
            data = t.sort_asc(data, q.sort_by);
        } else if (q.sort_type == "DESC") {
            data = t.sort_desc(data, q.sort_by);
        } else {
            cout << "Invalid sort type. Use ASC or DESC." << endl;
            return;
        }
    }

    // CHECK *
    if (q.all_operator == "*") {
        displaytable(data);
    } else if (!q.columns.empty()) {
        data = t.select_by_col(data, q.columns);
        displaytable(data);
    } else {
        cout << "Invalid Query. No columns specified and no '*' operator." << endl;
    }
}


void ExecutionEngine::runDelete(const DeleteQuery& q) {
    return;
}

void ExecutionEngine::runCreate(const CreateQuery& q) {
    if (q.table.empty()) {
        if (q.database.empty()) {
            cout << "Invalid query ." << endl;
            return;
        }

        string path = "./data/" + q.database;

        if (filesystem::exists(path)) {
            cout << "Database '" << q.database << "' already exists." << endl;
        } else {
            databases(q.database);

            if (filesystem::exists(path)) {
                cout << "Created Database '" << q.database << "'." << endl;
                add_database(fetch_structure(),q.database);
            } 
            else {
                cout << "Database not created OR Unknown query ..." << endl;
            }
        }
    }

    else if(q.database.empty()){
        if(dbManager->getCurrentDatabase() == ""){
            cout << "Please Open a Database to create a Table ..." << endl;
        }

        else {

            tables t;
            t.create_table(dbManager->getCurrentDatabase(),q.table);
            add_table(fetch_structure(),dbManager->getCurrentDatabase(),q.table);
            cout << q.table << " created successfully in database " << dbManager->getCurrentDatabase() << " ..." << endl;
        }
    }
}

void ExecutionEngine::runUpdate(const UpdateQuery& q) {
    tables t;

    if (dbManager->getCurrentDatabase().empty()) {
        cout << "Please open a database to update a table ..." << endl;
        return;
    }

    
    vector<vector<string>> data = t.read(dbManager->getCurrentDatabase(), q.table);
    if (data.empty()) {
        cout << "Table is empty or does not exist ..." << endl;
        return;
    }


    if (!q.where_column.empty() && !q.where_operator.empty() && !q.where_value.empty()) {
        vector<vector<string>> filteredData = t.where(data, q.where_column, q.where_operator, q.where_value);

        if (filteredData.size() <= 1) { 
            cout << "No rows match the WHERE condition ..." << endl;
            return;
        }
        
        t.update_table(dbManager->getCurrentDatabase(), q.table, q.set_column, q.set_value, q.where_column, q.where_value, q.where_operator);
        cout << "Update successful on matching rows." << endl;
    } 

    else {
        t.update_table(dbManager->getCurrentDatabase(), q.table, q.set_column, q.set_value, "", "", "");
        cout << "Update successful on all rows (no WHERE condition)." << endl;
    }
    
}
