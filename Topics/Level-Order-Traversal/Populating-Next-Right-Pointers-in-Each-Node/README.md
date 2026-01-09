# Populating Next Right Pointers in Each Node
Problem link: [116. Populating Next Right Pointers in Each Node](https://leetcode.com/problems/populating-next-right-pointers-in-each-node/)\

### Overview
One key thing to identify about this problem is that it involves a level-order traversal of a binary tree where we traverse each level one-by-one from up to down, left to right. By traversing the tree in this way, we can work level by level to traverse our nodes left to right, connecting the `next` pointers of each node to its right neighbor (if it exists).

From this initial idea, we will notice that there is a very interesting optimization we can make to achieve O(1) space complexity.

### Level-Order Traversal
A goal of a level-order traversal is to process a tree one level at a time, from top to bottom and left to right.

The simplest form of level-order traversal uses a queue to track which nodes should be processed next. Parent nodes enqueue their children after being processed, and because the queue processes nodes in first-in, first-out order, this ensures that all nodes at a given level are processed from left to right before moving on to the next level.

The basic outline of how a level-order traversal for a binary tree (meaning each parent has at most two children "left" and "right"), using a queue works is as follows:
1. Start by putting the root node into the queue. 
2. While the queue is not empty:
   - Remove the front node from the queue and process it. 
   - Add its left child to the queue (if it exists).
   - Add its right child to the queue (if it exists).

One example of this simple implementation is as follows
```c++
// Initialize the queue with the root node
queue<TreeNode*> q;
q.push(root);

while (!q.empty()) {
    // Get next node to process
    TreeNode* curr = q.front();
    q.pop();

    // Process current node
    proccess(curr);

    // Add children to the queue
    if (curr->left)  q.push(curr->left);
    if (curr->right) q.push(curr->right);
}
```

An additional adaptation allows us **to perform some operation/s just before fully traversing every level**. The adaptation is as follows: At the beginning of each iteration of the while loop, we determine how many nodes belong to the current level by recording the queue’s size. We then process exactly that many nodes using a for loop before performing some operations we would like to perform, and advancing to the next level.

An example of this implementation is as follows:
```c++
// Initialize the queue with the root node
queue<TreeNode*> q;
q.push(root);

// While there is at least one node in the next level to process
while (!q.empty()) {
        // Here, we can write code to perform operations we want to have happen just before processing through the new level

    int levelSize = q.size();  // number of nodes at the current level

    // Process exactly the nodes belonging to this level
    for (int i = 0; i < levelSize; ++i) {
        // Find current node to process
        TreeNode* curr = q.front();
        q.pop();
        
        // Proccess current node
        process(curr, level);

        // Enqueue children for the next level
        if (curr->left)  q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
}
```

### Populating Next Right Pointers
**Building off of the above, we can use the fact that we process nodes level-by-level and left to right to connect each node’s `next` pointer to its immediate neighbor on the same level.** During the inner `for` loop, we maintain a `prev` pointer to the previously processed node in the current level. Each time we pop a new node `curr`, we process it by setting `prev->next = curr` (when `prev` exists), then updating `prev = curr`. In this case, what we want to have happen before processing each level is simply to reset `prev` to `nullptr` so that the last node of the previous level does not get connected to the first node of the next. This works to guarantees that we connect, and only connect, nodes within the same level. 

The below is an implementation of this solution idea:

```c++
Node* connect(Node* root) { // Function that takes in the root of a binary tree, 
                            // connects each of it's node's next pointer to its right neighbour, 
                            // then returns the root
    if (!root) return root; // trivial case where tree is empty
    
    queue<Node*> toTraverse; // queue to facilitate level-order traversal. Initialized with root node
    toTraverse.push(root);

    while(!toTraverse.empty()) { // While there are still nodes in the next level to process
        Node* prev = nullptr; // Pointer to previously processed node in the current level, reset
                              // at the start of each level so that we do not connect across levels
        int levelSize = toTraverse.size(); // number of nodes at the current level
        
        for (int i = 0; i < levelSize; ++i) { // Process exactly the nodes belonging to this level
            
            // Get current node to process
            Node* curr = toTraverse.front(); 
            toTraverse.pop();
            
            // Add children to the queue for the next level if they exist
            if (curr->left) toTraverse.push(curr->left);
            if (curr->right) toTraverse.push(curr->right);
            
            // Connect previous node's next pointer to current node if previous exists
            if (prev) prev->next = curr;
            prev = curr;
        }
    }
    return root;
}
```
#### Analyzing the Current Solution
The current solution idea, implemented, above, is a correct one. As well, it runs quite quickly, in O(n) time, where n is the number of nodes in the tree. This is because we visit each node exactly once throughout our level-order traversal. There is no way to optimize this, since to connect each node's `next` pointer, we must visit each node at least once.

The space complexity of this solution is O(w), where w is the maximum width of the tree (the maximum number of nodes at any level). This is because, in the worst case, our queue will need to store all nodes at the current level before we process them. In a perfect binary tree, w can be as large as n/2, leading to O(n) space complexity in the worst case.

However, we can optimize this solution to achieve O(1) space complexity by leveraging how the `next` pointers are being set during the traversal. Once the next pointers for a given level are established, they can be reused to traverse that level directly, eliminating the need for an auxiliary queue.

To make this optimization, we require three main changes to our current solution:
1. Instead of using a queue to keep track of nodes at the current level, we will use the `next` pointers that we are setting to traverse the current level.
2. We will maintain pointers to the first node of the next level and the previous node at the next level to facilitate the connections.
3. Since we will need to have the next pointers already set before we traverse a level, this means that instead of making the connections for a level as we process it, we will instead need to have made the connections for the level while processing the previous level.

### Optimized O(1) Space Solution
