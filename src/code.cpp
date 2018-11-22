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
vector<pair<pair<int, int>, int>> _dest; //_destination: storing shortest path from 'R'
enum Action{UP, DOWN, LEFT, RIGHT, ERROR};
vector<pair<int, int>> stk; //reversing the step when gothrough()

vector<int> BFS(int i, int j, vector<char> tmap, bool flag)
{

    vector<int> bfs(M*N,-1);
    if(i<0 || j<0 || i>=M || j>=N) return bfs;

    queue<pair<int, int>> q;
    pair<int, int> ptr;
    
    q.push(make_pair(i, j));
    bfs[TARGET] = ((flag) ? 0 : 1);
    tmap[TARGET] = 'D';

    while(q.empty()==false){
        ptr = q.front();
        i = ptr.first;
        j = ptr.second;
        int cnt = bfs[TARGET] + 1;
        if((cnt>(B/2)) && flag==0) break;
        //cout << i << " " << j << ' ' << cnt << '\n';

        if(i>0) if(tmap[T_UP]=='0') {
            q.push(make_pair(i-1, j));
            _dest.push_back(make_pair(q.back(), cnt));
            bfs[T_UP] = cnt;
            tmap[T_UP] = 'D';
        }
        if(i<M-1) if(tmap[T_DOWN]=='0'){
            q.push(make_pair(i+1, j));
            _dest.push_back(make_pair(q.back(), cnt));
            bfs[T_DOWN] = cnt;
            tmap[T_DOWN] = 'D';
        }
        if(j>0) if(tmap[T_LEFT]=='0'){
            q.push(make_pair(i, j-1));
            _dest.push_back(make_pair(q.back(), cnt));
            bfs[T_LEFT] = cnt;
            tmap[T_LEFT] = 'D';
        }
        if(j<N-1) if(tmap[T_RIGHT]=='0'){
            q.push(make_pair(i, j+1));
            _dest.push_back(make_pair(q.back(), cnt));
            bfs[T_RIGHT] = cnt;
            tmap[T_RIGHT] = 'D';
        }

        q.pop();

    }
    return bfs;
}

void gothrough(const int i, const int j, const Action action)
{
    if(action==ERROR){
        throw string("Error when going through path: There is no way for some element(s)\n");
        return;
    }

    switch(action){
        //TODO: iterate through the destination and change the map value to label visited

    }
}

Action nextaction(const int i, const int j, const vector<int>& bfs_up, 
                    const vector<int>& bfs_down, const vector<int>& bfs_left, const vector<int>& bfs_right)
{
    Action action = UP;
    int minstep = bfs_up[TARGET];
    if(bfs_down[TARGET] < minstep){
        minstep = bfs_down[TARGET];
        action = DOWN;
    }
    if(bfs_left[TARGET] < minstep){
        minstep = bfs_left[TARGET];
        action = LEFT;
    }
    if(bfs_right[TARGET] < minstep){
        minstep = bfs_right[TARGET];
        action = RIGHT;
    }
    
    return ((minstep>0) ? action : ERROR);
}

void cleaning_naive()
{

    vector<int> bfs = BFS(ri, rj, map, 1);
    vector<pair<pair<int, int>, int>> dest = _dest;
    //_dest.clear();

    vector<int> bfs_up = BFS(ri-1, rj, map, 0);
    //vector<pair<pair<int, int>, int>> dest_up = _dest;
    //_dest.clear();

    vector<int> bfs_down = BFS(ri+1, rj, map, 0);
    //vector<pair<pair<int, int>, int>> dest_down = _dest;
    //_dest.clear();

    vector<int> bfs_left = BFS(ri, rj-1, map, 0);
    //vector<pair<pair<int, int>, int>> dest_left = _dest;
    //_dest.clear();

    vector<int> bfs_right = BFS(ri, rj+1, map, 0);
    //vector<pair<pair<int, int>, int>> dest_right = _dest;
    _dest.clear(); _dest.shrink_to_fit();

/* test BFS
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
*/
    int curB = B;
    while(dest.empty()==false){
        int i = dest.back().first.first;
        int j = dest.back().first.second;
        
        if(map[TARGET]!='0'){
            dest.pop_back(); continue;
        }

        try{
            gothrough(i, j, nextaction(i, j, bfs_up, bfs_down, bfs_left, bfs_right));
        } catch(string c){cout << c;}

        dest.pop_back();
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