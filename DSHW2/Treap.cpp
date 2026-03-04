#include "Treap.h"

Treap::Treap() {
    root = nullptr;
    nodeCount = 0;
}

Treap::~Treap() {
    destroy(root);
}

void Treap::destroy(Node* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ── Rotations ────────────────────────────────────────────────────────────────

Treap::Node* Treap::rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

Treap::Node* Treap::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// ── Insert ───────────────────────────────────────────────────────────────────

bool Treap::insert(int key) {
    bool inserted = false;
    root = insert(root, key, inserted);
    if (inserted) nodeCount++;
    return inserted;
}

Treap::Node* Treap::insert(Node* node, int key, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new Node(key);
    }

    if (key == node->key) {
        inserted = false;
        return node;
    }

    if (key < node->key) {
        node->left = insert(node->left, key, inserted);
        // fix max-heap priority
        if (node->left != nullptr && node->left->priority > node->priority)
            node = rotateRight(node);
    }
    else {
        node->right = insert(node->right, key, inserted);
        if (node->right != nullptr && node->right->priority > node->priority)
            node = rotateLeft(node);
    }

    return node;
}

// ── Find ─────────────────────────────────────────────────────────────────────

bool Treap::find(int key) const {
    return find(root, key);
}

bool Treap::find(Node* node, int key) const {
    while (node != nullptr) {
        if (key == node->key) return true;
        if (key < node->key)  node = node->left;
        else                  node = node->right;
    }
    return false;
}

// ── Size / Height / Average Depth ────────────────────────────────────────────

int Treap::size() const {
    return nodeCount;
}

int Treap::height() const {
    return height(root);
}

int Treap::height(Node* node) const {
    if (node == nullptr) return 0;
    int left = height(node->left);
    int right = height(node->right);
    return 1 + (left > right ? left : right);
}

double Treap::averageDepth() const {
    if (nodeCount == 0) return 0.0;
    return depthSum(root, 0) / nodeCount;
}

double Treap::depthSum(Node* node, int depth) const {
    if (node == nullptr) return 0.0;
    return depth + depthSum(node->left, depth + 1) + depthSum(node->right, depth + 1);
}