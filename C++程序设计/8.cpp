#include <iostream>
#include <string>
#include <vector>

// 头文件回去加

using namespace std;

// 习题8，2
void print(vector<int> v,string s)
{
    if(v.size() != s.size()) cout << "not match";
    for(int i = 0;i < s.size();i++)
        cout << s[i] <<  v[i] << "/n";
}

// 3
vector<int> fibonacci(int x,int y,vector<int> v,int  n)
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
    
    return v;
}
