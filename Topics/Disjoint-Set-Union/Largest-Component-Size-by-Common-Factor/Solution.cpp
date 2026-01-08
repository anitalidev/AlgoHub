class Solution {
public:
    // Union two sets containing elements a and b
    // Uses union by size (stored as negative values in disset)
    void setUni (vector<int>& disset, int a, int b) {
        // Find the root of each element
        a = setFind(disset, a);
        b = setFind(disset, b);

        // If both elements are already in the same set, do nothing
        if (a == b) {
            // already same
            return;
        }

        // Compare sizes of the two trees
        // (more negative value means larger set)
        if (-1 * disset[a] < -1 * disset[b]) {
            // b tree is bigger, so swap to attach a under b
            swap(a, b);
        }

        // Merge b into a and update the size
        disset[a] += disset[b];
        disset[b] = a;  // make a the parent of b
    }

    // Find the representative (root) of the set containing i
    // Uses path compression
    int setFind (vector<int>& disset, int i) {
        // If disset[i] is negative, i is the root
        if (disset[i] < 0) {
            return i;
        }

        // Recursively find the root
        int root = setFind(disset, disset[i]);

        // Path compression: directly connect i to the root
        disset[i] = root;
        return root;
    }

    int largestComponentSize(vector<int>& nums) {
        int n = nums.size();

        // Disjoint set array:
        // negative value -> root, magnitude = size of the set
        // non-negative value -> parent index
        vector<int> disset(n, -1);

        // Map each prime factor to the first index in nums that has it
        unordered_map<int, int> factors;

        // Iterate through each number in nums
        for (int i = 0; i < n; ++i) {
            // Get unique prime factors of nums[i]
            vector<int> fac = primeFactors(nums[i]);

            // For each factor, union indices that share it
            for (int f : fac) {
                if (factors.find(f) != factors.end()) {
                    // This factor was seen before, union the components
                    setUni(disset, i, factors[f]);
                } else {
                    // First time seeing this factor, store its index
                    factors[f] = i;
                }
            }
        }

        // Find the largest connected component size
        int maxsize = 0;
        for (int size : disset) {
            // Only roots have negative values; negate to get component size
            maxsize = max(maxsize, size * -1);
        }

        return maxsize;
    }

private:
    // Return the unique prime factors of n
    vector<int> primeFactors(int n) {
        vector<int> factors;

        // Factor out all powers of 2
        if (n % 2 == 0) {
            factors.push_back(2);
            n /= 2;
            while (n % 2 == 0) {
                n /= 2;
            }
        }

        // Factor odd numbers up to sqrt(n)
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                factors.push_back(i);
                n /= i;
                while (n % i == 0) {
                    n /= i;
                }
            }
        }

        // If remaining n is a prime greater than 1
        if (n > 1) {
            factors.push_back(n);
        }

        return factors;
    }
};
