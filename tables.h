# include <iostream>
# include <filesystem>
# include <fstream>
# include <sstream>
# include <variant>
# include <string>
# include <vector>
# include <iomanip>
# include <algorithm>
# include <unordered_set>


#define GREEN   "\033[32m"
#define RESET   "\033[0m"

using namespace std;

void write_table(vector<vector<string>> data, string owner_database,string table_name) {
    string path = "./data/"+owner_database+"/"+table_name+".csv";
    ofstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << path << " for writing." << endl;
        return;
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1)
                file << ","; 
        }
        file << "\n"; 
    }

    file.close();
}

class tables{
private:
    vector<vector<string>> data;
    string name;
public:
    
    vector<vector<string>> read(string owner_database,string table_name);
    void create_table(string owner_database,string table_name);
    void delete_table(string owner_database,string table_name);
    void addcol(vector<string> col);
    vector<vector<string>> addrow(vector<string> row,vector<vector<string>> &data);
    vector<vector<string>> sort_asc(vector<vector<string>> data , string col);
    vector<vector<string>> sort_desc(vector<vector<string>> data , string col);
    vector<vector<string>> where(vector<vector<string>> &vec, string column, string op, string val );
    vector<vector<string>> select_by_col(vector<vector<string>> vec,unordered_set<string> col);
    void update_table(string owner_database,string table_name, string c1name,string c1value, string c2name,string c2value,string op);
    
};

void tables::create_table(string owner_database,string table_name){
    string path = "./data/"+owner_database+"/"+table_name+".csv";
    ofstream table(path);
}

void tables::delete_table(string owner_database, string table_name) {
    string path = "./data/" + owner_database + "/" + table_name + ".csv";
    if (filesystem::exists(path)) {
        filesystem::remove(path);
        cout << "Table '" << table_name << "' deleted successfully.\n";
    } else {
        cout << "Table '" << table_name << "' does not exist.\n";
    }
}

vector<vector<string>> tables::read(string owner_database,string table_name){
    data.clear();
    string path = "./data/"+owner_database+"/"+table_name+".csv";
    ifstream file(path);
    string line;

       while(getline(file,line)){
        stringstream ss(line);
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}

void displaytable(vector<vector<string>> data) {
    if (data.empty() || data[0].empty()) return;

    int rows = data.size();
    int cols = data[0].size();
    int cellWidth = 0;

    for (int j = 0; j < cols; j++) {
        int len = data[0][j].length();
        if (len > cellWidth) cellWidth = len;
    }
    cellWidth += 6;

    for (int j = 0; j < cellWidth * cols; j++) cout <<GREEN<< "=" <<RESET;
    cout << endl;

    for (int j = 0; j < cols; j++) {
        cout << setw(cellWidth - 2) << data[0][j] << GREEN<<" |"<<RESET;
    }
    cout << endl;

    for (int j = 0; j < cellWidth * cols; j++) cout <<GREEN<< "="<<RESET;
    cout << endl;

    for (int i = 1; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << setw(cellWidth - 2) << data[i][j] << GREEN<<" |"<<RESET;
        }
        cout << endl;
        for (int j = 0; j < cellWidth * cols; j++) cout <<GREEN <<"-"<<RESET;
        cout << endl ;
    }
}

void tables::addcol(vector<string> col){
        int index = data[0].size()-1;
        for(int i=0;i<data.size();i++){
            data[i].push_back(col[i]);
        }
}

vector<vector<string>> tables::addrow(vector<string> row,vector<vector<string>> &data){
   
    data.push_back(row);
    return data;
    
}

vector<vector<string>> tables::sort_asc(vector<vector<string>> vec, string column){
    int idx=-1;
    for(int i=0;i<vec[0].size();i++){
        if(vec[0][i]==column){
            idx=i;
            break;
        }
    }
    
    for(int i=1;i<vec.size();i++){
        for(int j=1;j<vec.size()-i;j++){
            if(vec[j][idx]>vec[j+1][idx]){
                swap(vec[j],vec[j+1]);
            }
        }
    }
    return vec;
}

