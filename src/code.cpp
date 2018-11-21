#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
ifstream infile;
ofstream outfile;

#define TARGET (i*N+j)

int main(int argc, char* argv[])
{
    if(argc != 2){
        cout <<  "Input Error!!\n";
        return 1;
    }
    stringstream strin, strout;
    strin << "./" << argv[1] << "/floor.data";
    strout << "./" << argv[1] << "/final.path";

    infile.open(strin.str());
    outfile.open(strout.str());

    if(!infile){
        cout << "Cannot open file!!\n";
        return 1;
    }
    if(!outfile){
        cout << "Cannot write file!!\n";
        return 1;
    }

    int M, N, B;
    infile >> M >> N >> B;

    vector<char> map;
    map.reserve(M*N);

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            infile >> map[TARGET];
            cout << map[TARGET] << " ";
        }
        cout << '\n';
    }

    return 0;
}