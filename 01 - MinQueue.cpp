#include <bits/stdc++.h>
using namespace std;
  
#define int uint32_t

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
using ii = pair<int,int>;
using iii = tuple<int,int,int>;
using ll = long long;
const int INF = 0x3f3f3f3f3f3f3f3f;

const int A = 1664525;
const int C = 1013904223;

int GCL(int last){
    return ((A * last) + C);
}

struct MinQueue{
    queue<int> q;
    deque<ii> mins;
    int cnt = 0;
    int erased = 0;

    void push(int x){
        q.push(x);
        while(!mins.empty() && mins.back().first > x){
            mins.pop_back();
        }
        mins.push_back({x, cnt++});
    }

    void pop(){
        if(q.empty()) return;
        int x = q.front();
        q.pop();
        erased++;
        if(mins[0].first == x) mins.pop_front();
    }

    int min(){
        return mins[0].second - erased;
    }
};

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    int seed, burn, n, p;
    cin >> seed >> burn >> n >> p;

    MinQueue q;

    int last = seed;
    for(int i = 0; i < burn; i++){
        q.push(last);
        last = GCL(last);
    }

    for(int i = 0; i < n; i++){
        if(last % 10 < p){
            last = GCL(last);
            q.push(last);
        }
        else{
            q.pop();
        }
        last = GCL(last);
        cout << q.mins.size() << " " << q.min() << '\n';
    }
    return 0;
}