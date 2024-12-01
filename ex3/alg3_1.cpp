#include <iostream>
#include <bits/stdc++.h>
using namespace std;
//The code below uses segments of the GeekforGeeks code for kruskal's algorithm

class DSU { // Disjoint Set Union class, used to check if adding an edge to the spanning tree will create a cycle
    int* parent;
    int* rank;

public:
    DSU(int n)
    {
        parent = new int[n];
        rank = new int[n];

        for (int i = 0; i < n; i++) {
            parent[i] = -1;
            rank[i] = 1;
        }
    }

    // Find function
    int find(int i)
    {
        if (parent[i] == -1)
            return i;

        return parent[i] = find(parent[i]);
    }

    // Union function
    void unite(int x, int y)
    {
        int s1 = find(x);
        int s2 = find(y);

        if (s1 != s2) {
            if (rank[s1] < rank[s2]) {
                parent[s1] = s2;
            }
            else if (rank[s1] > rank[s2]) {
                parent[s2] = s1;
            }
            else {
                parent[s2] = s1;
                rank[s1] += 1;
            }
        }
    }
};

class Graph {
    vector<vector<int> > edgeList;
    int V;

public:
    Graph(int V) {
        this->V = V;
    }

    void addEdge(int x, int y, int w){
        //Adds an edge to the graph
        edgeList.push_back({w, x, y });
    }

    void highway(int K){
        // This is the function that solves our problem. It uses an altered version of Kruskal's algorithm, that instead of
        // the minimum spanning tree, returns a spanning tree with the egdes with the biggest weights. Then, by removing the
        // K smallest edges, we can find the answer to the problem, the smallest of the remaining edges, that will be the bottleneck

        // Sort all edges in decreasing order of weight
        sort(edgeList.begin(), edgeList.end(), greater<vector<int> >());

        // Execute Kruskal's algorithm
        DSU s(V);

        list<int> ans; // This list contains all the edges that are part of the spanning tree, in decreasing order of weight

        for (auto edge : edgeList) { //edgeList is now sorted, so the first element each time will be the biggest edge
            int w = edge[0];
            int x = edge[1];
            int y = edge[2];

            // Using Union-Find approach we can check if adding the edge will create a cycle or not, so we can add it to the spanning tree
            if (s.find(x) != s.find(y)) {
                s.unite(x, y);
                ans.push_back(w);
            }
        }

        // At this point, our spanning tree is complete
        // Now we just need to remove the K smallest edges from the spanning tree and produce the answer
        for(int i = 0; i < K; i++){
            ans.pop_back();
            if(ans.empty()){
                cout<<"infinity"<<endl;
                return;
            }
        }
        cout<<ans.back()<<endl;
    }
};

int main()
{
    long long int N, M, K;
    cin>>N>>M>>K;

    Graph g(N);

    long long int u, v, w;
    for(int i=0; i<M; i++){
        cin>>u>>v>>w;
        g.addEdge(u-1, v-1, w);
    }

    g.highway(K);

    return 0;
}
