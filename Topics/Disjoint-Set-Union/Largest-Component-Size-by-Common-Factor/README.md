# Largest Component Size by Common Factor
Problem link: [952. Largest Component Size by Common Factor](https://leetcode.com/problems/largest-component-size-by-common-factor/description/)

### Overview
To solve this problem, we need a way to keep track of which numbers are connected together, either directly or indirectly, as we discover common factor relationships between them. A Disjoint Set Union (DSU) data structure is designed exactly for this purpose. It maintains a collection of groups (called sets), where each element belongs to exactly one set, and it supports two fast operations:
- Find: determine which set an element belongs to
- Union: merge two sets when we learn they should be connected

In this problem, numbers become connected when they share a common factor greater than 1. Rather than building the entire graph in advance, we can start with each number in its own set and merge sets whenever we discover a shared factor. As we do this, DSU efficiently keeps track of which numbers end up in the same connected component and how large each component becomes. By the end of the process, any two numbers with a shared factor should be in the same set, and so the size of the largest set will be the answer we seek.

### Disjoint Set Overview
A disjoint set is a data structure that efficiently keeps track of a partition of a set into disjoint (non-overlapping) subsets, meaning that each element can only be in one subset. In this problem, the “elements” are the indices of nums (0 to n−1). At the start, we don’t know any connections yet, so we treat every number as its own subset. The problem states that two numbers should be connected if they share a factor > 1. So every time we discover that “index i is connected to index j”, we want to merge their subsets into one bigger subset. Thus, every subset contains all indices of numbers that are directly or indirectly connected through shared connections. In other words, every subset contains all the values in some connected component in the graph formed by the method described in the problem.

We thus hold an invariant: two indices are in the same Disjoint Set Union set **if, and only if** their values are connected in the factor graph (directly or indirectly).

### Implementing a Disjoint Set
C++ does not support a built-in disjoint set data structure, so we need to implement one ourselves. The key operations needed are `find` and `union`. Union is needed to merge sets together when we discover a connection, and find is needed to determine which set an element belongs to.

Disjoint sets are typically implemented through arrays as "Uptrees", where each array can contain many Uptrees with each tree representing a single set. It is quite similar to a tree, but instead of each element pointing to its children, each element points to its parent. The root of the tree is the representative of the set. The value of each node can be seen as its index in the array. We connect these nodes of the disjoint set to the values in nums by making it so that each index i in the disjoint set corresponds to `nums[i]`.

To continue our implementation, each value in the disjoint set array is either the index of its parent or a special value indicating that it is the root of a set (and thus has no parent). For this simple implementation, we will use the convention that if `parent[i] < 0`, then `i` is the root of its set, since no indices have value less than 0. Thus, our disjoint set will be created by ``vector<int> parent(n, -1)`` where `n` is the number of elements. As well, if we say that `parent[i] = j`, it means we are saying that the parent of element `i` is element `j`.

The find operation of a disjoint set is used to determine which set an element belongs to. Since the root of an Uptree is said to be the representative of the set, we are essentially finding, and returning, the root of the set that contains the element. If the element is not the root, we recursively call find on its parent until we reach the root. The union operation attempts to merge two sets together. To do this, we first find the roots of the sets that contain the two elements. If the roots are the same, then the two elements are already in the same set, so we simply return. If, however, the roots are different, we can merge the sets by making one root point to the other.

With that in mind, here is a simple implementation of a Disjoint Set's find/union (union also called "unite") operations in C++:

```
int find(int x) {
    if (parent[x] < 0) return x; // x is the root, parent[x] = -1
    return find(parent[x]); // otherwise, recursively look for the root
}

void unite(int a, int b) {
    a = find(a); // find root of a
    b = find(b); // find the root of b
    if (a == b) return; // already in the same set

    parent[b] = a; // make a the parent of b
}
```

However, this simplified implementation has a few weaknesses:
1. Our current implementation of unite simply points the root of the second given value to the root of the former. This can lead to unbalanced trees (trees where one side is much deeper than the other), which can degrade the performance of find operations. One way to mitigate this issue is for us to instead perform union by size, where we always attach the tree with smaller size under the root of the larger tree.
2. It's hard for us to find the sizes of the up-trees, and we would wish to have an easy way to find the sizes both for union by size and for finding the largest set of values that direct/indirectly share a common factor (as our goal in this problem). To solve this, we can try to keep track of the size of each tree at its root node. How so? Well, we said earlier that any negative value `parent[i]` will signal that the index `i` is a root. Our current implementation always uses `parent[i] = -1` to signal a root, but we don't have to as any negative number works. Taking this to our advantage, we can use `parent[i]` to store the negative size of the tree rooted at `i` if `i` is a root. For example, if `parent[3] = -5`, then `3` is the root of a tree with size 5. If `parent[4] = 3`, then `4` is not a root, and its parent is `3`. Since parent[i], if index `i` is a root, stores the negative size of the tree, "more negative", and thus smaller parent[i] value, means a larger tree.
3. As trees grow larger, and thus likely deeper, find operations can become slower (since we may have to recurse more times). To mitigate this, we can implement Path Compression, which attempts to flatten the structure of the tree whenever find is called. This is done by making each node point directly to the root before returning it whenever we traverse up the tree during a find operation.

With the above considerations, here is an improved implementation of Disjoint Set's find/union operations in C++:

```
int find(int x) {
        if (parent[x] < 0) return x; // If x is the root (has parent[x] value < 0), return it
        parent[x] = find(parent[x]);  // path compression
        return parent[x];  // return found root, which has been assigned to parent[x] above
    }

    void unite(int a, int b) {
        a = find(a); // Find root of a
        b = find(b); // Find root of b
        if (a == b) return; // already in the same set

        // union by size (attach smaller to larger)
        if (parent[a] < parent[b]) { // a's tree is larger (more negative)
          parent[a] += parent[b]; // update size to be the total of a and b's sizes
          parent[b] = a; // make a the parent of b
        } else {
            parent[b] += parent[a]; // update size to be the total of a and b's sizes
            parent[a] = b; // make b the parent of a
        }
    }
```
In summary:
- The disjoint set is initialized as `vector<int> parent(n, -1)`, where `n` is the number of elements, and each is initially part of, and is the root of, their own set of size 1.
- If for some index `i`, `parent[i] < 0`, then `i` is the root of its set, and the size of that set is `-parent[i]`.
- If `parent[i] >= 0`, then `i` is not the root, and its parent is `parent[i]`.
- The `find` operation of our disjoint set finds the root of the subset the index passed in is part of. It uses path compression to flatten the tree structure for efficiency.
- The `unite` operation of our disjoint set merges the subsets of two indices into one if they are not already in the same subset, and uses union by size to keep trees more balanced and updates the size of the resulting tree at its root.

With this implementation, we can efficiently manage disjoint sets and keep track of their sizes, which will be crucial for solving the "Largest Component Size by Common Factor" problem.

### Solving the problem
To solve the "Largest Component Size by Common Factor" problem using the Disjoint Set Union (DSU) data structure, a simple solution can follow the steps below. Essentially, the solution attempts to group numbers that share common factors greater than 1 into one set. Each set is essentially a connected component, with the largest connected component thus being the size of the largest set.
1. Initialize the Disjoint Set with n elements (where n is the size of nums)
2. For each index, `i` in nums, check it against all other numbers, `j`, later in the array to see if they share any common factors greater than 1 through ``gcd(nums[i], nums[j]) != 1``. If they do, we perform a union operation on indices `i` and `j` in the disjoint set union. Essentially, we group them together into one set.
3. After processing this for all numbers in nums, we can then determine the size of each connected component by checking the negative values stored in the DSU structure and easily return the size of the largest component using just one traversal.

And while the above is a working implementation that will achieve the correct answer, having to check gcd for every pair of numbers can be too slow, especially for larger inputs. A more optimal approach would be to factor each number and use those factors to perform unions.

Instead of asking: ``“Do nums[i] and nums[j] share a gcd > 1?”`` for every pair, we ask a cheaper question: ``“What factors (> 1) does nums[i] have, and have we seen those factors before?”``. Our invariant outlined earlier was that any two indices are in the same Disjoint Set Union set if, and only if their values are connected in the factor graph (directly or indirectly). If two numbers share a factor greater than 1, they share a direct edge/connection as stated in the problem statement. Thus, to maintain our invariant, we must union the indices `i` and `j` if `nums[i]` has some factor that `nums[j]` was found to also have, earlier on.

To implement this efficiently, we maintain a map from each factor to an index of a number that contains that factor. There is no need to store all indices associated with a given factor, since any indices encountered earlier with that factor will already have been merged into the same disjoint set subset.

To implement this idea, when we process a number `nums[i]`, we compute its (prime) factors. For each factor f:
- if we have seen f before at index j, then `nums[i]` and `nums[j]` must be connected, so we unite(i, j)
- otherwise, record that this factor now “belongs” to i

This way, each factor acts like a hub that pulls all numbers containing it into one DSU component, without ever explicitly building all pairwise edges.

The implementation of a function ``vector<int> primeFactors(int n)`` that returns a vector containing the distinct prime factors of n is as follows:

```
vector<int> primeFactors(int n) {
    vector<int> factors;

    // Factor out 2s
    if (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
        while (n % 2 == 0) {
            n /= 2;
         
    }

    // Factor odd numbers
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            factors.push_back(i);
            n /= i;
            while (n % i == 0) {
                n /= i;
            }
        }
    }

    // If n is prime and > 1
    if (n > 1) {
        factors.push_back(n);
    }

    return factors;
}
```
After processing all numbers and performing unions for every shared factor, the Disjoint Set Union structure fully captures the connected components of the factor graph. Each root in the disjoint set represents one connected component, and the negative value stored at that root gives the size of the component. A final pass over the DSU structure allows us to identify the largest such component by taking the maximum of these sizes.

With this, we have now outlined a complete solution to the Largest Component Size by Common Factor problem using Disjoint Set Union. You can see the full written solution in the Solution.cpp file under this same directory.

### Complexity Analysis
Let:
* `n` = number of elements in `nums`
* `A` = maximum value in `nums`

### 1.  Time Complexity

**1) Factoring each number**

