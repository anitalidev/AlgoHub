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
        if (!root) return nullptr; // if it's an empty tree, immediately return. No nodes to make next connections for

        // levelStart marks the beginning (leftmost node) of the level we are "standing on"
        Node* levelStart = root;

        // We stop when we reach the last level, because there are no children below to connect.
        // If the leftmost node has no left child, since this is a perfect binary tree, it means we've reached the last level.
        while (levelStart->left != nullptr) {

            // curr "slides" across the current level like a linked list
            Node* curr = levelStart;

            while (curr != nullptr) {
                // STITCH 1, Internal: Connect children of the same parent
                curr->left->next = curr->right;

                // STITCH 2, External: Connect children of adjacent parents
                // We use the existing 'next' pointer of the parent to bridge the gap
                if (curr->next != nullptr) {
                    curr->right->next = curr->next->left;
                }

                // Slide horizontally to the next node on the current level
                curr = curr->next;
            }

            // The level below is now fully stitched. Drop down to start the next pass.
            levelStart = levelStart->left;
        }

        return root;
    }
};