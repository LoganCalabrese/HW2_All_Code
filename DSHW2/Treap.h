#ifndef TREAP_H
#define TREAP_H

#include <random>
#include <vector>
#include <utility>
#include <cstddef>

class Treap {
public:
    // If you pass a seed, the treap priorities will be repeatable (useful for tests).
    explicit Treap(unsigned int seed = std::random_device{}());

    // Insert key. Returns true if inserted, false if key already exists.
    bool insert(int key);

    // Find key. Returns true if exists, false otherwise.
    bool find(int key) const;

    // Number of nodes.
    std::size_t size() const { return nodeCount; }

    // Height in levels (empty tree = 0, single node = 1).
    int height() const;

    // Average depth (root depth = 0). Returns 0.0 if empty.
    double averageDepth() const;

private:
    struct Node {
        int key;
        int priority;
        Node* left;
        Node* right;
        Node(int k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {}
    };

    Node* root;
    std::size_t nodeCount;

    mutable std::mt19937 rng;
    std::uniform_int_distribution<int> prioDist;

    // Helpers
    static Node* rotateRight(Node* y);
    static Node* rotateLeft(Node* x);

    Node* insert(Node* node, int key, bool& inserted);

    static bool find(Node* node, int key);

    static int height(Node* node);
    static void depthSum(Node* node, int depth, long long& sum, long long& count);

    static void destroy(Node* node);

public:
    ~Treap();
    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;
};

#endif
#pragma once
