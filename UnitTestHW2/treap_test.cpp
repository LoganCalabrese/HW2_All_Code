#include "Treap.h"
#include <iostream>
#include <vector>
#include <string>

// Simple test macros (no external libraries)
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " expected true: " #cond "\n"; \
        g_failed++; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    if ((cond)) { \
        std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " expected false: " #cond "\n"; \
        g_failed++; \
    } \
} while (0)

#define EXPECT_EQ(a,b) do { \
    auto _a = (a); auto _b = (b); \
    if (!(_a == _b)) { \
        std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ << " expected " #a " == " #b \
                  << " (" << _a << " vs " << _b << ")\n"; \
        g_failed++; \
    } \
} while (0)

static void test_empty_tree() {
    Treap t(123);
    EXPECT_FALSE(t.find(10));
    EXPECT_EQ(t.size(), (std::size_t)0);
    EXPECT_EQ(t.height(), 0);
    EXPECT_EQ(t.averageDepth(), 0.0);
}

static void test_insert_and_find_basic() {
    Treap t(123);
    EXPECT_TRUE(t.insert(5));
    EXPECT_TRUE(t.insert(2));
    EXPECT_TRUE(t.insert(8));

    EXPECT_TRUE(t.find(5));
    EXPECT_TRUE(t.find(2));
    EXPECT_TRUE(t.find(8));
    EXPECT_FALSE(t.find(99));

    EXPECT_EQ(t.size(), (std::size_t)3);
    EXPECT_TRUE(t.height() >= 2); // with 3 nodes, height is 2 or 3 depending on priorities
}

static void test_duplicate_inserts() {
    Treap t(123);
    EXPECT_TRUE(t.insert(10));
    EXPECT_FALSE(t.insert(10));
    EXPECT_FALSE(t.insert(10));
    EXPECT_EQ(t.size(), (std::size_t)1);
    EXPECT_TRUE(t.find(10));
}

static void test_negative_and_large_values() {
    Treap t(123);
    EXPECT_TRUE(t.insert(-100));
    EXPECT_TRUE(t.insert(0));
    EXPECT_TRUE(t.insert(2000000000));

    EXPECT_TRUE(t.find(-100));
    EXPECT_TRUE(t.find(0));
    EXPECT_TRUE(t.find(2000000000));
    EXPECT_FALSE(t.find(-101));
}

static void test_many_inserts_all_found() {
    Treap t(123);
    const int N = 2000;
    for (int i = 1; i <= N; i++) {
        EXPECT_TRUE(t.insert(i));
    }
    EXPECT_EQ(t.size(), (std::size_t)N);

    for (int i = 1; i <= N; i++) {
        EXPECT_TRUE(t.find(i));
    }
    EXPECT_FALSE(t.find(N + 1));
    EXPECT_TRUE(t.height() > 0);
}

static void test_order_independence_sanity() {
    // Insert the same keys in different orders. The exact shape can differ
    // (because priorities depend on insertion calls), but all keys should exist.
    std::vector<int> keys = { 40, 10, 60, 20, 50, 70, 30 };
    Treap a(123), b(123);

    for (int k : keys) EXPECT_TRUE(a.insert(k));

    // different order
    std::vector<int> keys2 = { 30, 70, 10, 50, 40, 60, 20 };
    for (int k : keys2) EXPECT_TRUE(b.insert(k));

    for (int k : keys) {
        EXPECT_TRUE(a.find(k));
        EXPECT_TRUE(b.find(k));
    }
    EXPECT_EQ(a.size(), b.size());
}

int main() {
    std::cout << "Running Treap unit tests...\n";

    test_empty_tree();
    test_insert_and_find_basic();
    test_duplicate_inserts();
    test_negative_and_large_values();
    test_many_inserts_all_found();
    test_order_independence_sanity();

    if (g_failed == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
    else {
        std::cout << g_failed << " tests FAILED.\n";
        return 1;
    }
}
