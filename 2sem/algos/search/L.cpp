#include <bits/stdc++.h>
using namespace std;
//#define int long long

struct Node {
    int value;
    int prior;
    Node* left;
    Node* right;
    Node* parent;

    Node(Node* l, Node* r, int v, int p) {
        left = l;
        right = r;
        value = v;
        prior = p;
    }

    Node(int v, int p) : left(nullptr), right(nullptr), value(v), prior(p) {}

    Node(int v, int p, Node* pr) : Node(v, p) {
        parent = pr;
    }
};

#define pNode Node*

vector<string> orderedOut;
vector<int> vals;
map<int, int> priors;
map<int, int> priorToOrder;

pNode fastInsert(vector<int> vals, map<int,int> priors) 
{
    pNode previous = new Node(vals[0], priors[vals[0]]);
    for (int i = 1; i < vals.size(); i++) {
        int currentPrior = priors[vals[i]];
        if (currentPrior > previous->prior) {
            previous->right = new Node(vals[i], currentPrior, previous);
            previous = previous->right;
        } else {
            pNode traverse = previous;
            while (traverse->parent && traverse->prior > currentPrior) traverse = traverse->parent;
            if (traverse->prior > currentPrior) {
                previous = new Node(vals[i], currentPrior, nullptr);
                previous->left = traverse;
                traverse->parent = previous;
            } else {
                previous = new Node(vals[i], currentPrior, traverse);
                previous->left = traverse->right;
                traverse->right->parent = previous;
                traverse->right = previous;
            }
        }
    }
    return previous;
}

int getOrder(pNode n) {
    return priorToOrder[n->prior];
}

void dfs(pNode tree, pNode from) {
    if (tree->left) { 
        dfs(tree->left, tree);
        if (tree->right) dfs(tree->right, tree);
        orderedOut[getOrder(tree)-1] = ((from != tree) ? to_string(getOrder(from)) : "0") + " " + to_string(getOrder(tree->left)) + " " + ((tree->right) ? to_string(getOrder(tree->right)) : "0") ;
    } else if (tree->right) {
        dfs(tree->right, tree);
        orderedOut[getOrder(tree)-1] = ((from != tree) ? to_string(getOrder(from)) : "0") + " 0 " + to_string(getOrder(tree->right)) + " " ;
    } else {
        orderedOut[getOrder(tree)-1] = ((from != tree) ? to_string(getOrder(from)) : "0") + " 0 0";
    }
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    pNode tree = nullptr;
    int n;
    cin >> n;
    orderedOut.resize(n); 
    vals.resize(n);
    for (int i = 0; i < n; i++) {
        int prior;
        cin >> vals[i] >> prior;
        priors.emplace(vals[i], prior);
        priorToOrder.emplace(priors[vals[i]], i+1);
    }
    sort(vals.begin(), vals.end());
    tree = fastInsert(vals, priors);
    while (tree->parent) tree = tree->parent;
    cout << "YES" << "\n";

    dfs(tree, tree);
    for (string item : orderedOut) cout << item << "\n";

    return 0;
}
