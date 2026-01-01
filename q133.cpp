#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Definition for a Node (LeetCode 133).
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() : val(0), neighbors() {}
    Node(int _val) : val(_val), neighbors() {}
    Node(int _val, vector<Node*> _neighbors) : val(_val), neighbors(_neighbors) {}
};

class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        unordered_map<Node*, Node*> m;
        queue<Node*> q;
        q.push(node);
        m[node] = new Node(node->val);
        while (!q.empty()) {
            Node* cur = q.front(); q.pop();
            for (Node* nei : cur->neighbors) {
                if (m.find(nei) == m.end()) {
                    m[nei] = new Node(nei->val);
                    q.push(nei);
                }
                m[cur]->neighbors.push_back(m[nei]);
            }
        }
        return m[node];
    }
};

// Helpers for testing: build graph from adjacency list (1-indexed values), print adjacency list
Node* buildGraph(const vector<vector<int>>& adj) {
    if (adj.empty()) return nullptr;
    int n = (int)adj.size();
    vector<Node*> nodes(n + 1, nullptr);
    for (int i = 1; i <= n; ++i) nodes[i] = new Node(i);
    for (int i = 1; i <= n; ++i) {
        for (int v : adj[i-1]) nodes[i]->neighbors.push_back(nodes[v]);
    }
    return nodes[1];
}

vector<vector<int>> collectAdj(Node* node) {
    vector<vector<int>> res;
    if (!node) return res;
    unordered_map<int, vector<int>> mp;
    unordered_set<int> seen;
    queue<Node*> q;
    q.push(node);
    seen.insert(node->val);
    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        for (Node* nei : cur->neighbors) {
            mp[cur->val].push_back(nei->val);
            if (!seen.count(nei->val)) {
                seen.insert(nei->val);
                q.push(nei);
            }
        }
        // ensure node exists in map even if no neighbors
        if (mp.find(cur->val) == mp.end()) mp[cur->val] = {};
    }
    int maxv = 0;
    for (auto &p : mp) maxv = max(maxv, p.first);
    res.assign(maxv, {});
    for (auto &p : mp) {
        int idx = p.first - 1;
        res[idx] = p.second;
        sort(res[idx].begin(), res[idx].end());
    }
    return res;
}

bool sameStructure(Node* a, Node* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    vector<vector<int>> A = collectAdj(a);
    vector<vector<int>> B = collectAdj(b);
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i] != B[i]) return false;
    }
    return true;
}

void printAdj(const vector<vector<int>>& adj) {
    cout << "[";
    for (size_t i = 0; i < adj.size(); ++i) {
        if (i) cout << ", ";
        cout << "[";
        for (size_t j = 0; j < adj[i].size(); ++j) {
            if (j) cout << ",";
            cout << adj[i][j];
        }
        cout << "]";
    }
    cout << "]";
}

int main() {
    Solution sol;
    // Test 1: example: [[2,4],[1,3],[2,4],[1,3]]
    vector<vector<int>> adj1 = {{2,4},{1,3},{2,4},{1,3}};
    Node* g1 = buildGraph(adj1);
    Node* c1 = sol.cloneGraph(g1);
    cout << "Test 1 original: "; printAdj(collectAdj(g1)); cout << "\n";
    cout << "Test 1 cloned:   "; printAdj(collectAdj(c1)); cout << "\n";
    cout << "Test 1 same? " << (sameStructure(g1, c1) ? "YES" : "NO") << "\n\n";

    // Test 2: single node with no neighbors: [[]]
    vector<vector<int>> adj2 = {{}}; // one node (1) with empty neighbors
    Node* g2 = buildGraph(adj2);
    Node* c2 = sol.cloneGraph(g2);
    cout << "Test 2 original: "; printAdj(collectAdj(g2)); cout << "\n";
    cout << "Test 2 cloned:   "; printAdj(collectAdj(c2)); cout << "\n";
    cout << "Test 2 same? " << (sameStructure(g2, c2) ? "YES" : "NO") << "\n\n";

    // Test 3: null graph
    Node* g3 = buildGraph({});
    Node* c3 = sol.cloneGraph(g3);
    cout << "Test 3 original: null\n";
    cout << "Test 3 cloned:   " << (c3 ? "not-null" : "null") << "\n";
    cout << "Test 3 same? " << ((g3==nullptr && c3==nullptr) ? "YES" : "NO") << "\n";

    return 0;
}
