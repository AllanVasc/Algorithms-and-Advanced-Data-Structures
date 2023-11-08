#include <bits/stdc++.h>
using namespace std;
  
#define int long long

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

struct SplayTree{
    struct Node{
        Node* par;
        Node* l;
        Node* r;
        int key;
        Node(Node* par, int key, Node* l = NULL, Node* r = NULL) : 
            par(par), key(key), l(l), r(r) {}
    };
    Node* root;
    int depth;

    SplayTree(){
        root = NULL;
        depth = 0;
    }

    Node* rotate_left(Node* x){
        Node* parX = x->par;
        Node* rX = x->r;
        Node* rlX = rX->l;
        x->r = rlX;
        if(rlX) rlX->par = x;
        rX->l = x;
        x->par = rX;
        rX->par = parX;
        if(parX && x == parX->l){
            parX->l = rX;
        }
        else if(parX && x == parX->r){
            parX->r = rX;
        }
        return rX;
    }
    Node* rotate_right(Node* x){
        Node* parX = x->par;
        Node* lX = x->l;
        Node* lrX = lX->r;
        x->l = lrX;
        if(lrX) lrX->par = x;
        lX->r = x;
        x->par = lX;
        lX->par = parX;
        if(parX && x == parX->l){
            parX->l = lX;
        }
        else if(parX && x == parX->r){
            parX->r = lX;
        }
        return lX;
    }
    Node* zig(Node* x){
        Node* parX = x->par; // Caso P != NULL
        if(x == parX->l){
            return rotate_right(parX);
        }
        else{
            return rotate_left(parX);
        }
    }
    Node* zigzag(Node* x){
        Node* P = x->par;
        Node* GP = P->par;
        if(P == GP->l){
            if(x == P->l){ // left - left
                rotate_right(GP);
                return rotate_right(P);
            }
            else{   // left-right
                rotate_left(P);
                return rotate_right(GP);
            }
        }
        else{
            if(x == P->r){ // Right - right
                rotate_left(GP);
                return rotate_left(P);
            }
            else{// Right - left
                rotate_right(P);
                return rotate_left(GP);
            }
        }
    }
    Node* splay(Node* x){
        while(x->par){
            Node* GP = x->par->par;
            if(GP == NULL){
                zig(x);
            }
            else{
                zigzag(x);
            }
        }
        return x;
    }
    Node* searchBST(Node* x, int key){
        if(x == NULL || x->key == key) return x;
        depth++;
        if(key < x->key) return searchBST(x->l, key);
        else return searchBST(x->r, key);
    }
    Node* findKey(int key, bool flag){
        Node* x = searchBST(root, key);
        if(x != NULL){
            if(flag) cout << "Q " << key << " " << depth << '\n';
            root = splay(x);
        }
        else{
            if(flag) cout << "Q " << key << " " << -1 << '\n';
        }
        depth = 0;
        return x;
    }
    void insertAux(Node*& x, int key, bool flag, Node* p){
        if(x == NULL){
            x = new Node(p, key);;
            if(flag) cout << "I " << key << " " << depth << '\n';
        }
        else{
            depth++;
            if(x->key == key){
                if(flag) cout << "I " << key << " "  << -1 << '\n';
                return;
            }
            else if(key < x->key){
                insertAux(x->l, key, flag, x);
            }
            else{
                insertAux(x->r, key, flag, x);
            }
        }
    }
    void insert(int key, bool flag){
        insertAux(root, key, flag, NULL);
        depth = 0;
    }
};


int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    
    int S, U, B, N, I, Q, P;
    while(cin >> S >> U >> B >> N >> I >> Q >> P){
        gen = RNG(S, 1664525, 1013904223);
        SplayTree st;
        for(int i = 0; i < B; i++){
            int val = gen.next() % U;
            st.insert(val, false);
        }
        for(int i = 0; i < N; i++){
            int x = gen.next();
            if(x % (I + Q) < I){
                int k = gen.next() % U;
                bool flag = !(i % P);
                st.insert(k, flag);
            }
            else{
                int k = gen.next() % U;
                bool flag = !(i % P);
                st.findKey(k, flag);
            }
        }
    }
    return 0;
}