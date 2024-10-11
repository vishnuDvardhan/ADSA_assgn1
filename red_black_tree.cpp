#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <numeric>

using namespace std;

struct Node {
    int data;
    char color;
    Node *left, *right, *parent;

    Node(int data) {
        this->data = data;
        left = right = parent = nullptr;
        this->color = 'r';
    }
};

class RBTree {
private:
    Node *root;

protected:
    void rotateLeft(Node *&node) {
        Node *rightChild = node->right;
        node->right = rightChild->left;
        if (node->right != nullptr)
            node->right->parent = node;
        rightChild->parent = node->parent;
        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;
        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node *&node) {
        Node *leftChild = node->left;
        node->left = leftChild->right;
        if (node->left != nullptr)
            node->left->parent = node;
        leftChild->parent = node->parent;
        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->left)
            node->parent->left = leftChild;
        else
            node->parent->right = leftChild;
        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixViolation(Node *&node) {
        Node *parent = nullptr;
        Node *grandparent = nullptr;

        while ((node != root) && (node->color != 'b') && (node->parent->color == 'r')) {
            parent = node->parent;
            grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node *uncle = grandparent->right;
                if (uncle != nullptr && uncle->color == 'r') {
                    grandparent->color = 'r';
                    parent->color = 'b';
                    uncle->color = 'b';
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            } else {
                Node *uncle = grandparent->left;
                if (uncle != nullptr && uncle->color == 'r') {
                    grandparent->color = 'r';
                    parent->color = 'b';
                    uncle->color = 'b';
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = 'b';
    }

public:
    RBTree() { root = nullptr; }

    void insert(const int &data) {
        Node *node = new Node(data);
        root = bstInsert(root, node);
        fixViolation(node);
    }

    Node* bstInsert(Node* root, Node *node) {
        if (root == nullptr)
            return node;

        if (node->data < root->data) {
            root->left = bstInsert(root->left, node);
            root->left->parent = root;
        } else if (node->data > root->data) {
            root->right = bstInsert(root->right, node);
            root->right->parent = root;
        }

        return root;
    }

    void inorder() { inorderHelper(root); }

    void inorderHelper(Node *root) {
        if (root == nullptr)
            return;

        inorderHelper(root->left);
        cout << root->data << " ";
        inorderHelper(root->right);
    }

    int height() { return heightHelper(root); }

    int heightHelper(Node *node) {
        if (node == nullptr)
            return 0;

        int leftHeight = heightHelper(node->left);
        int rightHeight = heightHelper(node->right);

        return 1 + max(leftHeight, rightHeight);
    }
};

vector<int> generate_array(int n) {
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 1);

    random_device rd;
    default_random_engine rng(rd());

    shuffle(arr.begin(), arr.end(), rng);

    return arr;
}

int main() {
    RBTree rbtree ;

    vector<int> arr = generate_array(10000);

    for (int i = 0; i < arr.size(); i++)
    {
        rbtree.insert(arr[i]);
    }
    cout << rbtree.height()<< endl;


    return 0;
}
