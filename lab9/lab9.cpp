#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> adj; 

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void add(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void BFS(int start, int component_number, vector<int>& visit) {
        vector<int> queue; 
        queue.push_back(start); 
        visit[start] = component_number; 

        int idx = 0; 
        while (idx < queue.size()) {
            int tmp = queue[idx]; 
            idx++; 
            for (int j = 0; j < adj[tmp].size(); j++) {
                if (visit[adj[tmp][j]] == -1) { 
                    queue.push_back(adj[tmp][j]);
                    visit[adj[tmp][j]] = component_number; 
                }
            }
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    Graph g(n);
    vector<int> visit(n, -1); 

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g.add(u - 1, v - 1); 
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (visit[i] == -1) {
            //count++;
            g.BFS(i, count, visit); 
            count++;
        }
    }

    vector<vector<int>> result(count);
    for (int i = 0; i < n; i++) {
        result[visit[i]].push_back(i);
    }

    sort(result.begin(), result.end());

    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] + 1 << " "; 
        }
        cout << endl;
    }

    return 0;
}
