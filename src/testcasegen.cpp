#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream out("./test/floor.data");
    if(!out){
        cout << "Unable to write data!!!\n";
        return 1;
    }

    out << "1000 1000 2000\n";
    for(int i=0; i<1000; i++){
        for(int j=0; j<1000; j++){
            out << ((i==500 && j==500) ? 'R' : ((rand()%2) ? '0' : '0'));
            out << ((j==999) ? '\n' : ' ');
        }
    }

    return 0;
}