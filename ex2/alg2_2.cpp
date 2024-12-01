#include <iostream>
#include <vector>
#include <list>
#include <deque>
using namespace std;

// Define a struct to hold two integers
struct Edge {
    int node;
    int weight;
};

// Define a struct to hold a list of edges
struct ConnectedNodes {
    list<Edge> edges;
    int p = 0, s = 0;
    int path = -1;
    int optimal = -1;

};

#include <iostream>
#include <vector>

using namespace std;

// Struct to encapsulate convex hull data
struct ConvexHull {
    vector<int> slope;
    vector<int> intercept;
    int pointer;  // Pointer to the current best line in the convex hull

    // Constructor to initialize data
    ConvexHull() : pointer(0) {}
};

// Function to add a line to the convex hull
void addLine(ConvexHull &convexHull, int a, int b) {
    // Add the line to the convex hull
    convexHull.slope.push_back(a);
    convexHull.intercept.push_back(b);

    // Adjust the convex hull by removing unnecessary lines
    while (convexHull.slope.size() >= 3 &&
           (convexHull.intercept[convexHull.slope.size() - 3] - convexHull.intercept[convexHull.slope.size() - 1]) *
           (convexHull.slope[convexHull.slope.size() - 1] - convexHull.slope[convexHull.slope.size() - 2]) >=
           (convexHull.intercept[convexHull.slope.size() - 2] - convexHull.intercept[convexHull.slope.size() - 1]) *
           (convexHull.slope[convexHull.slope.size() - 1] - convexHull.slope[convexHull.slope.size() - 3])) {
        convexHull.slope.erase(convexHull.slope.end() - 2);
        convexHull.intercept.erase(convexHull.intercept.end() - 2);
    }

    // Update the pointer to the current best line
    if (convexHull.pointer >= convexHull.slope.size()) {
        convexHull.pointer = convexHull.slope.size() - 1;
    }
}

// Function to query the convex hull for a specific value of 'x'
int query(ConvexHull &convexHull, int x) {
    // Find the line with the minimum value at 'x' using the convex hull
    while (convexHull.pointer < convexHull.slope.size() - 1 &&
           convexHull.slope[convexHull.pointer + 1] * x + convexHull.intercept[convexHull.pointer + 1] <=
           convexHull.slope[convexHull.pointer] * x + convexHull.intercept[convexHull.pointer]) {
        convexHull.pointer++;
    }

    // Return the result based on the current best line in the convex hull
    return convexHull.slope[convexHull.pointer] * x + convexHull.intercept[convexHull.pointer];
}

int ConvexHullTrick(int node, vector<ConnectedNodes>& tree, int* result, int* next, int* path, int* optimal){

    // Create an instance of the ConvexHull struct
    ConvexHull convexHull;

    // Example input for 'a' and 'b' values for each iteration i
    // Replace these with your actual values
    vector<int> a_values = { /* Your 'a' values for each i go here */ };
    vector<int> b_values = { /* Your 'b' values for each i go here */ };

    // Example input for 'x' values for each iteration i
    // Replace these with your actual values
    vector<int> x_values = { /* Your 'x' values for each i go here */ };

    // Array to store calculated results
    vector<int> results[a_values.size()];

    // Iterate over the values of 'a', 'b', and 'x'
    for (int i = 0; i < a_values.size(); ++i) {
        // Call the convex hull trick function for each iteration i
        addLine(convexHull, tree[node].path, tree[node].p);        // Store the result in the array
        tree[next[node]].optimal = (query(convexHull, tree[next[node]].path) + tree[next[node]].s * tree[next[node]].path + tree[next[node]].p); // Store the result in the array
    }

    return 0;
}




