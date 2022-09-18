#ifndef WEBGRAPH_H
#define WEBGRAPH_H

#include "Trie.h"

// Data structure to store a graph edge
struct Edge {
    int src, dest;
    Edge(int s, int d)
    {
        src = s;
        dest = d;
    }
};

// A class to represent a graph object
class Graph
{
public:
    vector<vector<double>> adjMatrix;
    vector<double> pr;
    vector<int> out;
    int N;
    double dampingF, stoppingF;

    // Graph Constructor
    Graph();
    Graph(vector<Edge> edges, int n, double df, double sf);
    void add_edge(int u, int v);
    bool is_zeros(int j);
    void convert(int j, double val);
    bool done(vector<double> x, vector<double> y);
    void pagerank();
    void printGraph();
};
#endif 