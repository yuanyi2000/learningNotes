// cin cout
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string word;
	while( cin >> word )
		cout << "word read is:" << word << endl;
	cout << "ok,no more words to read.\n";
	return 0;
}