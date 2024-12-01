#include <bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;

#define INF 0x3f3f3f3f // Defining infinity, used for Dijkstra's algorithm

typedef pair<long long int, long long int> Edge;

class Graph {
    int V;
    list<pair<long long int, long long int> >* adj;
    vector<vector<long long int>> kings_route;
    vector<vector<long long int>> edge_array;

public:

    Graph(long long int V) : V(V), kings_route(V, vector<long long int>(V, -1)), edge_array(V, vector<long long int>(V)) {
        adj = new list<Edge>[V];
    }

    void addEdge(long long int u, long long int v, long long int w) {
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
        edge_array[u][v] = w;
        edge_array[v][u] = w;
    }

    void delete_edge_array(){
        edge_array.clear();
    }

    int add_kings_route(long long int u, long long int v, long long int weight){
        kings_route[u][v] = weight + edge_array[u][v];
        kings_route[v][u] = weight + edge_array[v][u];
        return kings_route[u][v];
    }

    // prints shortest path from s
    long long int shortestPath(long long int A, long long int T, long long int B){
            /*
             * The idea of the algorithm for this problem is that in normal circumstances, Dijsktra's algorithm can find
             * the shortest path between two nodes in a graph with constant weight. However, in this problem, the
             * king's route makes the weights non-constant. To solve this, we can dynamically change the weights of
             * the graphs based on the king's route, so in the end Dijkstra's algorithm can find the shortest path
             * between the two nodes taking into account the king's route.
             */

            //Dijkstra's algorithm
            priority_queue<Edge, vector<Edge>, greater<Edge> > pq;

            vector<long long int> dist(V, INF);

            pq.push(make_pair(0, A));
            dist[A] = T; // The initial distance from the source to itself is T, the time offset


            while (!pq.empty()) {
                long long int u = pq.top().second;
                pq.pop();
                list<pair<long long int, long long int> >::iterator i;
                for (i = adj[u].begin(); i != adj[u].end(); ++i) {
                    long long int v = (*i).first;
                    long long int weight = (*i).second;

                    // Here we must take into account the king's route
                    //if the king's route is not -1, it means that the king passes through that edge at some point
                    if(kings_route[u][v] != -1)
                        // Now we check if we can use this edge, or we have to wait for the king to pass, basically
                        // increasing its weight
                        if(dist[u]>=kings_route[u][v]-weight && dist[u]<kings_route[u][v])
                            weight += kings_route[u][v] - dist[u];

                    if (dist[v] > dist[u] + weight) {
                        dist[v] = dist[u] + weight;
                        pq.push(make_pair(dist[v], v));
                    }
                }
            }
            //The end result will be the final distance to the target minus the time offset
            return dist[B] - T;
        }

};

int main()
{
    long long int N, M;
    long long int A, B, T, K;
    long long int u, v, w;

    cin >> N >> M;
    cin >> A >> B >> T >> K;

    Graph g(N);
    long long int* kings_queue = new long long int[K];

    // Store the vertices the king will pass through in a priority queue
    for (int i = 0; i < K; i++){
        cin >> v;
        kings_queue[i] = v-1;
    }

    // Input the edges
    for(int i = 0; i < M; i++){
        cin >> u >> v >> w;
        g.addEdge(u-1, v-1, w);
        }

    // Calculate the time, where each road will be available again after the king passes through it.
    // This constructs the kings_route matrix, used to dynamically change the weights of the graph.
    long long int weight = 0;
    for(int i=0; i<K-1; i++){
        weight = g.add_kings_route(kings_queue[i],kings_queue[i+1], weight);
    }

    // Memory management
    g.delete_edge_array();
    delete[] kings_queue;

    cout<<g.shortestPath(A-1, T, B-1)<<endl;
    return 0;
}
