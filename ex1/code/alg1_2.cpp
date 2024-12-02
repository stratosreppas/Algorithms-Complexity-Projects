#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int max_c, N, M;

struct Edge {
    int u, v, w, p, k_w;
};

struct Subset{
    int parent, rank;
};

struct Optimal{
    int p,w,k_w = 0;
};

int find(Subset subsets[], int i){
    if(subsets[i].parent!=i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

void Union(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}
Optimal kruskal(Edge* edges) {
    // Sort edges in non-ascending order by kruskal weight
    sort(edges[0], edges[M], [](const Edge& a, const Edge& b) {
        return a.k_w > b.k_w;
    });

    Subset* subsets = new Subset[N];

    Optimal result;

    // Create V subsets with single elements
    for (int v = 0; v < N; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Iterate through all sorted edges
    for (int i=0; i<M; i++) {
        int x = find(subsets, edges[i].u);
        int y = find(subsets, edges[i].v);

        // If including this edge doesn't cause a cycle, add it to the result
        if (x != y) {
            result.p += edges[i].p;
            result.w += edges[i].w;
            result.k_w += edges[i].k_w;
            Union(subsets, x, y);
        }
    }
    delete[] subsets;

    return result;
}

Optimal binary_search(Edge edges[], int c){

    for(int i=0; i<M; i++){
            edges[i].k_w = edges[i].p - c * edges[i].w;
        }
    Optimal val = kruskal(edges);

    if(val.k_w < 0){
        binary_search(edges, (max_c - c)/2);
    }

    else if(val.k_w > 0){
        binary_search(edges, c/2);
    }

    else
        return val;

}

int main(){
    int u,v,p,w;
    cin>>N>>M;

    Edge* edges = new Edge[M];

    for(int i=0; i<M; i++){
        cin>>u>>v>>p>>w;
        edges[i] = {u,v,w,p,0};
    }

    //Find the max possible profit of the edges
    for(int i=0; i<M; i++)
        max_c += edges[i].p;

    max_c = max_c / M;

    //start the search from the mid
    int c = max_c/2;

    Optimal final = binary_search(edges, c);
    gcd();
}
