/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* root) {
        if (!root) { // If there are no nodes in the tree, return nullptr
            return nullptr;
        }
        Node* head = root; // Start with the root node (first level)

        while(head) { // While there are more levels to process
            Node* prev = nullptr; // Previous node on the next level, starts nullptr as there are no nodes to the left
            Node* curr = head; // Current node to process on the current level
            head = head->left; // Move head to the leftmost node of the next level
            while (curr) { // Process all nodes on the current level
                if (prev) prev->next = curr->left; // Link previous node's next to current node's left child
                if (curr->left) curr->left->next = curr->right; // Link current node's left child to its right child
                if (curr->right) prev = curr->right; // Move prev to current node's right child if it exists

                curr = curr->next; // Move to the next node on the current level
            }
        }
        return root; // Return the root of the original tree with next pointers populated
    }
};