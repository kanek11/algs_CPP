
#include "m_pair.h"
#include "m_AVLTree.h"

#include <iostream>
#include <string>

using namespace m_std;

int main()
{
    pair<std::string, int> p1("apple", 1);
    pair<std::string, int> p2("banana", 2);
    swap(p1, p2);
    std::cout << p1.first << " " << p1.second << std::endl;

    AVLTree<int, int> bst;

    bst.insert(10, 10);
    bst.insert(5, 5);
    bst.insert(25, 25);
    bst.insert(15, 15);
    // bst.insert(20, 20);
    // bst.insert(30, 30);
    // bst.insert(31, 31);
    // bst.insert(32, 32);

    std::cout << "traverse prder: " << std::endl;
    bst.traverse();

    bst.print();
    std::cout << "height: " << bst.height() << std::endl;

    auto _node = bst.find(5);
    if (_node != nullptr)
        std::cout << "to delete: " << _node->value << std::endl;
    bst.erase(_node);

    bst.print();
    std::cout << "height: " << bst.height() << std::endl;

    std::cout << "iterate through the tree: \n";
    // iterator-like traversal
    // auto _it  = bst.begin();
    // auto _end = bst.end();

    std::cout << "KEY VALUE \n";
    for (auto _it = bst.begin(); _it != bst.end(); ++_it)
    {
        auto key = _it->first;
        auto val = _it->second;
        std::cout << key << " " << val << std::endl;
    }

    return 0;
}