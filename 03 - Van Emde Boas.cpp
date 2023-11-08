#include <bits/stdc++.h>
using namespace std;

#define int long long
#define optimize ios::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define ALL(x) x.begin(), x.end()
#define ll long long
#define vi vector<int>
#define pii pair<int,int>
#define iii tuple<int,int,int>

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
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
RNG gen(1,1,1);

typedef struct VEB* pVEB;
struct VEB{
    int U, sqrtU, log2U;
    int mn, mx;
    pVEB summary;
    vector<pVEB> clusters;
    VEB(int U) : U(U){
        sqrtU = sqrt(U);
        log2U = log2(sqrtU);
        mn = U;
        mx = -1;

        if(U <= 2){ // Base Case
            summary = NULL;
        }
        else{
            summary = new VEB(sqrtU);
            clusters.resize(sqrtU);
            for(int i = 0; i < sqrtU; i++) clusters[i] = new VEB(sqrtU);
        }
    }

    int high(int x){ return x >> log2U; }
    int low(int x){ return x & (sqrtU - 1); }
    int index(int h, int l){ return h*sqrtU + l; }
};

bool search(pVEB v, int key){
    if(v->mn == key || v->mx == key) return true;
    else if(v->U == 2) return false;
    else{
        int h = v->high(key);
        int l = v->low(key);
        return search(v->clusters[h], l);
    }
}

void insert(pVEB v, int key, int & depth){
    if(v->U == 2){ // Base Case
        v->mn = min(v->mn, key);
        v->mx = max(v->mx, key);
        return;
    }
    if(v->mn == v->U){ // V is empty
        v->mn = v->mx = key;
        return;
    }
    if(key < v->mn) swap(v->mn, key);
    v->mx = max(v->mx, key);
    int h = v->high(key);
    int l = v->low(key);
    if(v->clusters[h]->mn == v->clusters[h]->U){    // Cluster is empty
        int dummy = 0;
        insert(v->summary, h, dummy);
    }
    depth++;
    insert(v->clusters[h], l, depth);
}

int sucessor(pVEB v, int key){
    if(v->U == 2){
        if(key == -1 && v->mn == 0) return 0;
        else if(key <= 0 && v->mx == 1) return 1;
        return v->U;
    }

    if(key < v->mn) return v->mn;
    else if(key >= v->mx) return v->U;

    int h = v->high(key);
    int l = v->low(key);

    if(l < v->clusters[h]->mx){
        int s = sucessor(v->clusters[h], l);
        return v->index(h, s);
    }
    else{
        h = sucessor(v->summary, h);
        if(h == v->summary->U) return v->U;
        else{
            int s = v->clusters[h]->mn;
            return v->index(h, s);
        }
    }
}

void erase(pVEB v, int key, int & depth){
    if(v->U == 2){
        if(key == v->mn){
            v->mn = (v->mx != v->mn ? v->mx : v->U);
        }
        if(key == v->mx){
            v->mx = (v->mn != v->U ? v->mn : -1);
        }
        return;
    }
    if(key == v->mn){
        int i = v->summary->mn;
        if(i == v->summary->U){
            v->mn = v->U;
            v->mx = -1;
            return;
        }
        else{
            v->mn = v->index(i, v->clusters[i]->mn);
            key = v->mn;
        }
    }
    int h = v->high(key);
    int l = v->low(key);
    depth++;
    erase(v->clusters[h], l, depth);
    if(v->clusters[h]->mn == v->clusters[h]->U){
        int dummy = 0;
        erase(v->summary, h, dummy);
    }
    if(key == v->mx){
        int i = v->summary->mx;
        if(i == -1){
            v->mx = v->mn;
        }
        else{
            v->mx = v->index(i, v->clusters[i]->mx);
        }
    }
}

int32_t main() {
    optimize;
    
    int S, M, B, N, I, F, D, P;
    cin >> S >> M >> B >> N >> I >> F >> D >> P;
    gen = RNG(S, 1664525, 1013904223);
    
    int U = (1 << (1 << M));
    pVEB t = new VEB(U);
    for(int i = 0; i < B; i++){
        int X = gen.next() % U;
        int aux = 0;
        if(!search(t, X)) insert(t, X, aux);
    }

    for(int i = 0; i < N; i++){
        bool flag = !(i % P);
        int X = gen.next() % (I + F + D);
        if(X < I){
            // insert
            int K = gen.next() % U;
            int depth = 0;
            if(!search(t, K)) insert(t, K, ++depth);
            if(flag) cout << "I " << depth << '\n';
        }
        else if(X < I + F){
            // Sucessor
            int K = gen.next() % U;
            if(flag){
                int s = sucessor(t, K);
                cout << "S " << s << '\n';
            }
        }
        else{
            // Delete
            int Y = gen.next() % U;
            int s = sucessor(t, Y);
            
            int K = (s == t->U ? Y : s);
            int depth = 0;
            if(search(t, K)) erase(t, K, ++depth);
            if(flag) cout << "D " << depth << '\n';
        }
    }
    return 0;
}