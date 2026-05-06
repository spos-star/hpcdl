#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

class Graph{
    int V;
    vector<vector<int>> adj;

    public:
        Graph(int V){
            this->V = V;
            adj.resize(V);
        }

        void addEdge(int u, int v){
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // parallel bfs 
        void parallelBFS(int start){
            vector<bool> visited(V, false);
            queue<int> q;
            visited[start] = true;
            q.push(start);

            while(!q.empty()){
                int size = q.size();
                int node;

                #pragma omp parallel for
                for(int i=0; i<size;i++){

                    #pragma omp critical
                    {
                        if (!q.empty()){
                            node = q.front();
                            q.pop();
                        }
                        else{
                            node = -1;
                        }
                    }
                    if (node == -1) continue;
                    cout << node << " ";

                    for (int neighbour : adj[node]){
                        if(!visited[neighbour]){
                            visited[neighbour] = true;
                            #pragma omp critical
                            {
                                q.push(neighbour);
                            }
                        }
                    }
                }
            }
        }

        // parallel DFS
        void parallelDFS(int start){
            vector<bool> visited(V, false);
            stack<int> st;
            st.push(start);

            while(!st.empty()){
                int node;

                #pragma omp critical
                {
                    if (!st.empty()){
                        node = st.top();
                        st.pop();
                    } else{
                        node = -1;
                    }
                }
                if (node == -1) continue;

                if(!visited[node]){
                    cout << node << " ";
                    visited[node] = true;

                    #pragma omp parallel for
                    for(int i=0; i<adj[node].size(); i++){
                        int neighbour = adj[node][i];
                        if(!visited[neighbour]){
                            #pragma omp critical
                            {
                                st.push(neighbour);
                            }
                        }
                    }
                }
                
            }

        }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // -------- BFS Timing --------
    auto start_bfs = high_resolution_clock::now();

    cout << "Parallel BFS: ";
    g.parallelBFS(0);

    auto end_bfs = high_resolution_clock::now();
    auto duration_bfs = duration_cast<microseconds>(end_bfs - start_bfs);

    cout << "\nTime BFS: " << duration_bfs.count() << " microseconds\n\n";

    // -------- DFS Timing --------
    auto start_dfs = high_resolution_clock::now();

    cout << "Parallel DFS: ";
    g.parallelDFS(0);

    auto end_dfs = high_resolution_clock::now();
    auto duration_dfs = duration_cast<microseconds>(end_dfs - start_dfs);

    cout << "\nTime DFS: " << duration_dfs.count() << " microseconds\n";

    return 0;
}