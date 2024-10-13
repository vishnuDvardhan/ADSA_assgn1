#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <numeric>
#include <iostream>

using namespace std;

int rotations = 0;
vector<int> all_rotation_insertHelperions, all_rotation_deletions;
vector<int> all_heights, all_heights_after_dels;
int n = 100;
ofstream file("rb_tree_output.txt");

struct Node
{
    int d;
    char c;
    Node *left, *right, *p;

    Node(int d)
    {
        this->d = d;
        left = right = p = nullptr;
        this->c = 'r';
    }
};

class RBTree
{
private:
    Node *root;

    void rotateL(Node *&node)
    {
        Node *rc = node->right;
        node->right = rc->left;
        if (rc->left != nullptr)
            rc->left->p = node;
        rc->p = node->p;
        if (node->p == nullptr)
            root = rc;
        else if (node == node->p->left)
            node->p->left = rc;
        else
            node->p->right = rc;
        rc->left = node;
        node->p = rc;
        rotations++;
    }

    void rotateR(Node *&node)
    {
        Node *leftChild = node->left;
        node->left = leftChild->right;
        if (leftChild->right != nullptr)
            leftChild->right->p = node;
        leftChild->p = node->p;
        if (node->p == nullptr)
            root = leftChild;
        else if (node == node->p->left)
            node->p->left = leftChild;
        else
            node->p->right = leftChild;
        leftChild->right = node;
        node->p = leftChild;
        rotations;
    }

    void fixViolation(Node *&node)
    {
        Node *p = nullptr;
        Node *gp = nullptr;

        while ((node != root) && (node->c != 'b') && (node->p->c == 'r'))
        {
            p = node->p;
            gp = p->p;

            if (p == gp->left)
            {
                Node *uncl = gp->right;
                if (uncl != nullptr && uncl->c == 'r')
                {
                    gp->c = 'r';
                    p->c = 'b';
                    uncl->c = 'b';
                    node = gp;
                }
                else
                {
                    if (node == p->right)
                    {
                        rotateL(p);
                        node = p;
                        p = node->p;
                    }
                    rotateR(gp);
                    swap(p->c, gp->c);
                    node = p;
                }
            }
            else
            {
                Node *uncl = gp->left;
                if (uncl != nullptr && uncl->c == 'r')
                {
                    gp->c = 'r';
                    p->c = 'b';
                    uncl->c = 'b';
                    node = gp;
                }
                else
                {
                    if (node == p->left)
                    {
                        rotateR(p);
                        node = p;
                        p = node->p;
                    }
                    rotateL(gp);
                    swap(p->c, gp->c);
                    node = p;
                }
            }
        }
        root->c = 'b';
    }

    void transplant(Node *u, Node *v)
    {
        if (u->p == nullptr)
            root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        if (v != nullptr)
            v->p = u->p;
    }

