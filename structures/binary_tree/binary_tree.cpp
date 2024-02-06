//
// Created by maria on 04.02.2024.
//

#include "node.cpp"
#include <queue>

class binary_tree {
private:
    Node* root_node{};
public:
    explicit binary_tree(int data) {
        root_node = new Node(data);
    }

    Node* get_root() {
        return root_node;
    }

    Node* insert(Node* root, int item) {
        Node* new_node;
        if (root == NULL) {
            new_node = new Node(item);
            return new_node;
        }

        if (item > root->data) {
            // shift the smaller value to the left
            root->left = insert(root->left, root->data);
            root->data = item;
        } else if (item < root->data) {
            root->right = insert(root->right, root->data);
            root->data = item;
            // instead, call insert with the current node and the former item of it and after the insertion replace it with the new value.
        } else {
            root->left = insert(root->left, item);
        }

        return new_node;
    }

    void remove(int item) {

    }

    Node* search(Node* start, int item, int mode) {
        // TFS:
        // Preorder Traversal (current-left-right) Visit the current node before visiting any nodes inside the left or right subtrees. Here, the traversal is
        //      root – left child – right child. It means that the root node is traversed first then its left child and finally the right child.

        // Inorder Traversal (left-current-right) Visit the current node after visiting all nodes inside the left subtree but before visiting any node within the
        //      right subtree. Here, the traversal is left child – root – right child.  It means that the left child is traversed first then its root node and finally the right child.

        // Postorder Traversal (left-right-current) Visit the current node after visiting all the nodes of the left and right subtrees.  Here, the traversal is
        //      left child – right child – root.  It means that the left child has traversed first then the right child and finally its root node.

        // BFS:
        // Level Order Traversal Visit nodes level-by-level and left-to-right fashion at the same level. Here, the traversal is level-wise.
        //      It means that the most left child has traversed first and then the other children of the same level from left to right have traversed.

        return nullptr;
    };

    int get_height();
    int get_level();
    int get_size();
};