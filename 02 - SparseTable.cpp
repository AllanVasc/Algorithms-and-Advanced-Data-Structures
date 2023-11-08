#include <bits/stdc++.h>
using namespace std;
  
#define int long long

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
using ii = pair<int,int>;
using iii = tuple<int,int,int>;
using ll = long long;
const int INF = 0x3f3f3f3f3f3f3f3f;
const int MOD = 1e9 + 7;

struct RNG{
    uint32_t number, seed, A, C;
    RNG(uint32_t seed, uint32_t A, uint32_t C) : seed(seed), A(A), C(C) {
        number = seed;
    }
    uint32_t next(){
        int last = number;
        number = A * number + C;
        return last;
    }
};

struct SparseTable{
    int n, LOG;
    vector<vector<int>> st;
    vector<int> lg2;
    int type;
    SparseTable(vector<int>& v, int type) : type(type) {
        n = v.size();
        LOG = 25;
        st.assign(LOG, vector<int>(n, 0));
        lg2.assign(n + 1, 0);

        for(int i = 0; i < n; i++) st[0][i] = v[i];
        for(int i = 2; i <= n; i++) lg2[i] = lg2[i/2] + 1;

        for(int i = 1; i < LOG; i++){
            for(int j = 0; j + (1 << i) - 1 < n; j++){
                st[i][j] = func(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    int func(int x, int y){
        if(type == 0) return x + y;
        else if(type == 1) return min(x, y);
        else return max(x, y);
    }

    int query(int l, int r){
        if(type == 0){
            int answ = 0;
            for(int i = LOG - 1; i >= 0; i--){
                if((1 << i) <= r - l + 1){
                    answ += st[i][l];
                    l += (1 << i);
                }
            }
            return answ;
        }
        else{
            int i = lg2[r - l + 1];
            return func(st[i][l], st[i][r - (1 << i) + 1]);
        }
    }

    void update(int idx, int val){
        st[0][idx] = val;
        int k = 1;
        //cout << "idx = " << idx <<  '\n';
        for(int i = 1; i < LOG; i++){
            //cout << "i = " << i << "\n";
            for(int j = max(0LL, idx - 2*k + 1); (j <= idx) && (j + (1 << i) - 1 < n); j++){
                //cout << "[" << j << ":" << (j + (1 << (i - 1)) - 1) << "]" << " U ";
                //cout << "[" << j + k << ":" << (j + k + (1 << (i - 1)) - 1) << "]" << '\n';
                st[i][j] = func(st[i - 1][j], st[i - 1][j + k]);
            }
            k *= 2;
        }
    }
};

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    int s, n, o, q, u;
    string f;
    int cs = 0;
    while(cin >> s >> n >> f >> o >> q >> u){
        cout << "caso " << cs << "\n";

        RNG gen(s, 1664525, 1013904223);
        vector<int> v(n);
        int m = 4 * n;

        for(int i = 0; i < n; i++){
            v[i] = gen.next() % m;
        }

        int type = 0;
        if(f == "SUM") type = 0;
        else if(f == "MIN") type = 1;
        else type = 2;

        SparseTable st(v, type);
        while(o--){
            int x = gen.next();
            if((x % (q + u)) < q){    // Query
                int left = gen.next() % n;
                int right = left + (gen.next() % (n - left));
                cout << st.query(left, right) << '\n';
            }
            else{   // update
                int idx = gen.next() % n;
                int val = gen.next() % m;
                v[idx] = val;
                st.update(idx, val);
                cout << st.query(idx, n - 1) << '\n';
            }
        }
        cout << '\n';
        cs++;
    }
    return 0;
}