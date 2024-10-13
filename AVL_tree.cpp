#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>

using namespace std;
vector<int> all_rotations, all_heights;
int rotations = 0;

struct Node
{
    int key;
    Node *left;
    Node *right;
    int height;

    Node(int val) : key(val), left(nullptr), right(nullptr), height(1) {}
};

int height(Node *node)
{
    if (!node)
        return 0;
    return node->height;
}

int get_balance(Node *node)
{
    if (!node)
        return 0;
    return height(node->left) - height(node->right);
}

Node *right_rotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    rotations++;
    return x;
}

Node *left_rotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    rotations++;
    return y;
}

Node *insert(Node *node, int key)
{
    if (!node)
        return new Node(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    if (balance > 1 && key < node->left->key)
        return right_rotate(node);

    if (balance < -1 && key > node->right->key)
        return left_rotate(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

Node *min_value_node(Node *node)
{
    Node *current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node *delete_node(Node *root, int key)
{
    if (!root)
        return root;

    if (key < root->key)
        root->left = delete_node(root->left, key);
    else if (key > root->key)
        root->right = delete_node(root->right, key);
    else
    {
        if (!root->left || !root->right)
        {
            Node *temp = root->left ? root->left : root->right;

            if (!temp)
            {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;

            delete temp;
        }
        else
        {
            Node *temp = min_value_node(root->right);
            root->key = temp->key;
            root->right = delete_node(root->right, temp->key);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && get_balance(root->left) < 0)
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);

    if (balance < -1 && get_balance(root->right) > 0)
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void in_order(Node *root)
{
    if (root)
    {
        in_order(root->left);
        cout << root->key << " ";
        in_order(root->right);
    }
}

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
    Node *root = nullptr;
    vector<vector<int>> arrays = generate_arrays();
    ofstream file("avl_tree_outpur.txt");

    for (int i = 0; i < arrays.size(); ++i)
    {
        for (int j = 0; j < arrays[i].size(); ++j)
        {
            root = insert(root, arrays[i][j]);
        }
        cout << "inserting array of size " << arrays[i].size() << ": " << i << " \n";
        all_rotations.push_back(rotations);
        rotations = 0;
        all_heights.push_back(height(root));
        root = nullptr;
    }
    cout << "all rotations" << "\n";
    file << "all rotations" << "\n";

    for (int i = 0; i < all_rotations.size(); ++i)
    {
        cout << all_rotations[i] << " \n";
        file << all_rotations[i] << " \n";
    }
    cout << "all heights" << "\n";
    file << "all heights" << "\n";
    for (int i = 0; i < all_heights.size(); ++i)
    {
        cout << all_heights[i] << " \n";
        file << all_heights[i] << " \n";
    }

    return 0;
}
