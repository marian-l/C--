//
// Created by maria on 04.02.2024.
//

struct node {
    int data;
    struct node* left;
    struct node* right;
};

class Node {
public:
    int data;
    struct Node* left;
    struct Node* right;

    explicit Node(int val) {
        data = val;

        left = nullptr;
        right = nullptr;
    }

};