vector<vector<string>> tables::sort_desc(vector<vector<string>> vec, string column){
    int idx=-1;
    for(int i=0;i<vec[0].size();i++){
        if(vec[0][i]==column){
            idx=i;
            break;
        }
    }
    
    for(int i=1;i<vec.size();i++){
        for(int j=1;j<vec.size()-i;j++){
            if(vec[j][idx]<vec[j+1][idx]){
                swap(vec[j],vec[j+1]);
            }
        }
    }
    return vec;
}

vector<vector<string>> tables::where(vector<vector<string>> &vec, string column, string op, string val ){
    vector<vector<string>> ans;
    ans.push_back(vec[0]);
    int idx=-1;
    for(int i=0;i<vec[0].size();i++){
        if(vec[0][i]==column){
            idx=i;break;
        }
    }
    
    if(idx==-1){
        return ans;
    }
    if(op=="="){
        for(int i=1;i<vec.size();i++){
            if(vec[i][idx]==val){
                ans.push_back(vec[i]);
            }
        }
    }
    else if(op=="<"){
        for(int i=1;i<vec.size();i++){
            if(vec[i][idx]<val){
                ans.push_back(vec[i]);
            }
        }
    }
    else if(op=="<="){
        for(int i=1;i<vec.size();i++){
            if(vec[i][idx]<=val){
                ans.push_back(vec[i]);
            }
        }
    }
    else if(op==">"){
        for(int i=1;i<vec.size();i++){
            if(vec[i][idx]>val){
                ans.push_back(vec[i]);
            }
        }
    }
    else if(op==">="){
        for(int i=1;i<vec.size();i++){
            if(vec[i][idx]>=val){
                ans.push_back(vec[i]);
            }
        }
    }
    else if(op=="!="){
        for(int i=1;i<vec.size();i++){
            if(vec[i][idx]!=val){
                ans.push_back(vec[i]);
            }
        }
    }
    else{
        
        return ans;
    }
    return ans;
}

vector<vector<string>> tables::select_by_col(vector<vector<string>> vec, unordered_set<string> col){
    vector<vector<string>>ans;
    for(int i=0;i<vec.size();i++){
        vector<string>temp;
        for(int j=0;j<vec[i].size();j++){
            if(col.find(vec[0][j])!=col.end()){
                temp.push_back(vec[i][j]);
            }
        }
        ans.push_back(temp);
    }
    return ans;
}

void tables::update_table(string owner_database,string table_name, string c1name,string c1value, string c2name,string c2value,string op){

    tables t;
    vector<vector<string>> vec = t.read(owner_database,table_name);
    int c1idx=-1;
    int c2idx=-1;
    for(int i=0;i<vec[0].size();i++){
        if(vec[0][i]==c1name){
            c1idx=i;
        }
        if(vec[0][i]==c2name){
            c2idx=i;
        }
    }

    if (c1idx == -1 || (op != "" && c2idx == -1)) {
        cout << "Not a valid column name.\n";
        return;
    }

    else{
        for(int i=1;i<vec.size();i++){
            if(op=="="){
                if(vec[i][c2idx] ==c2value){
                    vec[i][c1idx]=c1value;
                }
            }
            if(op==">"){
                if(vec[i][c2idx] >c2value){
                    vec[i][c1idx]=c1value;
                }
            }
            if(op==">="){
                if(vec[i][c2idx] >=c2value){
                    vec[i][c1idx]=c1value;
                }
            }
            if(op=="<"){
                if(vec[i][c2idx] <c2value){
                    vec[i][c1idx]=c1value;
                }
            }
            if(op=="<="){
                if(vec[i][c2idx] <=c2value){
                    vec[i][c1idx]=c1value;
                }
            }
            if(op=="!="){
                if(vec[i][c2idx] !=c2value){
                    vec[i][c1idx]=c1value;
                }
            }
            if (op == ""){
                vec[i][c1idx] = c1value;
            } 
        }
    }

    write_table(vec,owner_database,table_name);
    return;
}



