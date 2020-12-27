#include "../std_lib_facilities.h"

int main()
{
    vector<string> vs;
    string s = "";
    
    cout << "please enyer 3 strings" << endl;
    
    for(int i = 0;i < 3;++i)
    {
        cin >> s;
        vs.push_back(s);
    }
    
    sort(vs[0], vs[2]);
    
    for(int i = 0;i < 3;++i)
        cout << vs[i];
        
    cout << endl;
}
