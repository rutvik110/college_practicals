#include <iostream>
#include <stack>
#include <vector>
#include <omp.h>

using namespace std;

void parallelDFS(vector<vector<int>>& graph, int source) {
    int numVertices = graph.size();
    vector<bool> visited(numVertices, false);

    stack<int> s;
    s.push(source);
    
    //double start_time = omp_get_wtime(); // Start the timer

    while (!s.empty()) {
        int currentVertex = s.top();
        s.pop();

        if (!visited[currentVertex]) {
            visited[currentVertex] = true;

            // Process the current vertex
            cout << "Visited vertex: " << currentVertex << endl;

            // Find neighbors of the current vertex
            vector<int> neighbors;
            #pragma omp parallel for
            for (int i = 0; i < numVertices; i++) {
                if (graph[currentVertex][i] && !visited[i]) {
                    #pragma omp critical
                    neighbors.push_back(i);
                }
            }

            // Push the neighbors onto the stack
            for (int neighbor : neighbors) {
                s.push(neighbor);
            }
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

    parallelDFS(graph, sourceVertex);

    return 0;
}
