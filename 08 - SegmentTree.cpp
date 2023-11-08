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

struct SegmentTree{
    struct Node{
        int mn, mx, sum;
        Node(int x){
            mn = x;
            mx = x;
            sum = x;
        }
        Node(Node l, Node r){
            mn = min(l.mn, r.mn);
            mx = max(l.mx, r.mx);
            sum = l.sum + r.sum;
        }
    };
    vector<Node> seg;
    int n;
    int type;
    SegmentTree(vi& v){
        n = v.size();
        seg.assign(4 * n, 0);
        build(1, 0, n - 1, v);
    }

    void build(int p, int l, int r, vi& v){
        if(l == r){
            seg[p] = Node(v[l]);
        }
        else{
            int m = (l + r) >> 1;
            int lc = 2*p;
            int rc = lc + 1;
            build(lc, l, m, v);
            build(rc, m + 1, r, v);
            seg[p] = Node(seg[lc], seg[rc]);
        }
    }

    void update(int p, int l, int r, int idx, int x){
        if(l == r){
            seg[p] = Node(x);
        }
        else{
            int m = (l + r) >> 1;
            int lc = 2*p;
            int rc = lc + 1;
            if(idx <= m) update(lc, l, m, idx, x);
            else update(rc, m + 1, r, idx, x);
            seg[p] = Node(seg[lc], seg[rc]);
        }
    }
    Node query(int p, int l, int r, int ql, int qr){
        if(l >= ql && r <= qr) return seg[p];
        int m = (l + r) >> 1;
        int lc = 2*p;
        int rc = lc + 1;
        if(qr <= m) return query(lc, l, m, ql, qr);
        else if(ql > m) return query(rc, m + 1, r, ql, qr);
        else return Node(query(lc, l, m, ql, qr), query(rc, m + 1, r, ql, qr));
    }
    inline int query(int l, int r, int type){
        if(type == 0) return query(1, 0, n - 1, l, r).mn;
        else if(type == 1) return query(1, 0, n - 1, l, r).mx;
        else return query(1, 0, n - 1, l, r).sum;
    }
};

int32_t main() {
    optimize;
    
    int S, N, W, Q, U, P;
    string F;
    int cs = 0;
    while(cin >> S >> N >> F >> W >> Q >> U >> P){
        cout << "caso " << cs << '\n';
        gen = RNG(S, 1664525, 1013904223);
        vi v(N);
        int mod = 4 * N;
        for(int i = 0; i < N; i++){
            int X = gen.next() % mod;
            v[i] = X;
        }
        int type;
        if(F == "MIN"){
            type = 0;
        }
        else if(F == "MAX"){
            type = 1;
        }
        else type = 2;
        SegmentTree seg(v);
        for(int i = 0; i < W; i++){
            bool flag = !(i % P);
            int X = gen.next();
            if(X % (Q + U) < Q){
                // Query
                int l = gen.next() % N;
                int r = l + (gen.next() % (N - l));
                if(flag) cout << seg.query(l, r, type) << '\n';
            }
            else{
                // Update
                int idx = gen.next() % N;
                int val = gen.next() % mod;
                seg.update(1, 0, N - 1, idx, val);
                if(flag) cout << seg.query(idx, N - 1, type) << '\n';
            }
        }
        cout << '\n';
        cs++;
    }
    return 0;
}