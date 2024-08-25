#pragma once

#include "m_pair.h"
#include <iostream>
#include <queue>
namespace m_std
{

template <typename Key_t, typename Value_t>
struct AVLNode
{
public:
    using pair_type = pair<Key_t, Value_t>;
    using node_type = AVLNode<Key_t, Value_t>;

    ~AVLNode() = default;
    AVLNode()  = delete; // must have a key and value
    template <typename Key_t, typename Value_t>
    AVLNode(Key_t&& k, Value_t&& v) :
        kv_pair(std::forward<Key_t>(k), std::forward<Value_t>(v))
    {
    }

    int getBalanceFactor() const
    {
        int leftHeight  = (left ? left->height : -1);
        int rightHeight = (right ? right->height : -1);

        return leftHeight - rightHeight;
    }

    // C++ impl wont replace the value for same key;
    pair<Key_t, Value_t> kv_pair;
    Key_t&               key   = kv_pair.first;
    Value_t&             value = kv_pair.second;

    int height = 0;

    node_type* left   = nullptr;
    node_type* right  = nullptr;
    node_type* parent = nullptr;
};

// iterator ===========

template <typename Key_t, typename Value_t>
class AVLIterator
{
public:
    using node_type = typename AVLNode<Key_t, Value_t>::node_type;
    using pair_type = typename AVLNode<Key_t, Value_t>::pair_type;

    AVLIterator() = default;
    AVLIterator(node_type* node) :
        m_current(node) { }

    // prefix increment
    AVLIterator& operator++()
    {
        m_current = successor(m_current);
        return *this;
    }

    // post increment
    AVLIterator operator++(int)
    {
        AVLIterator _tmp = *this;
        m_current        = successor(m_current);
        return _tmp;
    }

    friend bool operator==(const AVLIterator& thisIter, const AVLIterator& other)
    {
        return thisIter.m_current == other.m_current;
    }

    friend bool operator!=(const AVLIterator& thisIter, const AVLIterator& other)
    {
        return !(thisIter == other); // it's better to delegate to operator==
    }

    // deref directly gives the pair
    pair_type& operator*() const
    {
        return m_current->kv_pair;
    }

    pair_type* operator->() const
    {
        return &this->operator*();
    }

    node_type* successor(node_type* thisNode) const
    {
        if (thisNode == nullptr)
        {
            return nullptr;
        }

        if (thisNode->right != nullptr)
        {
            return minimum(thisNode->right);
        }

        // go up until as a left child
        auto _currNode = thisNode;
        while (_currNode->parent
               && _currNode != _currNode->parent->left)
        {
            _currNode = _currNode->parent;
        }

        // if it hits the root, or thisNode is a left child of the successor we want;
        return _currNode->parent;
    }

    node_type* minimum(node_type* thisNode) const
    {
        node_type* _curr_node = thisNode;

        while (_curr_node->left != nullptr)
        {
            _curr_node = _curr_node->left;
        }

        return _curr_node;
    }

private:
    node_type* m_current = nullptr;
};

//================================================================================================
template <typename Key_t, typename Value_t>
class AVLTree
{
public:
    using pair_type = typename AVLNode<Key_t, Value_t>::pair_type;
    using Node_type = typename AVLNode<Key_t, Value_t>::node_type;

public:
    AVLTree() = default;
    ~AVLTree()
    {
        // delete nodes by post-order traversal
        deleteNode(m_root);
    }

    void deleteNode(Node_type* thisNode)
    {
        if (thisNode == nullptr)
        {
            return;
        }

        deleteNode(thisNode->left);
        deleteNode(thisNode->right);

        delete thisNode;
    }

