#ifndef TREAP_H
#define TREAP_H

#include <cstdlib>  // rand()

class Treap {
private:
    struct Node {
        int key;
        int priority;
        Node* left;
        Node* right;

        Node(int k) {
            key = k;
            priority = rand();
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    int nodeCount;

    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* insert(Node* node, int key, bool& inserted);
    bool find(Node* node, int key) const;
    int height(Node* node) const;
    double depthSum(Node* node, int depth) const;
    void destroy(Node* node);

public:
    Treap();
    ~Treap();

    bool insert(int key);
    bool find(int key) const;
    int size() const;
    int height() const;
    double averageDepth() const;
};

#endif