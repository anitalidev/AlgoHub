class Solution {
public:
    Node* connect(Node* root) {
        if (!root) {
            return root;
        }
        Node* head = root;
        while(head) {
            Node* prev = nullptr;
            Node* curr = head;
            head = head->left;
            while (curr) {
                if (prev) prev->next = curr->left;
                if (curr->left) curr->left->next = curr->right;
                if (curr->right) prev = curr->right;

                curr = curr->next;
            }
        }
        return root;
    }
};