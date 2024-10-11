#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>

using namespace std;

int rotations;
vector<int> all_rotation;
vector<int> all_heights;
struct Node
{
    int data;
    char color;
    Node *left, *right, *parent;

    Node(int data)
    {
        this->data = data;
        left = right = parent = nullptr;
        this->color = 'r';
    }
};

class RBTree
{
private:
    Node *root;

    void rotateLeft(Node *&node)
    {
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
        rotations++;
    }

    void rotateRight(Node *&node)
    {
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
        rotations++;
    }

    void fixViolation(Node *&node)
    {
        Node *parent = nullptr;
        Node *grandparent = nullptr;

        while ((node != root) && (node->color != 'b') && (node->parent->color == 'r'))
        {
            parent = node->parent;
            grandparent = parent->parent;

            if (parent == grandparent->left)
            {
                Node *uncle = grandparent->right;
                if (uncle != nullptr && uncle->color == 'r')
                {
                    grandparent->color = 'r';
                    parent->color = 'b';
                    uncle->color = 'b';
                    node = grandparent;
                }
                else
                {
                    if (node == parent->right)
                    {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else
            {
                Node *uncle = grandparent->left;
                if (uncle != nullptr && uncle->color == 'r')
                {
                    grandparent->color = 'r';
                    parent->color = 'b';
                    uncle->color = 'b';
                    node = grandparent;
                }
                else
                {
                    if (node == parent->left)
                    {
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
    void refresh()
    {
        root = nullptr;
    }
    RBTree() { root = nullptr; }

    void insert(const int &data)
    {
        Node *node = new Node(data);
        root = Insert(root, node);
        fixViolation(node);
    }

    Node *Insert(Node *root, Node *node)
    {
        if (root == nullptr)
            return node;

        if (node->data < root->data)
        {
            root->left = Insert(root->left, node);
            root->left->parent = root;
        }
        else if (node->data > root->data)
        {
            root->right = Insert(root->right, node);
            root->right->parent = root;
        }

        return root;
    }

    void inorder() { inorderHelper(root); }

    void inorderHelper(Node *root)
    {
        if (root == nullptr)
            return;

        inorderHelper(root->left);
        cout << root->data << " ";
        inorderHelper(root->right);
    }

    int height() { return heightHelper(root); }

    int heightHelper(Node *node)
    {
        if (node == nullptr)
            return 0;

        int leftHeight = heightHelper(node->left);
        int rightHeight = heightHelper(node->right);

        return 1 + max(leftHeight, rightHeight);
    }
};

vector<int> generate_array(int n)
{
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 1);

    random_device rd;
    default_random_engine rng(rd());

    shuffle(arr.begin(), arr.end(), rng);

    return arr;
}

vector<vector<int>> generate_arrays()
{
    vector<vector<int>> arrays;
    vector<int> sizes = {10000, 100000, 1000000, 10000000};

    for (int size : sizes)
    {
        for (int i = 0; i < 100; ++i)
        {
            cout << "generating array of size " << size << ": " << i + 1 << "\n";
            arrays.push_back(generate_array(size));
        }
    }
    return arrays;
}

int main()
{
    RBTree rbtree;
    ofstream file("rb_tree_output.txt");

    vector<vector<int>> generated_arrays = generate_arrays();

        for (int i = 0; i < generated_arrays.size(); i++)
    {

        cout << "insertion in " << i << "th array\n";
        for (int j = 0; j < generated_arrays[i].size(); j++)
        {
            rbtree.insert(generated_arrays[i][j]);
        }
        all_rotation.push_back(rotations);
        rotations = 0;
        cout << "\ncalculating height" << i << "th array\n";
        int height_of_tree = rbtree.height();
        all_heights.push_back(height_of_tree);
        rbtree.refresh();
    }
    cout << "all rotations\n";
    file << "all_rotations\n";
    for (int i : all_rotation)
    {
        cout << i << "\n";
        file << i << "\n";
    }
    cout << "\n";
    file << "\n";
    cout << "all heights\n";
    file << "all heights\n";
    for (int i = 0; i < generated_arrays.size(); i++)
    {
        cout << all_heights[i] << "\n";
        file << all_heights[i] << "\n";
    }

    return 0;
}