For each `nums[i]`, we trial-divide up to `sqrt(nums[i])`, so worst-case per number is:
`O(sqrt(A))`. So factoring for all n numbers is `O(n sqrt(A))` (Dividing out repeated factors with `while (x % d == 0) x /= d;`, makes factoring faster in practice, but the worst-case bound remains as such.)

**2) Disjoint Set operations (find/union)**

Each factor for each number leads to at most one `union`, and each `union` does a constant number of `find`s. With **path compression + union by size/rank**, union and find operations are amortized: `O(a(n))` per operation, where `a` is the inverse Ackermann function (so small it’s effectively constant).

* Each number has at most `O(log(A))` **distinct prime factors**. So across all numbers, total factor events are: `O(n log(A))`

Thus the total time complexity for Disjoint Set operations (find/union) is: `O(n log(A) * a(n))`

### Overall time

The time complexity is the sum of the time for factoring and time for disjoint set operations, making it: `O(n sqrt(A) + n log(A) a(n))`

Since `sqrt(A)` grows faster than `log(A)`, the overall time complexity simplifies to:
`O(n sqrt(A))`

### 2. Space Complexity

The array used to implement our disjoint set is of size `n`, so that uses: `O(n)` space.

The hashmap that maps factors to an index can store at most one entry per distinct factor encountered. The total number of distinct prime factors across all numbers is at most `O(n log A)` in the worst case.

Thus the space used is `O(n + n log A) = O(n log A)`, but in many practical cases it’s closer to `O(n)`.