    Node_type* insert(const Key_t& key, const Value_t& value)
    {
        Node_type* _parent    = nullptr;
        Node_type* _curr_node = m_root;

        while (_curr_node != nullptr)
        {
            _parent = _curr_node;
            if (_curr_node->key > key)
            {
                _curr_node = _curr_node->left;
            }
            else if (_curr_node->key < key)
            {
                _curr_node = _curr_node->right;
            }
            else
            {
                return _curr_node;
            }
        }

        // if _curr_node is nullptr£» insert new;
        // be careful when assign to a local ptr

        std::cout << "inserted: " << value << std::endl;
        Node_type* _newNode = new Node_type(key, value);
        _newNode->parent    = _parent;

        // root case  cant access to member
        if (_parent == nullptr)
        {
            m_root = _newNode;
        }
        else if (_parent->key > key)
        {
            _parent->left = _newNode;
        }
        else if (_parent->key < key)
        {
            _parent->right = _newNode;
        }

        updateHeightAndBalanceUpwards(_parent);

        return _newNode;
    }

private:
    void updateHeightAndBalanceUpwards(Node_type* thisNode)
    {
        auto _curr_node = thisNode;

        while (_curr_node != nullptr)
        {
            _curr_node->height = 1 + std::max((_curr_node->left ? _curr_node->left->height : 0), (_curr_node->right ? _curr_node->right->height : 0));

            int balanceFactor = _curr_node->getBalanceFactor();

            // debug:
            if (balanceFactor > 1 || balanceFactor < -1)
            {
                std::cout << "balance happens at node: " << _curr_node->key << std::endl;
            }
            // four cases;
            if (balanceFactor > 1)
            {
                // L
                if (_curr_node->left->getBalanceFactor() > 0)
                {
                    _curr_node = right_rotate(_curr_node);
                }
                // LR
                else
                {
                    _curr_node->left = left_rotate(_curr_node->left);
                    _curr_node       = right_rotate(_curr_node);
                }
            }
            else if (balanceFactor < -1)
            {
                // RR
                if (_curr_node->right->getBalanceFactor() < 0)
                {
                    _curr_node = left_rotate(_curr_node);
                }
                // RL
                else
                {
                    _curr_node->right = right_rotate(_curr_node->right);
                    _curr_node        = left_rotate(_curr_node);
                }
            }

            _curr_node = _curr_node->parent;
        }
    }

    Node_type* left_rotate(Node_type* pivot)
    {
        if (pivot == nullptr)
        {
            return nullptr;
        }

        auto _right  = pivot->right;
        auto _parent = pivot->parent;

        // std::cout << "thisNode: " << pivot->value << std::endl;
        // std::cout << "pivot right: " << (_right ? _right->value : -1) << std::endl;
        // std::cout << "pivot parent: " << (_parent ? _parent->value : -1) << std::endl;

        pivot->right = _right->left;
        if (pivot->right)
        {
            pivot->right->parent = pivot;
        }

        _right->left  = pivot;
        pivot->parent = _right;

        _right->parent = _parent;
        if (_parent)
        {
            if (_parent->left == pivot)
            {
                _parent->left = _right;
            }
            else
            {
                _parent->right = _right;
            }
        }
        else
        {
            m_root = _right;
        }

        updateHeight(pivot);
        updateHeight(_right);

        return _right;
    }

