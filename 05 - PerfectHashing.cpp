#include <bits/stdc++.h>
using namespace std;
  
#define int uint32_t

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
using ii = pair<int,int>;
using iii = tuple<int,int,int>;
using ll = long long;
const int INF = 0x3f3f3f3f3f3f3f3f;
const int MOD = 1e9 + 7;

const int prime = (1LL << 31) - 1;

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

RNG gen(1,1,1);

struct HashFunc{
    uint32_t a, b, m;
    HashFunc() : a(1), b(1), m(1) {}
    HashFunc(int m) : m(m){
        a = 1 + gen.next() % (prime - 1);
        b = gen.next() % prime;
    }
    uint32_t calc(uint32_t k){
        return ((a * k + b) % prime) % m;
    }
};

struct OpenHash{
    vector<vector<int>> table;
    HashFunc h;
    int m, cnt;
    OpenHash(int m) : m(m){
        table.assign(m, vector<int>());
        h = HashFunc(m);
        cnt = 0;
    }
    int get(int key){
        int idx = h.calc(key);
        int pos = -1;
        for(int i = 0; i < table[idx].size(); i++){
            if(table[idx][i] == key) pos = i;
        }
        return pos;
    }
    void insert(int key, bool flag){
        int idx = h.calc(key);
        int pos = get(key);
        if(pos == -1){
            if(2*cnt > m){ // Rehashing
                OpenHash aux(2*m + 1);
                for(int i = 0; i < m; i++){
                    for(int j = 0; j < table[i].size(); j++){
                        int val = table[i][j];
                        aux.insert(val, false);
                    }
                }
                table = aux.table;
                h = aux.h;
                m = aux.m;
                cnt = aux.cnt;
                idx = h.calc(key);
            }
            table[idx].push_back(key);
            cnt++;
            if(flag) cout << "I " << key << " " << idx << " " << table[idx].size() - 1 << '\n';
        }
        else{
            if(flag) cout << "I " << key << " " << idx << " " << pos << '\n';
        }
    }
};

struct PerfectHash{
    vector<vector<int>> table;
    HashFunc h;
    vector<HashFunc> hashs;
    int m;
    PerfectHash(vector<int> v){
        int n = v.size();
        m = 2 * (n/2) + 1;
        int sum = 0;
        // part 1
        do{
            h = HashFunc(m);
            table.assign(m, vector<int>());
            for(int i = 0; i < n; i++){
                int val = v[i];
                int idx = h.calc(val);
                table[idx].push_back(val);
            }
            sum = 0;
            for(int i = 0; i < m; i++){
                int sz = table[i].size();
                sum += sz * sz;
            }
        }while(sum > 4 * n);
        // Part 2
        vector<int> tableSzs(m, 0);
        hashs.assign(m, HashFunc());
        for(int i = 0; i < m; i++){
            int sz = table[i].size();
            if(sz == 0) continue;
            tableSzs[i] = sz * sz + 1;
            bool colision;
            do{
                colision = false;
                vector<int> aux(tableSzs[i], -1);
                hashs[i] = HashFunc(tableSzs[i]);
                for(int j = 0; j < table[i].size(); j++){
                    int val = table[i][j];
                    int idx = hashs[i].calc(val);
                    if(aux[idx] != -1) colision = true;
                    aux[idx] = val;
                }
                if(!colision){
                    table[i] = aux;
                }
            }while(colision);
        }
    }
    bool get(int key, bool flag){
        int idx1 = h.calc(key);
        if(table[idx1].size() == 0){
            if(flag) cout << "Q " << key << " " << -1 << " " << -1 << '\n';
            return false;
        }
        else{
            int idx2 = hashs[idx1].calc(key);
            if(table[idx1][idx2] == key){
                if(flag) cout << "Q " << key << " " << idx1 << " " << idx2 << '\n';
                return true;
            }
            else{
                if(flag) cout << "Q " << key << " " << -1 << " " << -1 << '\n';
                return false;
            }
        }
    }
};

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    
    int S, U, M0, I, Q, Pi, Pq;
    while(cin >> S >> U >> M0 >> I >> Q >> Pi >> Pq){
        gen = RNG(S, 1664525, 1013904223);
        OpenHash t0(M0);
        for(int i = 0; i < I; i++){
            int k = gen.next() % U;
            bool flag = !(i % Pi);
            t0.insert(k, flag);
        }
        vector<int> v;
        for(int i = 0; i < t0.table.size(); i++){
            for(int j = 0; j < t0.table[i].size(); j++){
                int val = t0.table[i][j];
                v.push_back(val);
            }
        }
        PerfectHash t1(v);
        for(int i = 0; i < Q; i++){
            int k = gen.next() % U;
            bool flag = !(i % Pq);
            t1.get(k, flag);
        }
    }
    return 0;
}