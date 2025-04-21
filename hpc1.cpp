#include <iostream> 
#include <vector> 
#include <queue> 
#include <unordered_set> 
#include <omp.h> 
#include <chrono> 
 
using namespace std; 
using namespace std::chrono; 
 
class Graph { 
    int V; 
    vector<int> *adj; 
 
public: 
    Graph(int V) { 
        this->V = V; 
        adj = new vector<int>[V]; 
    } 
 
    void addEdge(int u, int v) { 
        adj[u].push_back(v); 
        adj[v].push_back(u); 
    } 
 
    void BFS_Normal(int start) { 
        unordered_set<int> visited; 
        queue<int> q; 
 
        visited.insert(start); 
        q.push(start); 
 
        while (!q.empty()) { 
            int u = q.front(); 
            q.pop(); 
            cout << u << " "; 
 
            for (int v : adj[u]) { 
                if (visited.find(v) == visited.end()) { 
                    visited.insert(v); 
                    q.push(v); 
                } 
            } 
        } 
        cout << endl; 
    } 
 
    void BFS_Parallel(int start) { 
        unordered_set<int> visited; 
        queue<int> q; 
 
        visited.insert(start); 
        q.push(start); 
 
        while (!q.empty()) { 
            int u = q.front(); 
            q.pop(); 
            cout << u << " "; 
 
            #pragma omp parallel for 
            for (size_t i = 0; i < adj[u].size(); i++) { 
                int v = adj[u][i]; 
                if (visited.find(v) == visited.end()) { 
                    #pragma omp cri cal 
                    { 
                        visited.insert(v); 
                        q.push(v); 
                    } 
                } 
            } 
        } 
        cout << endl; 
    } 
 
    void DFS_Normal(int start) { 
        vector<bool> visited(V, false); 
        DFS_Util(start, visited); 
        cout << endl; 
    } 
 
    void DFS_Parallel(int start) { 
        vector<bool> visited(V, false); 
        #pragma omp parallel 
        { 
            #pragma omp single nowait 
            { 
                DFS_Util(start, visited); 
            } 
        } 
        cout << endl; 
    } 
 
private: 
    void DFS_Util(int u, vector<bool>& visited) { 
        visited[u] = true; 
        cout << u << " "; 
 
        for (int v : adj[u]) { 
            if (!visited[v]) { 
                DFS_Util(v, visited); 
            } 
        } 
    } 
}; 
 
int main() { 
    int V, edgeCount; 
    cout << "Enter number of ver ces and edges: "; 
    cin >> V >> edgeCount; 
 
    Graph g(V); 
    cout << "Provide edges (u v):\n"; 
 
    for (int i = 0; i < edgeCount; i++) { 
        int u, v; 
        cin >> u >> v; 
        g.addEdge(u, v); 
    } 
 
    // Measure BFS Normal Time 
    auto start = high_resolution_clock::now(); 
    g.BFS_Normal(0); 
    auto stop = high_resolution_clock::now(); 
    cout << "BFS (Normal) Time: " <<duration_cast<microseconds>(stop - start).count() << "µs\n"; 
 
    // Measure BFS Parallel Time 
    start = high_resolution_clock::now(); 
    g.BFS_Parallel(0); 
    stop = high_resolution_clock::now(); 
    cout << "BFS (Parallel) Time: " <<duration_cast<microseconds>(stop - start).count() << "µs\n"; 
 
    // Measure DFS Normal Time 
    start = high_resolution_clock::now(); 
    g.DFS_Normal(0); 
    stop = high_resolution_clock::now(); 
    cout << "DFS (Normal) Time: " <<duration_cast<microseconds>(stop - start).count() << "µs\n"; 
 
    // Measure DFS Parallel Time 
    start = high_resolution_clock::now(); 
    g.DFS_Parallel(0); 
    stop = high_resolution_clock::now(); 
    cout << "DFS (Parallel) Time: " <<duration_cast<microseconds>(stop - start).count() << "µs\n"; 
    return 0; 
} 
 
// g++ -fopenmp -o bfsdfs bfsdfs.cpp
// ./bfsdfs

// Output: 
// Enter number of ver ces and edges: 6 7 
// 0 1 
// 0 2 
// 1 3 
// 1 4 
// 2 4 
// 3 5 
// 4 5 
// BFS (Normal): [0] [1] [2] [3] [4] [5]  
// BFS (Normal) Time: 32µs 
 
// BFS (Parallel): [0] [1] [2] [3] [4] [5]  
// BFS (Parallel) Time: 27µs 
 
// DFS (Normal): {0} {1} {3} {5} {4} {2}  
// DFS (Normal) Time: 22µs 
 
// DFS (Parallel): {0} {1} {3} {5} {4} {2}  
// DFS (Parallel) Time: 19µs 