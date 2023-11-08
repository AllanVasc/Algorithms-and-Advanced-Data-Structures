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

typedef struct Node* pNode;
struct Node{
    int key, priority;
    pNode c[2];
    Node(){}
    Node(int key){
        this->key = key;
        this->priority = gen.next();
        c[0] = c[1] = NULL;
    }
};

struct Treap{
    int cnt;
    pNode head;
    Treap(){
        head = NULL;
        cnt = 0;
    }
    void insert(int key, bool flag){
        if(search(head, key) != NULL){
            if(flag) cout << "I " << key << " " << -1 << endl;
        }
        else{
            pNode it = new Node(key);
            cnt = 0;
            insert(head, it, flag);
        }
    }

    void erase(int key, bool flag){
        pNode answ = search(head, key);
        if(answ){
            cnt = 0;
            erase(head, key);
        }
        if(flag) cout << "D " << key << " " << (answ ? cnt : -1) << endl;
    }

    void query(int key, bool flag){
        cnt = 0;
        pNode answ = search(head, key);
        if(flag) cout << "Q " << key << " " << (answ ? cnt : -1) << endl;
    }

private:
    void split(pNode t, pNode & l, pNode & r, int key){
        if(!t) return void(l = r = 0);
        if(key <= t->key){
            split(t->c[0], l, t->c[0], key);
            r = t;
        }
        else{
            split(t->c[1], t->c[1], r, key);
            l = t;
        }
    }

    void merge(pNode & t, pNode l, pNode r){
        if(!l || !r) t = l ? l : r;
        else if(l->priority > r->priority){
            merge(l->c[1], l->c[1], r);
            t = l;
        }
        else{
            merge(r->c[0], l, r->c[0]);
            t = r;
        }
    }

    pNode search(pNode& t, int key){
        if(!t) return NULL;
        if(t->key == key) return t;
        cnt++;
        if(t->key < key) return search(t->c[1], key);
        else return search(t->c[0], key);
    }
    
    void insert(pNode& t, pNode it, bool flag){
        if(!t){
            t = it;
            if(flag) cout << "I " << it->key << " " << cnt << endl;
        }
        else if(it->priority > t->priority){
            split(t, it->c[0], it->c[1], it->key);
            t = it;
            if(flag) cout << "I " << it->key << " " << cnt << endl;
            return;
        }
        else{
            cnt++;
            insert(t->key <= it->key ? t->c[1] : t->c[0], it, flag);
        }
    }

    void erase(pNode & t, int key){
        if(t->key == key){
            pNode th = t;
            merge(t, t->c[0], t->c[1]);
            delete th;
        }
        else{
            cnt++;
            erase(key < t->key ? t->c[0] : t->c[1], key);
        }
    }
};

int32_t main() {
    // optimize;
    
    int S, U, B, N, I, D, Q, P;
    cin >> S >> U >> B >> N >> I >> D >> Q >> P;
    gen = RNG(S, 1664525, 1013904223);
    Treap t;
    for(int i = 0; i < B; i++){
        int X = gen.next() % U;
        t.insert(X, false);
    }
    for(int i = 0; i < N; i++){
        int X = gen.next() % (I + D + Q);
        bool flag = !(i % P);
        if(X < I){
            // Insert
            int K = gen.next() % U;
            t.insert(K, flag);
        }
        else if(X >= I && X < (I + D)){
            // Delete
            int K = gen.next() % U;
            t.erase(K, flag);
        }
        else{
            // Query
            int K = gen.next() % U;
            t.query(K, flag);
        }
    }
    return 0;
}