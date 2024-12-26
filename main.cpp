#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <vector>

#define INF 1e9

class Graph {
public:
  int V;
  std::vector<std::vector<int>> adjMatrix;

  Graph(int vertices) : V(vertices) {
    adjMatrix.resize(V, std::vector<int>(V, INF));
    for (int i = 0; i < V; ++i) {
      adjMatrix[i][i] = 0;
    }
  }

  void addEdge(int src, int dest, int weight) { adjMatrix[src][dest] = weight; }

  void displayDistanceMatrix(std::vector<std::vector<int>> dist) {
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

  void applyfloydWarshallSimple(double &timeWithoutParallel) {
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

    auto start_time = high_resolution_clock::now();
    std::vector<std::vector<int>> dist = adjMatrix;

    for (int k = 0; k < V; ++k) {
      for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
          if (dist[i][k] < INF && dist[k][j] < INF) {
            dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
          }
        }
      }
    }

    auto end_time = high_resolution_clock::now();
    duration<double, std::milli> total_runtime = end_time - start_time;
    timeWithoutParallel = total_runtime.count();
  }

  void applyfloydWarshallParallel(int num_threads, double &timeWithParallel) {
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

    omp_set_num_threads(num_threads);
    auto start_time = high_resolution_clock::now();
    std::vector<std::vector<int>> dist = adjMatrix;

// Parallelize Floyd-Warshall algorithm
#pragma omp parallel for collapse(2)
    for (int k = 0; k < V; ++k) {
      for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
          if (dist[i][k] < INF && dist[k][j] < INF) {
            dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
          }
        }
      }
    }

    auto end_time = high_resolution_clock::now();
    duration<double, std::milli> total_runtime = end_time - start_time;
    timeWithParallel = total_runtime.count();
  }
};

int main() {
  int nodes = 1000;
  double probability = 0.5;
  srand(time(0));

  Graph g(nodes);

  for (int i = 0; i < nodes; ++i) {
    for (int j = 0; j < nodes; ++j) {
      if (i != j && ((double)rand() / RAND_MAX) < probability) {
        int weight = rand() % 100 + 1;
        g.addEdge(i, j, weight);
      }
    }
  }

  std::cout << "Graph generated with " << nodes << " nodes and ~"
            << probability * 100 << "% connection probability.\n";

  double timeWithoutParallel;
  g.applyfloydWarshallSimple(timeWithoutParallel);

  // Loop through different numbers of threads to calculate speedup
  const int MAX_THREADS = 8;
  std::vector<int> thread_counts;
  std::vector<double> speedups;

  for (int num_threads = 1; num_threads <= MAX_THREADS; num_threads++) {
    double timeWithParallel;
    g.applyfloydWarshallParallel(num_threads, timeWithParallel);

    double speedup = timeWithoutParallel / timeWithParallel;
    thread_counts.push_back(num_threads);
    speedups.push_back(speedup);

    std::cout << "Threads: " << num_threads << " Speedup: " << speedup << "\n";
  }

  // Print results
  std::cout << "\nThread Counts: ";
  for (int t : thread_counts) {
    std::cout << t << " ";
  }
  std::cout << "\nSpeedups: ";
  for (double s : speedups) {
    std::cout << s << " ";
  }

  return 0;
}
