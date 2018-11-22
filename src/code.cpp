#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;
ifstream infile;
ofstream outfile;

#define TARGET (i*N+j)
#define T_UP ((i-1)*N+j)
#define T_DOWN ((i+1)*N+j)
#define T_LEFT (i*N+j-1)
#define T_RIGHT (i*N+j+1)

//global variable
int M, N, B, ri, rj;
vector<char> map;

vector<int> BFS(int i, int j, vector<char> tmap)
{

    vector<int> bfs(M*N,-1);
    if(i<0 || j<0 || i>=M || j>=N) return bfs;

    queue<pair<int, int>> q;
    pair<int, int> ptr;
    
    q.push(make_pair(i, j));
    bfs[TARGET] = 1;
    tmap[TARGET] = 'D';

    while(q.empty()==false){
        ptr = q.front();
        i = ptr.first;
        j = ptr.second;
        int cnt = bfs[TARGET] + 1;
        //cout << i << " " << j << ' ' << cnt << '\n';

        if(i>0) if(tmap[T_UP]=='0') {
            q.push(make_pair(i-1, j));
            bfs[T_UP] = cnt;
            tmap[T_UP] = 'D';
        }
        if(i<M-1) if(tmap[T_DOWN]=='0'){
            q.push(make_pair(i+1, j));
            bfs[T_DOWN] = cnt;
            tmap[T_DOWN] = 'D';
        }
        if(j>0) if(tmap[T_LEFT]=='0'){
            q.push(make_pair(i, j-1));
            bfs[T_LEFT] = cnt;
            tmap[T_LEFT] = 'D';
        }
        if(j<N-1) if(tmap[T_RIGHT]=='0'){
            q.push(make_pair(i, j+1));
            bfs[T_RIGHT] = cnt;
            tmap[T_RIGHT] = 'D';
        }

        q.pop();

    }

    return bfs;
}

void cleaning_naive()
{

    vector<int> bfs_up = BFS(ri-1, rj, map);
    vector<int> bfs_down = BFS(ri+1, rj, map);
    vector<int> bfs_left = BFS(ri, rj-1, map);
    vector<int> bfs_right = BFS(ri, rj+1, map);

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            outfile << bfs_up[TARGET];
            outfile << ((j==N-1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            outfile << bfs_down[TARGET];
            outfile << ((j==N-1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            outfile << bfs_left[TARGET];
            outfile << ((j==N-1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            outfile << bfs_right[TARGET];
            outfile << ((j==N-1) ? '\n' : ' ');
        }
    }

}

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

    infile >> M >> N >> B;
    map.reserve(M*N);

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            char ch;
            infile >> ch;
            map.push_back(ch);
            if(map[TARGET]=='R'){
                ri = i;
                rj = j;
            }
        }
    }
    
    cleaning_naive();

    return 0;
}