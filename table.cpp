#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

class table{
vector<int> num;
vector<string> name;
vector<double> price;

public:
    table(int num,string name,double price){
        this->num.push_back(num);
        this->name.push_back(name);
        this->price.push_back(price);
    }

    void add(int num,string name,double price){
        this->num.push_back(num);
        this->name.push_back(name);
        this->price.push_back(price);
    }

    void display(){
        cout << " ------------------------------------------------------ " << endl;
        cout << "| S.No   | Name                 | Price                |" << endl;
        cout << " ------------------------------------------------------ " << endl;
        for (size_t i = 0; i < num.size(); i++) {
            cout << "| " << setw(5) << num[i] << "  | "
                 << setw(20) << name[i] << " | "
                 << setw(20) << fixed << setprecision(2) << price[i] << " |" << endl;
        }
        cout << " ------------------------------------------------------ " << endl;
    }
    

};
int main()
{
    // while(9){
    //     int x, y;
    //     cout<<"> enter rows and columns : ";
    //     cin>>x>>y;

    //     for(int i=0;i<x;i++){
    //         for(int j=0;j<y;j++){
    //             cout<<"*  ";
    //         }
    //         cout<<endl;
    //     }
    // }

    table t1(1,"animal",500);
    t1.add(2,"aarnya_jain",700);
    t1.display();
}

