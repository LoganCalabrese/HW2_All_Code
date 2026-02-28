#include "Treap.h"
#include <limits>

Treap::Treap(unsigned int seed)
    : root(nullptr),
    nodeCount(0),
    rng(seed),
    prioDist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())
{
}

Treap::~Treap() {
    destroy(root);
    root = nullptr;
    nodeCount = 0;
}

void Treap::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

bool Treap::find(int key) const {
    return find(root, key);
}

bool Treap::find(Node* node, int key) {
    while (node) {
        if (key == node->key) return true;
        node = (key < node->key) ? node->left : node->right;
    }
    return false;
}

Treap::Node* Treap::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

Treap::Node* Treap::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

bool Treap::insert(int key) {
    bool inserted = false;
    root = insert(root, key, inserted);
    if (inserted) nodeCount++;
    return inserted;
}

// Max-heap on priority: higher priority bubbles up toward the root.
Treap::Node* Treap::insert(Node* node, int key, bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node(key, prioDist(rng));
    }

    if (key == node->key) {
        inserted = false;
        return node; // no duplicates
    }

    if (key < node->key) {
        node->left = insert(node->left, key, inserted);

        // Fix heap property
        if (node->left && node->left->priority > node->priority) {
            node = rotateRight(node);
        }
    }
    else {
        node->right = insert(node->right, key, inserted);

        // Fix heap property
        if (node->right && node->right->priority > node->priority) {
            node = rotateLeft(node);
        }
    }

    return node;
}

int Treap::height() const {
    return height(root);
}

int Treap::height(Node* node) {
    if (!node) return 0;
    int hl = height(node->left);
    int hr = height(node->right);
    return 1 + (hl > hr ? hl : hr);
}

double Treap::averageDepth() const {
    long long sum = 0;
    long long count = 0;
    depthSum(root, 0, sum, count);
    if (count == 0) return 0.0;
    return static_cast<double>(sum) / static_cast<double>(count);
}

void Treap::depthSum(Node* node, int depth, long long& sum, long long& count) {
    if (!node) return;
    sum += depth;
    count += 1;
    depthSum(node->left, depth + 1, sum, count);
    depthSum(node->right, depth + 1, sum, count);
}
