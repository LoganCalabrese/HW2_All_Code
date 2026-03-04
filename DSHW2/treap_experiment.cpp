#include "Treap.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;

// Simple shuffle using rand()
void shuffle(int* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int perfectHeight(int n) {
    // ceil(log2(n+1))
    int h = 0;
    int nodes = 1;
    while (nodes - 1 < n) {
        h++;
        nodes *= 2;
    }
    return h;
}

int main() {
    srand(42);

    const int N = 10000;
    const int TRIALS = 10;

    int ideal = perfectHeight(N);

    // Accumulators
    int   ascHeightSum = 0, descHeightSum = 0, randHeightSum = 0;
    double ascDepthSum = 0, descDepthSum = 0, randDepthSum = 0;

    int ascMin = 999999, ascMax = 0;
    int descMin = 999999, descMax = 0;
    int randMin = 999999, randMax = 0;

    // Build key arrays
    int* keys = new int[N];
    for (int i = 0; i < N; i++) keys[i] = i + 1;

    for (int t = 0; t < TRIALS; t++) {

        // --- Ascending ---
        {
            Treap tr;
            for (int i = 0; i < N; i++) tr.insert(i + 1);
            int h = tr.height();
            ascHeightSum += h;
            ascDepthSum += tr.averageDepth();
            if (h < ascMin) ascMin = h;
            if (h > ascMax) ascMax = h;
        }

        // --- Descending ---
        {
            Treap tr;
            for (int i = N; i >= 1; i--) tr.insert(i);
            int h = tr.height();
            descHeightSum += h;
            descDepthSum += tr.averageDepth();
            if (h < descMin) descMin = h;
            if (h > descMax) descMax = h;
        }

        // --- Random ---
        {
            shuffle(keys, N);
            Treap tr;
            for (int i = 0; i < N; i++) tr.insert(keys[i]);
            int h = tr.height();
            randHeightSum += h;
            randDepthSum += tr.averageDepth();
            if (h < randMin) randMin = h;
            if (h > randMax) randMax = h;
        }
    }

    delete[] keys;

    double ascAvgH = (double)ascHeightSum / TRIALS;
    double descAvgH = (double)descHeightSum / TRIALS;
    double randAvgH = (double)randHeightSum / TRIALS;

    double ascAvgD = ascDepthSum / TRIALS;
    double descAvgD = descDepthSum / TRIALS;
    double randAvgD = randDepthSum / TRIALS;

    cout << "=== Treap Experiment: N=" << N << ", Trials=" << TRIALS << " ===\n\n";
    cout << "Perfectly balanced height: " << ideal << "\n\n";

    cout << fixed << setprecision(2);

    cout << left
        << setw(16) << "Insertion Order"
        << setw(12) << "Avg Height"
        << setw(6) << "Min"
        << setw(6) << "Max"
        << setw(16) << "Avg Node Depth"
        << setw(15) << "Ratio to Ideal"
        << "\n";

    cout << string(16, '-') << string(12, '-') << string(6, '-')
        << string(6, '-') << string(16, '-') << string(15, '-') << "\n";

    cout << left
        << setw(16) << "Ascending"
        << setw(12) << ascAvgH
        << setw(6) << ascMin
        << setw(6) << ascMax
        << setw(16) << ascAvgD
        << setw(15) << (ascAvgH / ideal)
        << "\n";

    cout << left
        << setw(16) << "Descending"
        << setw(12) << descAvgH
        << setw(6) << descMin
        << setw(6) << descMax
        << setw(16) << descAvgD
        << setw(15) << (descAvgH / ideal)
        << "\n";

    cout << left
        << setw(16) << "Random"
        << setw(12) << randAvgH
        << setw(6) << randMin
        << setw(6) << randMax
        << setw(16) << randAvgD
        << setw(15) << (randAvgH / ideal)
        << "\n";

    cout << "\n--- Analysis ---\n";
    cout << "Ideal log2(" << N << "+1) height = " << ideal << "\n";
    cout << "Ascending  height is ~" << (ascAvgH / ideal) << "x ideal\n";
    cout << "Descending height is ~" << (descAvgH / ideal) << "x ideal\n";
    cout << "Random     height is ~" << (randAvgH / ideal) << "x ideal\n";

    return 0;
}