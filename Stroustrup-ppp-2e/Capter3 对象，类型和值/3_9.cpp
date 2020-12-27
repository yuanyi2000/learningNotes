#include "../std_lib_facilities.h"

int main()
{
    cout << "Enter a spell-out number" << endl;
    
    string s = "";
    int res = -1;
    
    while(cin >> s)
    {
        if(s == "zero")       res = 0;
        else if(s == "one")   res = 1;
        else if(s == "two")   res = 2;
        else if(s == "three") res = 3;
        else if(s == "four")  res = 4;
        else if(s == "Quit"||s == "quit"||s == "q") return 0;
        
        if(res != -1) cout << res;
        else          cout << "\nnot a number i know\n";
        
        cout << "Please enter another number or \"Quit\"\n";
        res = -1;
    }
    return 0;
}
