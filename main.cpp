#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#define INF 1e9

// Graph class to represent the adjacency matrix for Floyd-Warshall
class Graph {
public:
  int V; // Number of vertices
  std::vector<std::vector<int> > adjMatrix;

  Graph(int vertices) : V(vertices) {
    adjMatrix.resize(V, std::vector<int>(V, INF));
    for (int i = 0; i < V; ++i) {
      adjMatrix[i][i] = 0;
    }
  }

  void addEdge(int src, int dest, int weight) { adjMatrix[src][dest] = weight; }

  void floydWarshall() {
    std::vector<std::vector<int> > dist = adjMatrix;
    for (int k = 0; k < V; ++k) {
      for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
          if (dist[i][k] < INF && dist[k][j] < INF) {
            dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
          }
        }
      }
    }

    std::cout << "The Distance matrix for Floyd-Warshall:\n";
    for (int i = 0; i < V; ++i) {
      for (int j = 0; j < V; ++j) {
        if (dist[i][j] == INF)
          std::cout << "INF\t";
        else
          std::cout << dist[i][j] << "\t";
      }
      std::cout << "\n";
    }
  }
};

int main() {
  using std::chrono::duration;
  using std::chrono::high_resolution_clock;

  auto start_time = high_resolution_clock::now();

  int nodes = 1000;
  double probability = 0.5;
  srand(time(0));

  Graph g(nodes);

  // Generate random edges based on probability
  for (int i = 0; i < nodes; ++i) {
    for (int j = 0; j < nodes; ++j) {
      if (i != j && ((double)rand() / RAND_MAX) < probability) {
        int weight = rand() % 100 + 1; // Random weight between 1 and 100
        g.addEdge(i, j, weight);
      }
    }
  }

  std::cout << "Graph generated with " << nodes << " nodes and ~"
            << probability * 100 << "% connection probability.\n";

  g.floydWarshall();

  auto end_time = high_resolution_clock::now();
  duration<double, std::milli> total_runtime = end_time - start_time;

  std::cout << "Program Execution Time: " << total_runtime.count() << "ms\n";

  return 0;
}