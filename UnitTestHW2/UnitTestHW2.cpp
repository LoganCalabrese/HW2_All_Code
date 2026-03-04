#include "pch.h"
#include "../DSHW2/Treap.h"
#include "../DSHW2/Treap.cpp"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestHW2
{
    TEST_CLASS(UnitTestHW2)
    {
    public:

        // --- Empty Tree ---
        TEST_METHOD(EmptyTree_SizeIsZero)
        {
            Treap t;
            Assert::AreEqual(0, t.size());
        }

        TEST_METHOD(EmptyTree_HeightIsZero)
        {
            Treap t;
            Assert::AreEqual(0, t.height());
        }

        TEST_METHOD(EmptyTree_AverageDepthIsZero)
        {
            Treap t;
            Assert::AreEqual(0.0, t.averageDepth());
        }

        TEST_METHOD(EmptyTree_FindReturnsFalse)
        {
            Treap t;
            Assert::IsFalse(t.find(1));
        }

        // --- Single Insert ---
        TEST_METHOD(SingleInsert_ReturnsTrue)
        {
            Treap t;
            Assert::IsTrue(t.insert(5));
        }

        TEST_METHOD(SingleInsert_SizeIsOne)
        {
            Treap t;
            t.insert(5);
            Assert::AreEqual(1, t.size());
        }

        TEST_METHOD(SingleInsert_HeightIsOne)
        {
            Treap t;
            t.insert(5);
            Assert::AreEqual(1, t.height());
        }

        TEST_METHOD(SingleInsert_FindReturnsTrue)
        {
            Treap t;
            t.insert(5);
            Assert::IsTrue(t.find(5));
        }

        TEST_METHOD(SingleInsert_FindMissingReturnsFalse)
        {
            Treap t;
            t.insert(5);
            Assert::IsFalse(t.find(99));
        }

        // --- Duplicate Insert ---
        TEST_METHOD(Duplicate_SecondInsertReturnsFalse)
        {
            Treap t;
            t.insert(10);
            Assert::IsFalse(t.insert(10));
        }

        TEST_METHOD(Duplicate_SizeStaysOne)
        {
            Treap t;
            t.insert(10);
            t.insert(10);
            Assert::AreEqual(1, t.size());
        }

        TEST_METHOD(Duplicate_FindStillReturnsTrue)
        {
            Treap t;
            t.insert(10);
            t.insert(10);
            Assert::IsTrue(t.find(10));
        }

        // --- Multiple Inserts ---
        TEST_METHOD(MultiInsert_SizeCorrect)
        {
            Treap t;
            int keys[] = { 5, 3, 7, 1, 4, 6, 8 };
            for (int k : keys) t.insert(k);
            Assert::AreEqual(7, t.size());
        }

        TEST_METHOD(MultiInsert_AllKeysFound)
        {
            Treap t;
            int keys[] = { 5, 3, 7, 1, 4, 6, 8 };
            for (int k : keys) t.insert(k);
            for (int k : keys)
                Assert::IsTrue(t.find(k));
        }

        TEST_METHOD(MultiInsert_MissingKeyNotFound)
        {
            Treap t;
            int keys[] = { 5, 3, 7, 1, 4, 6, 8 };
            for (int k : keys) t.insert(k);
            Assert::IsFalse(t.find(99));
        }

        // --- Negative Keys ---
        TEST_METHOD(NegativeKeys_InsertAndFind)
        {
            Treap t;
            t.insert(-10);
            t.insert(-5);
            t.insert(-1);
            Assert::IsTrue(t.find(-10));
            Assert::IsTrue(t.find(-5));
            Assert::IsTrue(t.find(-1));
        }

        TEST_METHOD(NegativeKeys_MissingNotFound)
        {
            Treap t;
            t.insert(-10);
            t.insert(-5);
            Assert::IsFalse(t.find(-3));
        }

        // --- Ascending Insert ---
        TEST_METHOD(Ascending_SizeCorrect)
        {
            Treap t;
            for (int i = 1; i <= 10; i++) t.insert(i);
            Assert::AreEqual(10, t.size());
        }

        TEST_METHOD(Ascending_FirstAndLastFound)
        {
            Treap t;
            for (int i = 1; i <= 10; i++) t.insert(i);
            Assert::IsTrue(t.find(1));
            Assert::IsTrue(t.find(10));
        }

        TEST_METHOD(Ascending_OutOfRangeNotFound)
        {
            Treap t;
            for (int i = 1; i <= 10; i++) t.insert(i);
            Assert::IsFalse(t.find(11));
        }

        // --- Descending Insert ---
        TEST_METHOD(Descending_SizeCorrect)
        {
            Treap t;
            for (int i = 10; i >= 1; i--) t.insert(i);
            Assert::AreEqual(10, t.size());
        }

        TEST_METHOD(Descending_FirstAndLastFound)
        {
            Treap t;
            for (int i = 10; i >= 1; i--) t.insert(i);
            Assert::IsTrue(t.find(1));
            Assert::IsTrue(t.find(10));
        }

        TEST_METHOD(Descending_OutOfRangeNotFound)
        {
            Treap t;
            for (int i = 10; i >= 1; i--) t.insert(i);
            Assert::IsFalse(t.find(0));
        }

        // --- Height Sanity ---
        TEST_METHOD(Height_ReasonableRange)
        {
            srand(42);
            Treap t;
            for (int i = 1; i <= 100; i++) t.insert(i);
            int h = t.height();
            // Should be well below 100 (degenerate) and at least 7 (perfect)
            Assert::IsTrue(h >= 7 && h <= 100);
        }

        // --- Average Depth ---
        TEST_METHOD(AverageDepth_SingleNodeIsZero)
        {
            Treap t;
            t.insert(42);
            Assert::AreEqual(0.0, t.averageDepth());
        }
    };
}