int* eval(int node, vector<ConnectedNodes>& tree, int* result, int* next) {

    int i, min_val, trying;
    ConnectedNodes cur_node;

    while (!tree[node].edges.empty()){ //while there are still nodes connected to the node we just evaluated

        Edge calculating = tree[node].edges.back(); //take the last one of them

        if(tree[calculating.node].optimal==-1){ //if this node isn't yet evaluated

        next[calculating.node] = node; //update the path in the next matrix
        tree[calculating.node].path = tree[node].path + calculating.weight;
        result[calculating.node] = tree[calculating.node].p + tree[calculating.node].s * tree[calculating.node].path;//calculate the fixed part of the eval for the node we are now examining
        i=calculating.node; //check all the nodes in the path to the starting node
        min_val = 0;

        while(next[i]!=-1){ //until i reach the starting node

            cur_node = tree[next[i]];//the node i am examining
            trying = cur_node.optimal - tree[calculating.node].s * cur_node.path; //its eval
            if(min_val>trying)
                min_val = trying; //if its eval is better than the one so far, use this instead

            i = next[i]; //go to the next node in the path
        }
        result[calculating.node] += min_val;
        tree[calculating.node].optimal = result[calculating.node]; // provide the final result for this node

        result = eval(calculating.node, tree, result, next); //repeat the eval for all the nodes connected to the one we just examined
        }
                tree[node].edges.pop_back(); //pop the node we just examined
    }

    return result;
}

int* build_routes(int node, vector<ConnectedNodes>& tree, int* result, int* next) {

    Edge calculating;

    while (!tree[node].edges.empty()) { //while there are still nodes connected to the node we just evaluated

        calculating = tree[node].edges.back(); //take the last one of them

        if (calculating.node != node) { //if this node isn't yet evaluated
            next[calculating.node] = node; //update the path in the next matrix
            tree[calculating.node].path = tree[node].path + calculating.weight;
            build_routes(calculating.node, tree, result, next); //repeat the eval for all the nodes connected to the one we just examined
        }

        tree[node].edges.pop_back(); //pop the node we just examined
    }

    return next;
}

int* dp(int N, vector<ConnectedNodes>& tree) {
    int* result = new int[N];
    int* next = new int [N];
    next[0] = -1;
    result = eval(0, tree, result, next);

    return result;
}

int* dp_110(int N, vector<ConnectedNodes>& tree) {
    int* result = new int[N];
    int* next = new int [N];
    next[0] = -1;
    int directions = tree[0].edges.size();
    int starting[directions];
    int i = 0;

    while (!tree[0].edges.empty()) {//while there are still nodes connected to the node we just evaluated
        starting[i] = tree[0].edges.back().node; //take the last one of them
        tree[0].edges.pop_back(); //pop the node we just examined
    }

    for (int i=0; i<directions; i++) {
        next[starting[i]] = 0; //update the path in the next matrix
        tree[starting[i]].path = tree[0].path + tree[starting[i]].edges.back().weight;
        next = build_routes(starting[i], tree, result, next);
    }

    for (int i=0; i<directions; i++) {
        result[starting[i]] = tree[starting[i]].p + tree[starting[i]].s * tree[starting[i]].path;//calculate the fixed part of the eval for the node we are now examining
        result = eval(starting[i], tree, result, next); //repeat the eval for all the nodes connected to the one we just examined
    }
    return result;
}

int main() {
    // Define the size of the matrix (one-dimensional)
    int N;
    cin >> N;

    // Create a vector of ConnectedNodes structs
    vector<ConnectedNodes> tree(N);

    int v1, v2, w;
    // Populate the matrix with some values
    for (int i = 0; i < N - 1; i++) {
        cin >> v1 >> v2 >> w;
        Edge element1 = {v2 - 1, w}; // Corrected v1-1 here
        Edge element2 = {v1 - 1, w}; // Corrected v2-1 here
        tree[v1 - 1].edges.push_back(element1);
        tree[v2 - 1].edges.push_back(element2);
    }

    tree[0].path = 0;
    tree[0].optimal = 0;

    for(int i=1; i<N; i++)
        cin>>tree[i].p>>tree[i].s;
    int* result;

    if(N<=8000)
        result = dp(N, tree);
    else
        result = dp_110(N, tree);
    for (int i = 1; i < N-1; i++)
        cout << result[i] << " ";
    cout<<result[N-1];

    cout << endl;

    // Don't forget to free the memory allocated in dp function
    delete[] result;

    return 0;
}
