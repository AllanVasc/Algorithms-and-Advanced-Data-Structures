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

RNG gen(1,1,1);

struct SkipList{
    struct Node{
        int key, val, height;
        vector<Node*> next;
        Node(int key = -INF, int val = -INF, int h = 1){
            this->key = key;
            this->val = val;
            this-> height = h;
            this->next.assign(h, NULL);
        }
    };
    Node head;
    int cnt = 0;
    SkipList(){}

    // Find precursors in each level
    vector<Node*> precursors(int x){
        vector<Node*> p(head.height, NULL);
        Node* cur = &head;
        cnt = 1;
        for(int i = head.height - 1; i >= 0; i--){
            while(cur->next[i] != NULL && cur->next[i]->key < x){
                cur = cur->next[i];
                cnt++;
            }
            p[i] = cur;
        }
        return p;
    }

    Node* search(int x, bool flag){
        vector<Node*> p = precursors(x);
        if(p[0]->next[0] != NULL && p[0]->next[0]->key == x){
            if(flag){
                cout << "F " << cnt << " " << p[0]->next[0]->height << '\n';
            }
            return p[0]->next[0];
        }
        else{
            if(flag){
                cout << "F " << cnt << " " << 0 << '\n';
            }
            return NULL;
        }
    }

    int setHeight(){
        int h = 1;
        int mxH = head.height + 1;
        while((gen.next() % 100 < 50) && h < mxH) h++;
        return h;
    }

    void insert(int key, int val, bool flag){
        vector<Node*> p = precursors(key);
        // key already exists, We need to update val
        if(p[0]->next[0] != NULL && p[0]->next[0]->key == key){
            if(flag){   // Invalid
                cout << "I " << 0 <<  '\n';
            }
            p[0]->next[0]->val = val;
            return;
        }
        int h = setHeight();
        if(h > head.height){
            head.height = h;
            while(head.next.size() < h) head.next.push_back(NULL);
            while(p.size() < h) p.push_back(&head);
        }
        Node* aux = new Node(key, val, h);
        for(int i = 0; i < h; i++){
            aux->next[i] = p[i]->next[i];
            p[i]->next[i] = aux;
        }
        if(flag){   // Sucess
            cout << "I " << 1 <<  '\n';
        }
    }

    void erase(int key, bool flag){
        vector<Node*> p = precursors(key);
        if(p[0]->next[0] == NULL || p[0]->next[0]->key != key){
            if(flag){ // Invalid
                cout << "D " << 0 <<  '\n';
            }
            return;
        }
        Node* aux = p[0]->next[0];
        for(int i = 0; i < aux->height; i++){
            p[i]->next[i] = aux->next[i];
        }
        while(head.height > 1 && head.next.back() == NULL){
            head.height--;
            head.next.pop_back();
        }
        if(flag){ // Sucess
            cout << "D " << 1 <<  '\n';
        }
    }
};

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    
    int S, U, B, N, F, I, D, P;
    while(cin >> S >> U >> B >> N >> F >> I >> D >> P){
        gen = RNG(S, 1664525, 1013904223);
        SkipList skplst;
        // Burn-in
        for(int i = 0; i < B; i++){
            uint32_t val = gen.next();
            skplst.insert(val % U, val % U, 0);
        }

        for(int i = 0; i < N; i++){
            uint32_t X = gen.next() % (F + I + D);
            bool flag = !(i % P);
            if(X < F){ // Find
                X = gen.next() % U;
                skplst.search(X, flag);
            }
            else if(F <= X && X < F + I){   // Insert
                X = gen.next() % U;
                skplst.insert(X, X, flag);
            }
            else if(F + I <= X){    // Delete
                X = gen.next() % U;
                skplst.erase(X, flag);
            }
        }
    }
    return 0;
}