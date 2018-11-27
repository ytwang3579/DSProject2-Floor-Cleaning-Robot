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
vector<char> map, _map;
vector<pair<pair<int, int>, int>> dest; //_destination: storing shortest path from 'R'
enum Action{UP, DOWN, LEFT, RIGHT, ERROR};
vector<pair<int, int>> stk; //reversing the step when gothrough()
queue<pair<int, int>> ans;
vector<int> bfs_r, bfs_up, bfs_down, bfs_left, bfs_right;
long long anscnt;
bool print;
stringstream dbg;

vector<int> BFS(int i, int j, vector<char> tmap, bool flag)
{

    vector<int> bfs(M*N,B+1);
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
        if((cnt>B) && flag==0) break;
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
    i = ri; j = rj;
    bfs[TARGET] = B;
    return bfs;
}
void go_a_step(int& i, int& j, const vector<int>& bfs, const Action action)
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
    if(i>0) if(bfs[T_UP]==bfs[TARGET]-1 || ((action==DOWN)&&bfs[T_UP]==B)){
        i--; return;
    }
    if(i<M-1) if(bfs[T_DOWN]==bfs[TARGET]-1 || ((action==UP)&&bfs[T_DOWN]==B)){
        i++; return;
    }
    if(j>0) if(bfs[T_LEFT]==bfs[TARGET]-1 || ((action==RIGHT)&&bfs[T_LEFT]==B)){
        j--; return;
    }
    if(j<N-1) if(bfs[T_RIGHT]==bfs[TARGET]-1 || ((action==LEFT)&&bfs[T_RIGHT]==B)){
        j++; return;
    }
    cout << dbg.str() << " debug\n";
}
Action goback(int i, int j, const Action action, const Action last, int curB)
{
    switch(action){
        case UP:
            if(bfs_up[TARGET]<=curB){
                while(bfs_up[TARGET]<B){
                    go_a_step(i, j, bfs_up, action);

                    //ans.push(make_pair(i, j));
                    if(print) outfile << i << ' ' << j << '\n';
                    else anscnt++;
                    map[TARGET] = 'D';
                }
                return action;
            } else if(last==DOWN){
                if(bfs_left[TARGET]<=curB){
                    while(bfs_left[TARGET]<B){
                        go_a_step(i, j, bfs_left, LEFT);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return LEFT;
                } else if(bfs_right[TARGET]<=curB){
                    while(bfs_right[TARGET]<B){
                        go_a_step(i, j, bfs_right, RIGHT);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return RIGHT;
                }
            }
            break;
        case DOWN:
            if(bfs_down[TARGET]<=curB){
                while(bfs_down[TARGET]<B){
                    go_a_step(i, j, bfs_down, action);
                    //ans.push(make_pair(i, j));
                    if(print) outfile << i << ' ' << j << '\n';
                    else anscnt++;
                    map[TARGET] = 'D';
                }
                return action;
            } else if(last==UP){
                if(bfs_left[TARGET]<=curB){
                    while(bfs_left[TARGET]<B){
                        go_a_step(i, j, bfs_left, LEFT);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return LEFT;
                } else if(bfs_right[TARGET]<=curB){
                    while(bfs_right[TARGET]<B){
                        go_a_step(i, j, bfs_right, RIGHT);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return RIGHT;
                }
            }
            break;
        case LEFT:
            if(bfs_left[TARGET]<=curB){
                while(bfs_left[TARGET]<B){
                    go_a_step(i, j, bfs_left, action);
                    //ans.push(make_pair(i, j));
                    if(print) outfile << i << ' ' << j << '\n';
                    else anscnt++;
                    map[TARGET] = 'D';
                }
                return action;
            } else if(last==RIGHT){
                if(bfs_up[TARGET]<=curB){
                    while(bfs_up[TARGET]<B){
                        go_a_step(i, j, bfs_up, UP);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return UP;
                } else if(bfs_down[TARGET]<=curB){
                    while(bfs_down[TARGET]<B){
                        go_a_step(i, j, bfs_down, DOWN);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return DOWN;
                }
            }
            break;
        case RIGHT:
            if(bfs_right[TARGET]<=curB){
                while(bfs_right[TARGET]<B){
                    go_a_step(i, j, bfs_right, action);
                    //ans.push(make_pair(i, j));
                    if(print) outfile << i << ' ' << j << '\n';
                    else anscnt++;
                    map[TARGET] = 'D';
                }
                return action;
            } else if(last==RIGHT){
                if(bfs_up[TARGET]<=curB){
                    while(bfs_up[TARGET]<B){
                        go_a_step(i, j, bfs_up, UP);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return UP;
                } else if(bfs_down[TARGET]<=curB){
                    while(bfs_down[TARGET]<B){
                        go_a_step(i, j, bfs_down, DOWN);
                        //ans.push(make_pair(i, j));
                        if(print) outfile << i << ' ' << j << '\n';
                        else anscnt++;
                        map[TARGET] = 'D';
                    }
                    return DOWN;
                }
            }
            break;
    }
    switch(last){
        case UP:
            while(bfs_up[TARGET]<B){
                go_a_step(i, j, bfs_up, last);
                //ans.push(make_pair(i, j));
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                map[TARGET] = 'D';
            }
            break;
        case DOWN:
            while(bfs_down[TARGET]<B){
                go_a_step(i, j, bfs_down, last);
                //ans.push(make_pair(i, j));
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                map[TARGET] = 'D';
            }
            break;
        case LEFT:
            while(bfs_left[TARGET]<B){
                go_a_step(i, j, bfs_left, last);
                //ans.push(make_pair(i, j));
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                map[TARGET] = 'D';
            }
            break;
        case RIGHT:
            while(bfs_right[TARGET]<B){
                go_a_step(i, j, bfs_right, last);
                //ans.push(make_pair(i, j));
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                map[TARGET] = 'D';
            }
            break;
    }
    return last;
}
Action gothrough(int i, int j, const Action action, const Action next)
{
    if(action==ERROR){
        throw string("Error when going through path: There is no way for some element(s)\n");
        return ERROR;
    }

    //cout << action << ' ' << "dest:(" << i << ", " << j << ")\n";
    int curB = B;
    int initi = i;
    int initj = j;

    switch(action){
        //TODO: check battery life for better path(s)
        case UP:
            while(bfs_up[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_up, action);
            }
            while(stk.empty()==false){
                //ans.push(stk.back());
                i = stk.back().first; j = stk.back().second;
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                stk.pop_back();
                curB--;
            }
            //i = ans.back().first; j = ans.back().second;
            i = initi; j = initj;
            return goback(i, j, next, action, curB);
            break;
        case DOWN:
            while(bfs_down[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_down, action);
            }
            while(stk.empty()==false){
                //ans.push(stk.back());
                i = stk.back().first; j = stk.back().second;
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                stk.pop_back();
                curB--;
            }
            //i = ans.back().first; j = ans.back().second;
            i = initi; j = initj;
            return goback(i, j, next, action, curB);
            break;
        case LEFT:
            while(bfs_left[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_left, action);
            }
            while(stk.empty()==false){
                //ans.push(stk.back());
                i = stk.back().first; j = stk.back().second;
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                stk.pop_back();
                curB--;
            }
            //i = ans.back().first; j = ans.back().second;
            i = initi; j = initj;
            return goback(i, j, next, action, curB);
            break;
        case RIGHT:
            while(bfs_right[TARGET]<B){
                map[TARGET] = 'D';
                stk.push_back(make_pair(i, j));
                go_a_step(i, j, bfs_right, action);
            }
            while(stk.empty()==false){
                //ans.push(stk.back());
                i = stk.back().first; j = stk.back().second;
                if(print) outfile << i << ' ' << j << '\n';
                else anscnt++;
                stk.pop_back();
                curB--;
            }
            //i = ans.back().first; j = ans.back().second;
            i = initi; j = initj;
            return goback(i, j, next, action, curB);
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
    
    return ((minstep<=(B/2)) ? action : ERROR);
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
    if(dest.empty()==true) return;
    int curi, curj, nexti, nextj;
    Action curDir, nextDir;
    bool lasttimeflag = true;

    curi = dest.back().first.first;
    curj = dest.back().first.second;
    curDir = nextaction(curi, curj);
    dest.pop_back();

    while(dest.empty()==false){
        int i = dest.back().first.first;
        int j = dest.back().first.second;

        //cout << i << ' ' << j << ' ' << dest.back().second << '\n';
        
        if(map[TARGET]!='0'){
            dest.pop_back(); continue;
        }

        nexti = i;
        nextj = j;
        nextDir = nextaction(nexti, nextj);

        try{
            //cout << curi << ' ' << curj << ' ' << curDir << ' ' << nextDir << '\n';
            nextDir = gothrough(curi, curj, curDir, nextDir);
        } catch(string c){cout << c;}

        while(map[TARGET]!='0'){
            //cout << i << ' ' << j << ' ' << map[TARGET] << '\n';
            dest.pop_back();
            if(dest.empty()==true){
                lasttimeflag = false;
                break;
            }
            i = dest.back().first.first; j = dest.back().first.second;
        }
        

        curi = i;
        curj = j;
        curDir = nextaction(i, j);
        if(dest.empty()==false) dest.pop_back();
        else break;

        //cout << i << ' ' << j << ' ' << map[TARGET] << '\n';
        //cout << "**\n";
        //cout << curDir << "<-" << nextDir << '\n';
        if(curDir!=nextDir){
            if(curDir==UP) curDir = gothrough(ri-1, rj, nextDir, curDir);
            else if(curDir==DOWN) curDir = gothrough(ri+1, rj, nextDir, curDir);
            else if(curDir==LEFT) curDir = gothrough(ri, rj-1, nextDir, curDir);
            else if(curDir==RIGHT) curDir = gothrough(ri, rj+1, nextDir, curDir);
        }
        
    }
    
    try{
        if(lasttimeflag){
            nextDir = nextaction(curi, curj);
            gothrough(curi, curj, curDir, curDir);
        }
    } catch(string c){cout << c;}
    
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
    dbg << argv[1];

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
    map.shrink_to_fit();
    _map = map;
    
    cleaning_naive();
    outfile << anscnt << '\n';

    map = _map;
    print = true;
    cleaning_naive();
    /*
    outfile << ans.size() << '\n';
    while(ans.empty()==false){
        outfile << ans.front().first << ' ' << ans.front().second << '\n';
        ans.pop();
    }
    */
    return 0;
}