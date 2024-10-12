#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>

using namespace std;

int rotations;
vector<int> all_rotation_insertions,all_rotation_deletions;
vector<int> all_heights,all_heights_after_dels;
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

    void transplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    Node* minimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void deleteFix(Node *x) {
        while (x != root && x->color == 'b') {
            if (x == x->parent->left) {
                Node *s = x->parent->right;
                if (s->color == 'r') {
                    s->color = 'b';
                    x->parent->color = 'r';
                    rotateLeft(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == 'b' && s->right->color == 'b') {
                    s->color = 'r';
                    x = x->parent;
                } else {
                    if (s->right->color == 'b') {
                        s->left->color = 'b';
                        s->color = 'r';
                        rotateRight(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 'b';
                    s->right->color = 'b';
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node *s = x->parent->left;
                if (s->color == 'r') {
                    s->color = 'b';
                    x->parent->color = 'r';
                    rotateRight(x->parent);
                    s = x->parent->left;
                }
                if (s->left->color == 'b' && s->right->color == 'b') {
                    s->color = 'r';
                    x = x->parent;
                } else {
                    if (s->left->color == 'b') {
                        s->right->color = 'b';
                        s->color = 'r';
                        rotateLeft(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = 'b';
                    s->left->color = 'b';
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = 'b';
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

    void delet(int data)
    {
        Node* z = root;
        Node* x, *y;

        while (z != nullptr && z->data != data) {
            if (data < z->data) z = z->left;
            else z = z->right;
        }

        if (z == nullptr) return;

        y = z;
        char yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (yOriginalColor == 'b') {
            deleteFix(x);
        }
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
    srand(time(0));
    vector<vector<int>> generated_arrays = generate_arrays();

    for (int i = 0; i < generated_arrays.size(); i++)
    {

        cout << "insertion in " << i << "th array\n";
        for (int j = 0; j < generated_arrays[i].size(); j++)
        {
            rbtree.insert(generated_arrays[i][j]);
        }
        all_rotation_insertions.push_back(rotations);
        rotations = 0;
        cout << "\ncalculating height" << i << "th array\n";
        int height_of_tree = rbtree.height();
        all_heights.push_back(height_of_tree);
        cout << "performing 10% deletions in " << i << "th array\n";
        for (int j = 0; j < generated_arrays[i].size()/10; j++) {
            rbtree.delet(rand() % generated_arrays[i].size());
        }
        all_rotation_deletions.push_back(rotations);
        rotations = 0;
        all_heights_after_dels.push_back(rbtree.height());
        rbtree.refresh();
    }
    cout << "all rotations for insertions\n";
    file << "all_rotations for insertions\n";
    for (int i : all_rotation_insertions)
    {
        cout << i << "\n";
        file << i << "\n";
    }
    cout << "\n";
    file << "\n";
    cout << "all heights before deletions\n";
    file << "all heights before deletions\n";
    for (int i = 0; i < generated_arrays.size(); i++)
    {
        cout << all_heights[i] << "\n";
        file << all_heights[i] << "\n";
    }
    cout << "\n";
    file << "\n";
    cout << "all heights after deletions\n";
    file << "all heights after deletions\n";
    for (int i = 0; i < generated_arrays.size(); i++)
    {
        cout << all_heights_after_dels[i] << "\n";
        file << all_heights_after_dels[i] << "\n";
    }
    cout << "\n";
    file << "\n";
    cout << "all rotations needed for deletions" << "\n";
    file << "all rotations needed for deletions" << "\n";

    return 0;
}
