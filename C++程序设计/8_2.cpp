#include "std_lib_facilities.h"

// 7

int main(){
    vector<string> name[5];
    vector<double> age[5];
    
    string s_name;
    double d_age;
    
    for(int i = 0;i < 5;i++)
    {
        cout << "Enter the name of man no." << i << "/n";
        cin >> s_name;
        name[i] = s_name;
    }
    
    for(int i = 0; i < 5;i++)
    {
        cout << "The age of " << name[i] << "is ?/n";
        cin >> d_age;   // 如果这里不输入数字会怎样？
        if(d_age < 0) error("age not correct/n");
        age[i] = d_age;
    }
    
    
}

