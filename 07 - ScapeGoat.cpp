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
    int key;
    pNode lc, rc, parent;
    Node(){}
    Node(int key){
        this->key = key;
        lc = rc = parent = NULL;
    }
};

struct ScapeGoat{
    pNode root;
    long double alfa;
    int n;
    int cnt;

    ScapeGoat(long double a){
        root = NULL;
        alfa = a;
        n = 0;
    }

    void insert(int key, bool flag){
        pNode curr = new Node(key);
        int depth1 = insertBST(curr);
        if(depth1 < 0){ // Already exists
            if(flag) cout << "I " << key << " " << depth1 << " " << depth1 << endl;
            delete curr;
        }
        else{
            if(depth1 > log(n, 1.0 / alfa)){ // Need to rebuild
                int dist = 0;
                int sz = size(curr);
                while(dist <= log(sz, 1.0 / alfa)){
                    dist++;
                    pNode p = curr->parent;
                    if(p->lc == curr){
                        sz += 1 + size(p->rc);
                    }
                    else sz += 1 + size(p->lc);

                    curr = curr->parent;
                }
                rebuild(curr);
            }
            if(flag){
                cnt = 0;
                search(root, key);
                int depth2 = cnt;
                cout << "I " << key << " " << depth1 << " " << depth2 << endl;
            }
        }
    }

    void query(int key, bool flag){
        cnt = 0;
        if(!search(root, key)){
            if(flag) cout << "Q " << key << " " << -1 << endl;
        }
        else{
            if(flag) cout << "Q " << key << " " << cnt << endl;
        }
    }

private:
    pNode search(pNode curr, int key){
        if(!curr) return NULL;
        if(curr->key == key) return curr;
        cnt++;
        if(key < curr->key) return search(curr->lc, key);
        else return search(curr->rc, key);
    }

    int size(pNode curr){
        if(!curr) return 0;
        else return 1 + size(curr->lc) + size(curr->rc);
    }

    int insertBST(pNode node){
        pNode aux = root;
        if(aux == NULL){
            root = node;
            n++;
            return 0;
        }
        bool done = false;
        int depth = 0;
        do{
            if(node->key < aux->key){
                if(!aux->lc){
                    aux->lc = node;
                    node->parent = aux;
                    done = true;
                }
                else aux = aux->lc;
            }
            else if(node->key > aux->key){
                if(!aux->rc){
                    aux->rc = node;
                    node->parent = aux;
                    done = true;
                }
                else aux = aux->rc;
            }
            else return -1; // Node already exists
            depth++;
        } while(!done);
        n++;
        return depth;
    }

    int log(long double x, long double y){
        return (int) (log2(x)/log2(y));
    }

    void flatten(pNode curr, vector<pNode> & v){
        if(!curr) return;
        flatten(curr->lc, v);
        v.push_back(curr);
        flatten(curr->rc, v);
    }

    void rebuild(pNode curr){
        vector<pNode> v;
        pNode p = curr->parent;
        flatten(curr, v);
        int sz = v.size();
        if(p == NULL){
            root = buildFromArray(v, 0, sz);
            root->parent = NULL;
        }
        else if(p->rc == curr){
            p->rc = buildFromArray(v, 0, sz);
            p->rc->parent = p;
        }
        else{
            p->lc = buildFromArray(v, 0, sz);
            p->lc->parent = p;
        }
    }

    pNode buildFromArray(vector<pNode> & v, int i, int sz){
        if(sz == 0) return NULL;
        int mid = sz / 2;
        v[i + mid]->lc = buildFromArray(v, i, mid);

        if(v[i + mid]->lc != NULL){
            v[i + mid]->lc->parent = v[i + mid];
        }

        v[i + mid]->rc = buildFromArray(v, i + mid + 1, sz - mid - 1);
        if(v[i + mid]->rc != NULL){
            v[i + mid]->rc->parent = v[i + mid];
        }
        return v[i + mid];
    }

};

int32_t main() {
    optimize;
    
    int S, U, B, N, I, Q, P;
    long double A;
    cin >> S >> U >> A >> B >> N >> I >> Q >> P;
    gen = RNG(S, 1664525, 1013904223);

    ScapeGoat t(A);
    for(int i = 0; i < B; i++){
        int X = gen.next() % U;
        t.insert(X, false);
    }

    for(int i = 0; i < N; i++){
        bool flag = !(i % P);
        int X = gen.next() % (I + Q);
        if(X < I){
            int K = gen.next() % U;
            t.insert(K, flag);
        }
        else{
            int K = gen.next() % U;
            if(flag) t.query(K, flag);
        }
    }
    return 0;
}