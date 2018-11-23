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
vector<pair<pair<int, int>, int>> dest; //_destination: storing shortest path from 'R'
enum Action{UP, DOWN, LEFT, RIGHT, ERROR};
vector<pair<int, int>> stk; //reversing the step when gothrough()
queue<pair<int, int>> ans;
vector<int> bfs_r, bfs_up, bfs_down, bfs_left, bfs_right;

vector<int> BFS(int i, int j, vector<char> tmap, bool flag)
{

    vector<int> bfs(M*N,B);
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
            if(flag) dest.push_back(make_pair(q.back(), cnt));
            bfs[T_UP] = cnt;
            tmap[T_UP] = 'D';
        }
        if(i<M-1) if(tmap[T_DOWN]=='0'){
            q.push(make_pair(i+1, j));
            if(flag) dest.push_back(make_pair(q.back(), cnt));
            bfs[T_DOWN] = cnt;
            tmap[T_DOWN] = 'D';
        }
        if(j>0) if(tmap[T_LEFT]=='0'){
            q.push(make_pair(i, j-1));
            if(flag) dest.push_back(make_pair(q.back(), cnt));
            bfs[T_LEFT] = cnt;
            tmap[T_LEFT] = 'D';
        }
        if(j<N-1) if(tmap[T_RIGHT]=='0'){
            q.push(make_pair(i, j+1));
            if(flag) dest.push_back(make_pair(q.back(), cnt));
            bfs[T_RIGHT] = cnt;
            tmap[T_RIGHT] = 'D';
        }

        q.pop();

    }
    return bfs;
}
void go_a_step(int& i, int& j, const vector<int>& bfs)
{
    if(i>0) if(map[T_UP]=='0') if(bfs[T_UP]==bfs[TARGET]-1){
        i--; return;
    }
    if(i<M-1) if(map[T_DOWN]=='0') if(bfs[T_DOWN]==bfs[TARGET]-1){
        i++; return;
    }
    if(j>0) if(map[T_LEFT]=='0') if(bfs[T_LEFT]==bfs[TARGET]-1){
        j--; return;
    }
    if(j<N-1) if(map[T_RIGHT]=='0') if(bfs[T_RIGHT]==bfs[TARGET]-1){
        j++; return;
    }
    if(i>0) if(bfs[T_UP]==bfs[TARGET]-1 || bfs[T_UP]==B){
        i--; return;
    }
    if(i<M-1) if(bfs[T_DOWN]==bfs[TARGET]-1 || bfs[T_DOWN]==B){
        i++; return;
    }
    if(j>0) if(bfs[T_LEFT]==bfs[TARGET]-1 || bfs[T_LEFT]==B){
        j--; return;
    }
    if(j<N-1) if(bfs[T_RIGHT]==bfs[TARGET]-1 || bfs[T_RIGHT]==B){
        j++; return;
    }
}
void gothrough(int i, int j, const Action action)
{
    if(action==ERROR){
        throw string("Error when going through path: There is no way for some element(s)\n");
        return;
    }

    //cout << action << ' ' << "dest:(" << i << ", " << j << ")\n";

    switch(action){
        //TODO: iterate through the destination and change the map value to label 'visited'
        case UP:
            while(bfs_up[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_up);
            }
            while(stk.empty()==false){
                ans.push(stk.back());
                stk.pop_back();
            }
            i = ans.back().first; j = ans.back().second;
            while(bfs_up[TARGET]<B){
                go_a_step(i, j, bfs_up);
                ans.push(make_pair(i, j));
                map[TARGET] = 'D';
            }
            break;
        case DOWN:
            while(bfs_down[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_down);
            }
            while(stk.empty()==false){
                ans.push(stk.back());
                stk.pop_back();
            }
            i = ans.back().first; j = ans.back().second;
            while(bfs_down[TARGET]<B){
                go_a_step(i, j, bfs_down);
                ans.push(make_pair(i, j));
                map[TARGET] = 'D';
            }
            break;
        case LEFT:
            while(bfs_left[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_left);
            }
            while(stk.empty()==false){
                ans.push(stk.back());
                stk.pop_back();
            }
            i = ans.back().first; j = ans.back().second;
            while(bfs_left[TARGET]<B){
                go_a_step(i, j, bfs_left);
                ans.push(make_pair(i, j));
                map[TARGET] = 'D';
            }
            break;
        case RIGHT:
            while(bfs_right[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_right);
            }
            while(stk.empty()==false){
                ans.push(stk.back());
                stk.pop_back();
            }
            i = ans.back().first; j = ans.back().second;
            while(bfs_right[TARGET]<B){
                go_a_step(i, j, bfs_right);
                ans.push(make_pair(i, j));
                map[TARGET] = 'D';
            }
            break;
    }

    
}

Action nextaction(const int i, const int j)
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
    
    return ((minstep<B) ? action : ERROR);
}

void cleaning_naive()
{

    bfs_r = BFS(ri, rj, map, 1);
    //vector<pair<pair<int, int>, int>> dest = _dest;
    //_dest.clear();

    bfs_up = BFS(ri-1, rj, map, 0);
    //vector<pair<pair<int, int>, int>> dest_up = _dest;
    //_dest.clear();

    bfs_down = BFS(ri+1, rj, map, 0);
    //vector<pair<pair<int, int>, int>> dest_down = _dest;
    //_dest.clear();

    bfs_left = BFS(ri, rj-1, map, 0);
    //vector<pair<pair<int, int>, int>> dest_left = _dest;
    //_dest.clear();

    bfs_right = BFS(ri, rj+1, map, 0);
    //vector<pair<pair<int, int>, int>> dest_right = _dest;
    //_dest.clear(); _dest.shrink_to_fit();

 /*/test BFS
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            outfile << bfs_r[TARGET];
            outfile << ((j==N-1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
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

        //cout << i << ' ' << j << ' ' << dest.back().second << '\n';
        
        if(map[TARGET]!='0'){
            dest.pop_back(); continue;
        }

        try{
            gothrough(i, j, nextaction(i, j));
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
    outfile << ans.size() << '\n';
    while(ans.empty()==false){
        outfile << ans.front().first << ' ' << ans.front().second << '\n';
        ans.pop();
    }

    return 0;
}