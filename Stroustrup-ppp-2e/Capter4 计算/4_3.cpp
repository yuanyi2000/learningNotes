#include "../std_lib_facilities.h"

int main()
{
    vector<double> vd;
    cout << "enter some numbers" << endl;
    double num = 0;
     
    while(cin >> num)
    {
        if(num < 0) error("not correct number");
        vd.push_back(num);
    }
    
    double sum = 0;
    double min = vd[0];
    double max = vd[0];
    double adv = 0;
    
    for(int i = 0;i < vd.size();i++)
    {
        sum += vd[i];
        if(min > vd[i]) min = vd[i];
        if(max < vd[i]) max = vd[i];
    }
    adv = sum / (vd.size() - 1);
    
    cout << "sum" << sum << endl
         << "max" << max << endl
         << "min" << min << endl
         << "adv" << adv << endl;
         
}