    Node_type* right_rotate(Node_type* pivot)
    {
        if (pivot == nullptr)
        {
            return nullptr;
        }
        auto _left   = pivot->left;
        auto _parent = pivot->parent;

        pivot->left = _left->right;
        if (pivot->left)
        {
            pivot->left->parent = pivot;
        }

        _left->right  = pivot;
        pivot->parent = _left;

        _left->parent = _parent;
        if (_parent)
        {
            if (_parent->left == pivot)
            {
                _parent->left = _left;
            }
            else
            {
                _parent->right = _left;
            }
        }
        else
        {
            m_root = _left;
        }

        updateHeight(pivot);
        updateHeight(_left);

        return _left;
    }

private:
    void updateHeight(Node_type* thisNode)
    {
        if (thisNode == nullptr)
        {
            return;
        }

        if (thisNode->left == nullptr && thisNode->right == nullptr)
        {
            thisNode->height = 0;
            return;
        }
        else
        {
            thisNode->height = 1 + std::max((thisNode->left ? thisNode->left->height : 0), (thisNode->right ? thisNode->right->height : 0));
            return;
        }
    }

public:
    void erase(Node_type* node)
    {
        if (node == nullptr)
        {
            return;
        }

        auto _parent_of_deleted = node->parent;

        // 2x2 = 4 cases
        // case 1
        if ((node->left == nullptr) && (node->right == nullptr))
        {
            if (node->parent)
            {
                if (node->parent->left == node)
                {
                    node->parent->left = nullptr;
                }
                else if (node->parent->right == node)
                {
                    node->parent->right = nullptr;
                }
            }
            delete node;
        }
        // case 2
        else if ((node->left == nullptr) && (node->right != nullptr))
        {
            transplant(node, node->right);
        }
        else if ((node->left != nullptr) && (node->right == nullptr))
        {
            transplant(node, node->left);
        }
        // case 3
        else if ((node->left != nullptr) && (node->right != nullptr))
        {
            auto _successor = minimum(node->right);

            swap(node->kv_pair, _successor->kv_pair);
            erase(_successor);
        }

        updateHeightAndBalanceUpwards(_parent_of_deleted);
    }

private:
    void transplant(Node_type* thisNode, Node_type* child)
    {
        // check either is nullptr
        if (thisNode == nullptr || child == nullptr)
        {
            return;
        }

        auto _grandParent = thisNode->parent;
        if (_grandParent == nullptr)
        {
            m_root = child;
        }
        else if (_grandParent->right == thisNode)
        {
            _grandParent->right = child;
        }
        else if (_grandParent->left == thisNode)
        {
            _grandParent->left = child;
        }
        child->parent = _grandParent;

        delete thisNode;
    }

public:
    // new : iterative version
    // the C++ impl wont replace for same key;
    Node_type* find(int key)
    {
        Node_type* _curr_node = m_root;

        while (_curr_node != nullptr)
        {
            if (_curr_node->key > key)
            {
                _curr_node = _curr_node->left;
            }
            else if (_curr_node->key < key)
            {
                _curr_node = _curr_node->right;
            }
            else
            {
                return _curr_node;
            }
        }

        return nullptr;
    }

public:
    void traverse()
    {
        traverseNode(m_root);
    }

    void traverseNode(Node_type* thisNode)
    {
        if (thisNode == nullptr) return;

        traverseNode(thisNode->left);

        std::cout << thisNode->value << std::endl;

        traverseNode(thisNode->right);
    }

    // print it in a tree-like structure by BFS
    void print()
    {
        printBFS(m_root);
    }

    void printBFS(Node_type* root)
    {
        if (root == nullptr) return;

        std::queue<std::pair<Node_type*, int>> q;
        q.push({ root, 0 });
        int currentLevel = 0;

        while (!q.empty())
        {
            auto nodePair = q.front();
            q.pop();
            Node_type* currentNode = nodePair.first;
            int        level       = nodePair.second;

            if (level != currentLevel)
            {
                std::cout << std::endl;
                currentLevel = level;
            }

            std::cout << "(" << currentNode->key << "," << currentNode->value << ") ";

            if (currentNode->left)
            {
                q.push({ currentNode->left, level + 1 });
            }

            if (currentNode->right)
            {
                q.push({ currentNode->right, level + 1 });
            }
        }

        std::cout << std::endl;
    }

    int height()
    {
        return m_root->height;
    }

public:
    Node_type* minimum()
    {
        return minimum(m_root);
    }

    Node_type* minimum(Node_type* thisNode)
    {
        Node_type* _curr_node = thisNode;

        while (_curr_node->left != nullptr)
        {
            _curr_node = _curr_node->left;
        }

        return _curr_node;
    }

    Node_type* maximum()
    {
        return maximum(m_root);
    }

    Node_type* maximum(Node_type* thisNode)
    {
        Node_type* _curr_node = thisNode;

        while (_curr_node->right != nullptr)
        {
            _curr_node = _curr_node->right;
        }

        return _curr_node;
    }

    // iterator
public:
    using iterator = AVLIterator<Key_t, Value_t>;

    iterator begin()
    {
        return iterator(this->minimum());
    }

    iterator end()
    {
        return iterator(nullptr);
    }

private:
    Node_type* m_root = nullptr;
};

} // namespace m_std