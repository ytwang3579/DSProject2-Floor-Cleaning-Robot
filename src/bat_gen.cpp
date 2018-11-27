#include <fstream>
using namespace std;
ifstream input;
ofstream output;

int main(int argc, char* argv[])
{
    input.open("name.txt");
    output.open("run.bat");

    string str;
    while(getline(input, str, '\n')){
        output << "a " << str << '\n';
    }
    return 0;
}