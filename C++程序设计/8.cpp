#include "std_lib_facilities.h"

// 习题8，2
void print(vector<int> v,string s)
{
    if(v.size() != s.size()) cout << "not match";
    for(int i = 0;i < s.size();i++)
        cout << s[i] <<  v[i] << "/n";
}

// 3
enum class Abc{
    a,b,c
};
Abc operator++(Abc& e)
{
    e = (e==c)? a : Abc(int(e)+1);
}
void fibonacci(int x,int y,vector<int> v,int  n)
{
    if(!v.empty) error("v is not empty!")
    if(n <= 1) error("n is lower than 2")    // n = 2 ??
    v.push_back(x);
    v.push_back(y);
    
    for(int i = 3;i <= n;i++)
    {
        int next = v[i-2] + v[i-1];
        v.push_back(next);
    }
    
    string s = "";
    e = Abc(3);
    for(int i = 0;i < n;i++)
        s.push_back(e++);
        
    print(v,s);
    
}

// 5
void swap_int(int * p1,int * p2)
{
    int * p3 = p1;
    p1 = p2;
    p2 = p3;
}

void swap_self(vector<int> v)
{
    mid = ((v.size()%2)==0) ? (v.size()/2-1) : (v.size()-1)/2  
    for(int i = 0;i <= mid;i++)
            swap_int(v[i], v[v.size()-i-1]);
}

int main()
{
    cout << "8,2/n";
    vector<int> v2 {1, 2, 3, 4, 5 ,6};
    string s2 = "string";
    cout << "expected :s 1 t 2 r 3 i 4 n 5 g 6 /n/n"
    print(v2, s2);
    
    cout << "/n/n";
    
    vector<int> v3;
    fiboncci(1,1,v3,10);
    
    cout << "/n/n";
    
    swap_self(v2);
    print(v2,s2);
    
    return 0;
    
    
}