    Node *minimum(Node *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    void deleteFix(Node *x)
    {
        while (x != root && (x == nullptr || x->c == 'b'))
        {
            if (x == x->p->left)
            {
                Node *sibling = x->p->right;
                if (sibling->c == 'r')
                {
                    sibling->c = 'b';
                    x->p->c = 'r';
                    rotateL(x->p);
                    sibling = x->p->right;
                }
                if ((sibling->left == nullptr || sibling->left->c == 'b') &&
                    (sibling->right == nullptr || sibling->right->c == 'b'))
                {
                    sibling->c = 'r';
                    x = x->p;
                }
                else
                {
                    if (sibling->right == nullptr || sibling->right->c == 'b')
                    {
                        if (sibling->left != nullptr)
                            sibling->left->c = 'b';
                        sibling->c = 'r';
                        rotateR(sibling);
                        sibling = x->p->right;
                    }
                    sibling->c = x->p->c;
                    x->p->c = 'b';
                    if (sibling->right != nullptr)
                        sibling->right->c = 'b';
                    rotateL(x->p);
                    x = root;
                }
            }
            else
            {
                Node *sibling = x->p->left;
                if (sibling->c == 'r')
                {
                    sibling->c = 'b';
                    x->p->c = 'r';
                    rotateR(x->p);
                    sibling = x->p->left;
                }
                if ((sibling->left == nullptr || sibling->left->c == 'b') &&
                    (sibling->right == nullptr || sibling->right->c == 'b'))
                {
                    sibling->c = 'r';
                    x = x->p;
                }
                else
                {
                    if (sibling->left == nullptr || sibling->left->c == 'b')
                    {
                        if (sibling->right != nullptr)
                            sibling->right->c = 'b';
                        sibling->c = 'r';
                        rotateL(sibling);
                        sibling = x->p->left;
                    }
                    sibling->c = x->p->c;
                    x->p->c = 'b';
                    if (sibling->left != nullptr)
                        sibling->left->c = 'b';
                    rotateR(x->p);
                    x = root;
                }
            }
        }
        if (x != nullptr)
            x->c = 'b';
    }

public:
    RBTree() { root = nullptr; }

    void refresh()
    {
        root = nullptr;
    }

    void insert(const int &d)
    {
        Node *node = new Node(d);
        root = InsertHelper(root, node);
        fixViolation(node);
    }

    Node *InsertHelper(Node *root, Node *node)
    {
        if (root == nullptr)
            return node;

        if (node->d < root->d)
        {
            root->left = InsertHelper(root->left, node);
            root->left->p = root;
        }
        else if (node->d > root->d)
        {
            root->right = InsertHelper(root->right, node);
            root->right->p = root;
        }

        return root;
    }

    void inorder_printer()
    {
        inorderHelper(root);
    }

    void inorderHelper(Node *root)
    {
        if (root == nullptr)
            return;
        inorderHelper(root->left);
        file << root->d << " ";

        inorderHelper(root->right);
    }

    void delet(int d)
    {
        Node *z = root;
        Node *x = nullptr, *y = nullptr;

        while (z != nullptr && z->d != d)
        {
            if (d < z->d)
                z = z->left;
            else
                z = z->right;
        }

        if (z == nullptr)
            return;

        y = z;
        char yoc = y->c;

        if (z->left == nullptr)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nullptr)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            yoc = y->c;
            x = y->right;
            if (y->p == z)
            {
                if (x != nullptr)
                    x->p = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->c = z->c;
        }

        delete z;

        if (yoc == 'b' && x != nullptr)
        {
            deleteFix(x);
        }
    }

    int height()
    {
        return getHeightHelper(root);
    }

    int getHeightHelper(Node *node)
    {
        if (node == nullptr)
            return -1;
        int leftHeight = getHeightHelper(node->left);
        int rightHeight = getHeightHelper(node->right);
        return max(leftHeight, rightHeight) + 1;
    }
};

vector<int> array_gen(int n)
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
        for (int i = 0; i < n; ++i)
        {
            cout << "generating array of size " << size << ": " << i + 1 << "\n";
            arrays.push_back(array_gen(size));
        }
    }
    return arrays;
}

int main()
{
    RBTree rbtree;
    srand(time(0));
    vector<vector<int>> generated_arrays = generate_arrays();

    for (int i = 0; i < generated_arrays.size(); i++)
    {

        cout << "insertion in " << i << "th array\n";
        for (int j = 0; j < generated_arrays[i].size(); j++)
        {
            rbtree.insert(generated_arrays[i][j]);
        }
        all_rotation_insertHelperions.push_back(rotations);
        rotations = 0;
        cout << "\ncalculating height" << i << "th array\n";
        int height_of_tree = rbtree.height();
        all_heights.push_back(height_of_tree);
        cout << "performing 10\% deletions in " << i << "th array\n";
        for (int j = 0; j < generated_arrays[i].size() / 10; j++)
        {
            int ele = rand() % generated_arrays[i].size();
            rbtree.delet(ele);
        }
        cout << "\n";
        all_rotation_deletions.push_back(rotations);
        rotations = 0;
        all_heights_after_dels.push_back(rbtree.height());
        rbtree.refresh();
    }
    cout << "all rotations for insertions\n";
    file << "all rotations for insertions\n";
    for (int i : all_rotation_insertHelperions)
    {
        cout << i << "\n";
        file << i << "\n";
    }
    cout << "all rotations needed for deletions" << "\n";
    file << "all rotations needed for deletions" << "\n";
    for (int i : all_rotation_deletions)
    {
        cout << i << "\n";
        file << i << "\n";
    }
    cout << "all heights before deletions\n";
    file << "all heights before deletions\n";
    for (int i = 0; i < generated_arrays.size(); i++)
    {
        cout << all_heights[i] << "\n";
        file << all_heights[i] << "\n";
    }
    cout << "all heights after deletions\n";
    file << "all heights after deletions\n";
    for (int i = 0; i < generated_arrays.size(); i++)
    {
        cout << all_heights_after_dels[i] << "\n";
        file << all_heights_after_dels[i] << "\n";
    }

    return 0;
}
