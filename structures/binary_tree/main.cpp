#include <iostream>
#include "binary_tree.cpp"
// each node has at least two children. These are pointers to the other nodes, or at leaf-level, values. also it might itself hold a value

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto* btree = new binary_tree(5);

    btree->insert(btree->get_root(), 3);
    btree->insert(btree->get_root(), 4);
    btree->insert(btree->get_root(), 5);
    btree->insert(btree->get_root(), 6);
    btree->insert(btree->get_root(), 7);

    std::cout << "Hello, World!" << std::endl;

    return 0;
}
