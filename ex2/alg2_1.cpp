#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int dp(int n, int k, vector<int>& c){

    vector<vector<int>> dp_array(n, vector<int>(k+1, 0)); // Initialize the dp array with 0 profits

    for (int j = 1; j < k+1; j++) {

        int max_j = INT_MIN; // we set this int to calculate the best position to buy

        // now for the rest of the spaces
        for (int i = 1; i < n; i++) {

            max_j = max(dp_array[i - 1][j - 1] - c[i - 1], max_j); // the evaluation of the best position to buy.
            // It is O(1) because we have already calculated the previous values, and each time we add a j, we only need to
            //check with the max value of the previous j-1 values, which is stored in max_j (and updated each time we add a j)

            dp_array[i][j] = max(c[i] + max_j, // to sell...
                                        dp_array[i - 1][j] // or not to sell...
            );
        }
    }

    return dp_array[n-1][k];

}

int main(){

    int N, K;
    cin>>N>>K;

    vector<int> c(N);

    for(int i=0; i<N; i++){
        cin>>c[i];
    }

    int result = dp(N, K, c);

    cout<<result<<endl;
}
