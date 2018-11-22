#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>


using namespace std;

int test_text()
{
    int i=0, x=0, y=0;
    ofstream myfile;
    myfile.open ("example1.txt");

    for (int j=0; j < 3; j++)
    {
        myfile  << i ;
        myfile  << " " << x;
        myfile  << " " << y << endl;
        i++;
        x = x + 2;
        y = x + 1;
    }
    myfile.close();
    return 0;
}

