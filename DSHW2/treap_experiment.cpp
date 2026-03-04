#include "Treap.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

struct Stats {
    double mean = 0.0;
    double stdev = 0.0;
    int minv = 0;
    int maxv = 0;
};

static Stats computeStats(const std::vector<int>& v) {
    Stats s;
    if (v.empty()) return s;
    s.minv = *std::min_element(v.begin(), v.end());
    s.maxv = *std::max_element(v.begin(), v.end());

    double sum = 0.0;
    for (int x : v) sum += x;
    s.mean = sum / v.size();

    double var = 0.0;
    for (int x : v) var += (x - s.mean) * (x - s.mean);
    var /= v.size();
    s.stdev = std::sqrt(var);
    return s;
}

static int idealBalancedHeight(int n) {
    // Perfectly balanced binary tree with n nodes has height about ceil(log2(n+1)).
    return (int)std::ceil(std::log2(n + 1.0));
}

int main() {
    const int N = 10000;
    const int TRIALS = 30;

    int ideal = idealBalancedHeight(N);

    std::vector<int> ascHeights, descHeights, randHeights;
    std::vector<double> ascAvgDepth, descAvgDepth, randAvgDepth;

    // Pre-build key arrays
    std::vector<int> ascKeys(N), descKeys(N), randKeys(N);
    for (int i = 0; i < N; i++) ascKeys[i] = i + 1;
    descKeys = ascKeys;
    std::reverse(descKeys.begin(), descKeys.end());
    randKeys = ascKeys;

    for (int t = 0; t < TRIALS; t++) {
        unsigned int seed = 1000u + (unsigned int)t;

        // Ascending
        {
            Treap tr(seed);
            for (int k : ascKeys) tr.insert(k);
            int h = tr.height();
            double d = tr.averageDepth();
            ascHeights.push_back(h);
            ascAvgDepth.push_back(d);
        }

        // Descending
        {
            Treap tr(seed);
            for (int k : descKeys) tr.insert(k);
            int h = tr.height();
            double d = tr.averageDepth();
            descHeights.push_back(h);
            descAvgDepth.push_back(d);
        }

        // Random insertion order (shuffle each trial)
        {
            std::mt19937 r(seed + 999u);
            std::shuffle(randKeys.begin(), randKeys.end(), r);

            Treap tr(seed);
            for (int k : randKeys) tr.insert(k);
            int h = tr.height();
            double d = tr.averageDepth();
            randHeights.push_back(h);
            randAvgDepth.push_back(d);
        }
    }

    Stats sAsc = computeStats(ascHeights);
    Stats sDesc = computeStats(descHeights);
    Stats sRand = computeStats(randHeights);

    auto meanDepth = [](const std::vector<double>& v) {
        double sum = 0.0;
        for (double x : v) sum += x;
        return v.empty() ? 0.0 : sum / v.size();
        };

    std::cout << "Treap height experiment (N=" << N << ", trials=" << TRIALS << ")\n";
    std::cout << "Perfectly balanced height (approx ceil(log2(N+1))) = " << ideal << "\n\n";

    std::cout << "Ascending insert order:\n";
    std::cout << "  height mean=" << sAsc.mean << " stdev=" << sAsc.stdev
        << " min=" << sAsc.minv << " max=" << sAsc.maxv << "\n";
    std::cout << "  average depth mean=" << meanDepth(ascAvgDepth) << "\n\n";

    std::cout << "Descending insert order:\n";
    std::cout << "  height mean=" << sDesc.mean << " stdev=" << sDesc.stdev
        << " min=" << sDesc.minv << " max=" << sDesc.maxv << "\n";
    std::cout << "  average depth mean=" << meanDepth(descAvgDepth) << "\n\n";

    std::cout << "Random insert order:\n";
    std::cout << "  height mean=" << sRand.mean << " stdev=" << sRand.stdev
        << " min=" << sRand.minv << " max=" << sRand.maxv << "\n";
    std::cout << "  average depth mean=" << meanDepth(randAvgDepth) << "\n\n";

    return 0;
}
