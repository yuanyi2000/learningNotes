#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
	ofstream outfile("name-of-file");
	if( ! outfile )
		cerr << "Sorry!We were unable to open the file!\n";
	ifstream infile("name_of_file");
	if( ! infile )
		cerr << "Sorry!We were unable to open input file\n";
	string word;
	while(infile >> word)
		outfile << word << ' ';
	return 0;
}