#include "Graph.h"

Graph::Graph() {}
Graph::Graph(vector<Edge> edges, int n, double df, double sf = 0.1)
{
    dampingF = df;
    stoppingF = sf;
    N = n;
    adjMatrix.resize(n);
    out.resize(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            adjMatrix[i].push_back(0);
        }
    }
    for (auto& edge : edges)
    {
        add_edge(edge.src, edge.dest);
        out[edge.src]++;
    }

}

void Graph::add_edge(int u, int v) {
    adjMatrix[u][v]++;
}

bool Graph::is_zeros(int j)
{
    for (int i = 0; i < N; i++)
    {
        if (adjMatrix[i][j])
        {
            return false;
        }
    }
    return true;
}

void Graph::convert(int j, double val)
{
    for (int i = 0; i < N; i++)
    {
        adjMatrix[i][j] = val;
    }
}

bool Graph::done(vector<double> x, vector<double> y)
{
    for (int i = 0; i < N; i++)
    {
        if (x[i] - y[i] > stoppingF)
        {
            return false;
        }
    }
    return true;
}

void Graph::pagerank()
{
    // probabilities matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (adjMatrix[i][j])
            {
                adjMatrix[i][j] = adjMatrix[i][j] / out[i];
            }
        }
    }

    // stochastic matrix
    for (int j = 0; j < N; j++)
    {
        if (is_zeros(j))
        {
            convert(j, 1 / N);
        }
    }

    // multiply by (1 - damping factor)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            adjMatrix[i][j] = adjMatrix[i][j] * dampingF;
        }
    }

    // sum it with the multiplication of 1/n matrix and damping factor
    // transition matrix
    double rd = 1 - dampingF;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            adjMatrix[i][j] = adjMatrix[i][j] + (rd / N);
        }
    }

    // rank matrix
    for (int i = 0; i < N; i++)
    {
        pr.push_back(1.0);
    }

    // calulating rank
    vector<double> pr_new = pr;
    bool d = false;
    for (int i = 0; i < 100; i++)
    {
        if (done(pr, pr_new))
        {
            break;
        }
        for (int i = 0; i < N; i++)
        {
            double tmp = 0;
            for (int j = 0; j < N; j++)
            {
                tmp += adjMatrix[i][j] * pr[j];
            }
            pr_new[i] = tmp;
        }
        pr = pr_new;
    }
    pr_new.clear();


    // noramlize
    double sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += pr[i];
    }
    for (int i = 0; i < N; i++)
    {
        pr[i] /= sum;
    }

}

void Graph::printGraph()
{
    for (int i = 0; i < N; i++)
    {
        // print the current vertex number
        cout << i << " ——> ";

        // print all neighboring vertices of a vertex `i`
        for (int v : adjMatrix[i]) {
            cout << v << " ";
        }

        cout << endl;
    }
}
