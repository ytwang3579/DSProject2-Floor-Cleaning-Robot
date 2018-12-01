#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;
ifstream infile;
ofstream outfile;

#define TARGET (i * N + j)
#define T_UP ((i - 1) * N + j)
#define T_DOWN ((i + 1) * N + j)
#define T_LEFT (i * N + j - 1)
#define T_RIGHT (i * N + j + 1)

//global variable
int M, N, B, ri, rj;
vector<char> map, _map;
vector<pair<pair<int, int>, int>> dest; //_destination: storing shortest path from 'R'
enum Action
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ERROR
};
vector<pair<int, int>> stk; //reversing the step when gothrough()
vector<int> bfs_r, bfs_up, bfs_down, bfs_left, bfs_right;
long long anscnt;
bool print;

vector<int> BFS(int i, int j, vector<char> tmap, bool flag)
{

    vector<int> bfs(M * N, B + 1);
    if (i < 0 || j < 0 || i >= M || j >= N || map[TARGET] == '1')
        return bfs;

    queue<pair<int, int>> q;
    pair<int, int> ptr;

    q.push(make_pair(i, j));
    bfs[TARGET] = ((flag) ? 0 : 1);
    tmap[TARGET] = 'D';

    while (q.empty() == false)
    {
        ptr = q.front();
        i = ptr.first;
        j = ptr.second;
        int cnt = bfs[TARGET] + 1;
        if ((cnt > B) && flag == 0)
            break;

        if (i > 0)
            if (tmap[T_UP] == '0')
            {
                q.push(make_pair(i - 1, j));
                if (flag)
                    dest.push_back(make_pair(q.back(), cnt));
                bfs[T_UP] = cnt;
                tmap[T_UP] = 'D';
            }
        if (i < M - 1)
            if (tmap[T_DOWN] == '0')
            {
                q.push(make_pair(i + 1, j));
                if (flag)
                    dest.push_back(make_pair(q.back(), cnt));
                bfs[T_DOWN] = cnt;
                tmap[T_DOWN] = 'D';
            }
        if (j > 0)
            if (tmap[T_LEFT] == '0')
            {
                q.push(make_pair(i, j - 1));
                if (flag)
                    dest.push_back(make_pair(q.back(), cnt));
                bfs[T_LEFT] = cnt;
                tmap[T_LEFT] = 'D';
            }
        if (j < N - 1)
            if (tmap[T_RIGHT] == '0')
            {
                q.push(make_pair(i, j + 1));
                if (flag)
                    dest.push_back(make_pair(q.back(), cnt));
                bfs[T_RIGHT] = cnt;
                tmap[T_RIGHT] = 'D';
            }

        q.pop();
    }
    i = ri;
    j = rj;
    bfs[TARGET] = B;
    return bfs;
}
void go_a_step(int &i, int &j, const vector<int> &bfs, const Action action)
{
    if (i > 0)
        if (map[T_UP] == '0')
            if (bfs[T_UP] == bfs[TARGET] - 1)
            {
                i--;
                return;
            }
    if (i < M - 1)
        if (map[T_DOWN] == '0')
            if (bfs[T_DOWN] == bfs[TARGET] - 1)
            {
                i++;
                return;
            }
    if (j > 0)
        if (map[T_LEFT] == '0')
            if (bfs[T_LEFT] == bfs[TARGET] - 1)
            {
                j--;
                return;
            }
    if (j < N - 1)
        if (map[T_RIGHT] == '0')
            if (bfs[T_RIGHT] == bfs[TARGET] - 1)
            {
                j++;
                return;
            }
    if (i > 0)
        if (bfs[T_UP] == bfs[TARGET] - 1 || ((action == DOWN) && bfs[T_UP] == B))
        {
            i--;
            return;
        }
    if (i < M - 1)
        if (bfs[T_DOWN] == bfs[TARGET] - 1 || ((action == UP) && bfs[T_DOWN] == B))
        {
            i++;
            return;
        }
    if (j > 0)
        if (bfs[T_LEFT] == bfs[TARGET] - 1 || ((action == RIGHT) && bfs[T_LEFT] == B))
        {
            j--;
            return;
        }
    if (j < N - 1)
        if (bfs[T_RIGHT] == bfs[TARGET] - 1 || ((action == LEFT) && bfs[T_RIGHT] == B))
        {
            j++;
            return;
        }
}
Action goback(int i, int j, const Action action, const Action last, int curB)
{
    switch (action)
    {
    case UP:
        if (bfs_up[TARGET] <= curB)
        {
            while (bfs_up[TARGET] < B)
            {
                go_a_step(i, j, bfs_up, action);
                if (print)
                    outfile << i << ' ' << j << '\n';
                else
                    anscnt++;
                map[TARGET] = 'D';
            }
            return action;
        }
        else if (last == DOWN)
        {
            if (bfs_left[TARGET] <= curB)
            {
                while (bfs_left[TARGET] < B)
                {
                    go_a_step(i, j, bfs_left, LEFT);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return LEFT;
            }
            else if (bfs_right[TARGET] <= curB)
            {
                while (bfs_right[TARGET] < B)
                {
                    go_a_step(i, j, bfs_right, RIGHT);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return RIGHT;
            }
        }
        break;
    case DOWN:
        if (bfs_down[TARGET] <= curB)
        {
            while (bfs_down[TARGET] < B)
            {
                go_a_step(i, j, bfs_down, action);
                if (print)
                    outfile << i << ' ' << j << '\n';
                else
                    anscnt++;
                map[TARGET] = 'D';
            }
            return action;
        }
        else if (last == UP)
        {
            if (bfs_left[TARGET] <= curB)
            {
                while (bfs_left[TARGET] < B)
                {
                    go_a_step(i, j, bfs_left, LEFT);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return LEFT;
            }
            else if (bfs_right[TARGET] <= curB)
            {
                while (bfs_right[TARGET] < B)
                {
                    go_a_step(i, j, bfs_right, RIGHT);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return RIGHT;
            }
        }
        break;
    case LEFT:
        if (bfs_left[TARGET] <= curB)
        {
            while (bfs_left[TARGET] < B)
            {
                go_a_step(i, j, bfs_left, action);
                if (print)
                    outfile << i << ' ' << j << '\n';
                else
                    anscnt++;
                map[TARGET] = 'D';
            }
            return action;
        }
        else if (last == RIGHT)
        {
            if (bfs_up[TARGET] <= curB)
            {
                while (bfs_up[TARGET] < B)
                {
                    go_a_step(i, j, bfs_up, UP);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return UP;
            }
            else if (bfs_down[TARGET] <= curB)
            {
                while (bfs_down[TARGET] < B)
                {
                    go_a_step(i, j, bfs_down, DOWN);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return DOWN;
            }
        }
        break;
    case RIGHT:
        if (bfs_right[TARGET] <= curB)
        {
            while (bfs_right[TARGET] < B)
            {
                go_a_step(i, j, bfs_right, action);
                if (print)
                    outfile << i << ' ' << j << '\n';
                else
                    anscnt++;
                map[TARGET] = 'D';
            }
            return action;
        }
        else if (last == RIGHT)
        {
            if (bfs_up[TARGET] <= curB)
            {
                while (bfs_up[TARGET] < B)
                {
                    go_a_step(i, j, bfs_up, UP);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return UP;
            }
            else if (bfs_down[TARGET] <= curB)
            {
                while (bfs_down[TARGET] < B)
                {
                    go_a_step(i, j, bfs_down, DOWN);
                    if (print)
                        outfile << i << ' ' << j << '\n';
                    else
                        anscnt++;
                    map[TARGET] = 'D';
                }
                return DOWN;
            }
        }
        break;
    }
    switch (last)
    {
    case UP:
        while (bfs_up[TARGET] < B)
        {
            go_a_step(i, j, bfs_up, last);
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            map[TARGET] = 'D';
        }
        break;
    case DOWN:
        while (bfs_down[TARGET] < B)
        {
            go_a_step(i, j, bfs_down, last);
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            map[TARGET] = 'D';
        }
        break;
    case LEFT:
        while (bfs_left[TARGET] < B)
        {
            go_a_step(i, j, bfs_left, last);
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            map[TARGET] = 'D';
        }
        break;
    case RIGHT:
        while (bfs_right[TARGET] < B)
        {
            go_a_step(i, j, bfs_right, last);
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            map[TARGET] = 'D';
        }
        break;
    }
    return last;
}
Action gothrough(int i, int j, const Action action, const Action next)
{
    if (action == ERROR)
    {
        throw string("Error when going through path: There is no way for some element(s)\n");
        return ERROR;
    }

    int curB = B;
    int initi = i;
    int initj = j;

    switch (action)
    {
    case UP:
        while (bfs_up[TARGET] < B)
        {
            map[TARGET] = 'D';
            stk.push_back(make_pair(i, j));
            go_a_step(i, j, bfs_up, action);
        }
        while (stk.empty() == false)
        {
            i = stk.back().first;
            j = stk.back().second;
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            stk.pop_back();
            curB--;
        }
        i = initi;
        j = initj;
        return goback(i, j, next, action, curB);
        break;
    case DOWN:
        while (bfs_down[TARGET] < B)
        {
            map[TARGET] = 'D';
            stk.push_back(make_pair(i, j));
            go_a_step(i, j, bfs_down, action);
        }
        while (stk.empty() == false)
        {
            i = stk.back().first;
            j = stk.back().second;
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            stk.pop_back();
            curB--;
        }
        i = initi;
        j = initj;
        return goback(i, j, next, action, curB);
        break;
    case LEFT:
        while (bfs_left[TARGET] < B)
        {
            map[TARGET] = 'D';
            stk.push_back(make_pair(i, j));
            go_a_step(i, j, bfs_left, action);
        }
        while (stk.empty() == false)
        {
            i = stk.back().first;
            j = stk.back().second;
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            stk.pop_back();
            curB--;
        }
        i = initi;
        j = initj;
        return goback(i, j, next, action, curB);
        break;
    case RIGHT:
        while (bfs_right[TARGET] < B)
        {
            map[TARGET] = 'D';
            stk.push_back(make_pair(i, j));
            go_a_step(i, j, bfs_right, action);
        }
        while (stk.empty() == false)
        {
            i = stk.back().first;
            j = stk.back().second;
            if (print)
                outfile << i << ' ' << j << '\n';
            else
                anscnt++;
            stk.pop_back();
            curB--;
        }
        i = initi;
        j = initj;
        return goback(i, j, next, action, curB);
        break;
    }
}

Action nextaction(const int i, const int j)
{
    Action action = UP;
    int minstep = bfs_up[TARGET];
    if (bfs_down[TARGET] < minstep)
    {
        minstep = bfs_down[TARGET];
        action = DOWN;
    }
    if (bfs_left[TARGET] < minstep)
    {
        minstep = bfs_left[TARGET];
        action = LEFT;
    }
    if (bfs_right[TARGET] < minstep)
    {
        minstep = bfs_right[TARGET];
        action = RIGHT;
    }

    return ((minstep <= (B / 2)) ? action : ERROR);
}

void forwarding(Action from, Action to)
{
    int i = ri, j = rj;
    switch (to)
    {
    case UP:
        if (from == DOWN)
        {
            if (bfs_down[T_UP] < B)
                gothrough(i - 1, j, from, to);
            else if (bfs_left[T_UP] < B && bfs_down[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i - 1, j, LEFT, to);
            }
            else if (bfs_right[T_UP] < B && bfs_down[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i - 1, j, RIGHT, to);
            }
        }
        else if (from == LEFT)
        {
            if (bfs_left[T_UP] < B)
                gothrough(i - 1, j, from, to);
            else if (bfs_right[T_UP] < B && bfs_left[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i - 1, j, RIGHT, to);
            }
            else if (bfs_down[T_UP] < B && bfs_left[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i - 1, j, DOWN, to);
            }
            else if (bfs_right[T_UP] < B && bfs_down[T_RIGHT] < B && bfs_left[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i, j + 1, DOWN, RIGHT);
                gothrough(i - 1, j, RIGHT, to);
            }
        }
        else if (from == RIGHT)
        {
            if (bfs_right[T_UP] < B)
                gothrough(i - 1, j, from, to);
            else if (bfs_left[T_UP] < B && bfs_right[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i - 1, j, LEFT, to);
            }
            else if (bfs_down[T_UP] < B && bfs_right[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i - 1, j, DOWN, to);
            }
            else if (bfs_left[T_UP] < B && bfs_down[T_LEFT] < B && bfs_right[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i, j - 1, DOWN, LEFT);
                gothrough(i - 1, j, LEFT, to);
            }
        }
        break;
    case DOWN:
        if (from == UP)
        {
            if (bfs_up[T_DOWN] < B)
                gothrough(i + 1, j, from, to);
            else if (bfs_left[T_DOWN] < B && bfs_up[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i + 1, j, LEFT, to);
            }
            else if (bfs_right[T_DOWN] < B && bfs_up[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i + 1, j, RIGHT, to);
            }
        }
        else if (from == LEFT)
        {
            if (bfs_left[T_DOWN] < B)
                gothrough(i + 1, j, from, to);
            else if (bfs_right[T_DOWN] < B && bfs_left[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i + 1, j, RIGHT, to);
            }
            else if (bfs_down[T_DOWN] < B && bfs_left[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i + 1, j, UP, to);
            }
            else if (bfs_right[T_DOWN] < B && bfs_up[T_RIGHT] < B && bfs_left[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i, j + 1, UP, RIGHT);
                gothrough(i + 1, j, RIGHT, to);
            }
        }
        else if (from == RIGHT)
        {
            if (bfs_right[T_DOWN] < B)
                gothrough(i + 1, j, from, to);
            else if (bfs_left[T_DOWN] < B && bfs_right[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i + 1, j, LEFT, to);
            }
            else if (bfs_up[T_DOWN] < B && bfs_right[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i + 1, j, UP, to);
            }
            else if (bfs_left[T_DOWN] < B && bfs_up[T_LEFT] < B && bfs_right[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i, j - 1, UP, LEFT);
                gothrough(i + 1, j, LEFT, to);
            }
        }
        break;
    case LEFT:
        if (from == RIGHT)
        {
            if (bfs_right[T_LEFT] < B)
                gothrough(i, j - 1, from, to);
            else if (bfs_up[T_LEFT] < B && bfs_right[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i, j - 1, UP, to);
            }
            else if (bfs_down[T_LEFT] < B && bfs_right[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i, j - 1, DOWN, to);
            }
        }
        else if (from == UP)
        {
            if (bfs_up[T_LEFT] < B)
                gothrough(i, j - 1, from, to);
            else if (bfs_down[T_LEFT] < B && bfs_up[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i, j - 1, DOWN, to);
            }
            else if (bfs_right[T_LEFT] < B && bfs_up[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i, j - 1, RIGHT, to);
            }
            else if (bfs_down[T_LEFT] < B && bfs_right[T_DOWN] < B && bfs_up[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i + 1, j, RIGHT, DOWN);
                gothrough(i, j - 1, DOWN, to);
            }
        }
        else if (from == DOWN)
        {
            if (bfs_down[T_LEFT] < B)
                gothrough(i, j - 1, from, to);
            else if (bfs_up[T_LEFT] < B && bfs_down[T_UP] < B)
            {
                gothrough(i + 1, j, from, UP);
                gothrough(i, j - 1, UP, to);
            }
            else if (bfs_right[T_LEFT] < B && bfs_down[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i, j - 1, RIGHT, to);
            }
            else if (bfs_up[T_LEFT] < B && bfs_right[T_UP] < B && bfs_down[T_RIGHT] < B)
            {
                gothrough(i, j + 1, from, RIGHT);
                gothrough(i - 1, j, RIGHT, UP);
                gothrough(i, j - 1, UP, to);
            }
        }
        break;
    case RIGHT:
        if (from == LEFT)
        {
            if (bfs_left[T_RIGHT] < B)
                gothrough(i, j + 1, from, to);
            else if (bfs_up[T_RIGHT] < B && bfs_left[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i, j + 1, UP, to);
            }
            else if (bfs_down[T_RIGHT] < B && bfs_left[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i, j + 1, DOWN, to);
            }
        }
        else if (from == UP)
        {
            if (bfs_up[T_RIGHT] < B)
                gothrough(i, j + 1, from, to);
            else if (bfs_down[T_RIGHT] < B && bfs_up[T_DOWN] < B)
            {
                gothrough(i + 1, j, from, DOWN);
                gothrough(i, j + 1, DOWN, to);
            }
            else if (bfs_left[T_RIGHT] < B && bfs_up[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i, j + 1, LEFT, to);
            }
            else if (bfs_down[T_RIGHT] < B && bfs_left[T_DOWN] < B && bfs_up[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i + 1, j, LEFT, DOWN);
                gothrough(i, j + 1, DOWN, to);
            }
        }
        else if (from == DOWN)
        {
            if (bfs_down[T_RIGHT] < B)
                gothrough(i, j + 1, from, to);
            else if (bfs_up[T_RIGHT] < B && bfs_down[T_UP] < B)
            {
                gothrough(i - 1, j, from, UP);
                gothrough(i, j + 1, UP, to);
            }
            else if (bfs_left[T_RIGHT] < B && bfs_down[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i, j + 1, LEFT, to);
            }
            else if (bfs_up[T_RIGHT] < B && bfs_left[T_UP] < B && bfs_down[T_LEFT] < B)
            {
                gothrough(i, j - 1, from, LEFT);
                gothrough(i - 1, j, LEFT, UP);
                gothrough(i, j + 1, UP, to);
            }
        }
        break;
    }
}

void cleaning_naive()
{

    bfs_r = BFS(ri, rj, map, 1);

    bfs_up = BFS(ri - 1, rj, map, 0);

    bfs_down = BFS(ri + 1, rj, map, 0);

    bfs_left = BFS(ri, rj - 1, map, 0);

    bfs_right = BFS(ri, rj + 1, map, 0);
/* TEST BFS 
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << bfs_r[TARGET];
            outfile << ((j == N - 1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << bfs_up[TARGET];
            outfile << ((j == N - 1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << bfs_down[TARGET];
            outfile << ((j == N - 1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << bfs_left[TARGET];
            outfile << ((j == N - 1) ? '\n' : ' ');
        }
    }
    outfile << '\n';
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            outfile << bfs_right[TARGET];
            outfile << ((j == N - 1) ? '\n' : ' ');
        }
    }
*/
    if (dest.empty() == true)
        return;
    int curi, curj, nexti, nextj;
    Action curDir, nextDir;
    bool lasttimeflag = true;

    curi = dest.back().first.first;
    curj = dest.back().first.second;
    curDir = nextaction(curi, curj);
    dest.pop_back();

    while (dest.empty() == false)
    {
        int i = dest.back().first.first;
        int j = dest.back().first.second;

        if (map[TARGET] != '0')
        {
            dest.pop_back();
            continue;
        }

        nexti = i;
        nextj = j;
        nextDir = nextaction(nexti, nextj);

        try
        {
            nextDir = gothrough(curi, curj, curDir, nextDir);
        }
        catch (string c)
        {
            cout << c;
        }

        do
        {
            while (map[TARGET] != '0')
            {
                dest.pop_back();
                if (dest.empty() == true)
                {
                    lasttimeflag = false;
                    break;
                }
                i = dest.back().first.first;
                j = dest.back().first.second;
            }

            if (dest.empty() == true)
                break;
            curi = i;
            curj = j;
            curDir = nextaction(i, j);

            if (curDir != nextDir)
            {
                forwarding(nextDir, curDir);
                nextDir = curDir;
            }
        } while (map[TARGET] != '0');

        if (dest.empty() == false)
            dest.pop_back();
        else
            break;
    }

    try
    {
        if (lasttimeflag)
        {
            nextDir = nextaction(curi, curj);
            gothrough(curi, curj, curDir, curDir);
        }
    }
    catch (string c)
    {
        cout << c;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Input Error!!\n";
        return 1;
    }
    stringstream strin, strout;
    strin << "./" << argv[1] << "/floor.data";
    strout << "./" << argv[1] << "/final.path";

    infile.open(strin.str());
    outfile.open(strout.str());

    if (!infile)
    {
        cout << "Cannot open file!!\n";
        return 1;
    }
    if (!outfile)
    {
        cout << "Cannot write file!!\n";
        return 1;
    }

    infile >> M >> N >> B;
    map.reserve(M * N);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            char ch;
            infile >> ch;
            map.push_back(ch);
            if (map[TARGET] == 'R')
            {
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

    return 0;
}