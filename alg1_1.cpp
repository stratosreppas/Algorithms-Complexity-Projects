#include <iostream>
#include <vector>
using namespace std;

int N,K;

bool evaluate(vector<int>& c, int mid){

    vector<int> help(N);

    for(int i=0; i<N; i++) {
        if (c[i] < mid)
            help[i] = -1; // if the number is smaller than the one we examine, evaluate it with -1
        else
            help[i] = 1; // if the number is greater or equal than the one we examine, evaluate it with -1
        if(i!=0)
            help[i]+=help[i-1]; //each time add them together so that th ith element of the matrix represents the starting subset with length i
    }
    int max_eval = help[K - 1]; // The shortest subset with min length K evaluated so far

    int min_eval = 0; // The minimum possible value to exclude

    /*
     * We now begin to iterate through the help[] matrix from the (K+1)th value to the Nth value. Each time, we do
     * the following:
     *      We check if taking into account the values before the i-Kth subset is effectively worsening the evaluation
     *          of our subset of minimum length K.
     *      We see taking the next element into consideration improves our evaluation
     * Because we start from the very first subset with length K, we can gradually increase its length to include more numbers and
     * see if the evaluation gets more than 0. If a starting subset of this subset doesn't help, we exclude it each time.
     * We do that in order to find a max value that is greater than 0. If this happens, it means that we can find a subset of min
     * lenght K, so that a number greater (or equal since that is what the value of 1 represents) than the one we examine
     * can be a median of this subset. So we have actually determined that there can be a median larger than the one we are examining.
     */
    for (int i = K; i < N && max_eval<=0; i++) {
        min_eval = min(min_eval, help[i - K]);
        max_eval = max(max_eval, help[i] - min_eval);
    }

    // we need O(N) make this evaluation
    return(max_eval > 0);
}


int binary_search(int min, int mid, int max, vector<int>& c, int max_val){

    if (min == mid || max == mid) //if we re-examined the same value, it means that we reached the end of the recursion
        // (we needed a number greater/smaller than n but the max/min value we have calculated is n)
        return max_val;

    bool eval = evaluate(c, mid); // evaluate if there can't be a median smaller than this one

    if (eval)
        max_val = mid;  // if this is true, this is our greatest median so far, until we find the next one.
    if(eval)
        max_val = binary_search(mid, (mid+max)/2, max, c, max_val); //if we need a greater value than the one we examined, we look at the right part of the possible numbers
    else
        max_val = binary_search(min, (mid+min)/2, mid, c, max_val); //if we need a lesser value than the one we examined, we look at the left part of the possible numbers

    // so we iterate through evaluation() O(log(N)) times
    return max_val;
}

int main(){
    cin>>N>>K;

    vector<int> c(N);

    for(int i=0; i<N; i++){
        cin>>c[i];
    }
    int result = binary_search(1, (N+1)/2, (N+1), c, 0);

    // The result should be available at O(NlogN)
    cout<<result<<endl;
}