#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream floor, path;
    if(argc != 2){
        cout << "Input Error!!\n";
    }
    stringstream strfloor, strpath;
    strfloor << "./" << argv[1] << "/floor.data";
    strpath << "./" << argv[1] << "/final.path";
    floor.open(strfloor.str());
    path.open(strpath.str());
    if(!floor){
        cout << "Couldn't read ./" << argv[1] << "/floor.data!!\n";
    }
    if(!path){
        cout << "Couldn't read ./" << argv[1] << "/final.path!!\n";
    }

    int M, N, B, ri, rj;
    floor >> M >> N >> B;
    cout << M << ' ' << N << ' ' << B << '\n';

    char map[M][N];
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            floor >> map[i][j];
            if(map[i][j]=='R'){
                ri = i;
                rj = j;
            }
        }
    }

    int curB = B;
    int lasti, lastj, lastlasti, lastlastj;
    int nowi = ri;
    int nowj = rj;
    bool firsttime = true;
    int nouse;
    path >> nouse;
    while(1){
        if(firsttime==false){
            lastlasti = lasti;
            lastlastj = lastj;
        }
        lasti = nowi;
        lastj = nowj;

        if(!(path >> nowi >> nowj)) break;
        if((nowi!=ri) || (nowj!=rj)){
            curB--;
            if(curB<=0){
                cout << "RUN OUT OF BATTERY!!\n";
                return 1;
            }
        } else {
            curB = B;
        }

        if(firsttime==true){
            lastlasti = nowi;
            lastlastj = nowj;
            firsttime = false;
        }

        if((nowi != (lasti-1)) && (nowi != (lasti+1)) &&
        (nowj != (lastj-1)) && (nowj != (lastj+1))){
            cout << "JUMP STEP: (" << lasti << ',' << lastj << ") -> (" << nowi << ',' << nowj << ")!!\n";
            return 1;
        }

        if((lasti==ri) && (lastj==rj)){
            if((nowi!=lastlasti) && (nowj!=lastlastj)){
                cout << "RECHARGE NEED TO BE I/O WITH SAME DIRECTION!!\n";
                return 1;
            }
        }

        if(map[nowi][nowj]=='1'){
            cout << "PATH THROUGH WALL : (" << nowi << ',' << nowj << ")!!\n";
            return 1;
        }

        map[nowi][nowj] = 'D';

    }
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            if(map[i][j]=='0'){
                cout << "SOME ELEMENTS ARE NOE CLEAN YET : (" << i << ',' << j << ")!!\n";
                return 1;
            }
        }
    }

    cout << argv[1] << " PASS!!CONGRATS!!\n";
    return 0;
    
}