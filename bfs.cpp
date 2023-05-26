#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

using namespace std;

void parallelBFS(vector<vector<int>>& graph, int source) {
    int numVertices = graph.size();
    vector<bool> visited(numVertices, false);

    queue<int> q;
    q.push(source);
    visited[source] = true;
    
    //double start_time = omp_get_wtime(); // Start the timer

    while (!q.empty()) {
        int currentVertex = q.front();
        q.pop();

        // Process the current vertex
        cout << "Visited vertex: " << currentVertex << endl;

        // Find neighbors of the current vertex
        vector<int> neighbors;
        #pragma omp parallel for
        for (int i = 0; i < numVertices; i++) {
            if (graph[currentVertex][i] && !visited[i]) {
                #pragma omp critical
                {
                    neighbors.push_back(i);
                    visited[i] = true;
                }
            }
        }

        // Enqueue the neighbors for the next level
        for (int neighbor : neighbors) {
            q.push(neighbor);
        }
    }
    
    // double end_time = omp_get_wtime(); // Stop the timer
    // double execution_time = end_time - start_time;

    // cout << "Execution Time: " << execution_time << " seconds" << endl;
}

int main() {
    // Define your graph here
    vector<vector<int>> graph = {
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 1},
        {0, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };

    int sourceVertex = 0;

    parallelBFS(graph, sourceVertex);

    return 0;
}
