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

static void writePPMBarChart(const std::string& filename,
                            int ideal,
                            int ascMean, int descMean, int randMean)
{
    // Very simple grayscale bar chart in a PPM image (still "C++ only").
    const int W = 800;
    const int H = 450;
    const int margin = 60;

    auto clamp = [](int x){ return x < 0 ? 0 : (x > 255 ? 255 : x); };

    // background white
    std::vector<unsigned char> img(W * H * 3, 255);

    auto setPixel = [&](int x, int y, unsigned char c) {
        if (x < 0 || x >= W || y < 0 || y >= H) return;
        int idx = (y * W + x) * 3;
        img[idx + 0] = c;
        img[idx + 1] = c;
        img[idx + 2] = c;
    };

    auto drawRect = [&](int x0, int y0, int x1, int y1, unsigned char c) {
        if (x0 > x1) std::swap(x0, x1);
        if (y0 > y1) std::swap(y0, y1);
        for (int y = y0; y <= y1; y++) {
            for (int x = x0; x <= x1; x++) {
                setPixel(x, y, c);
            }
        }
    };

    // find max for scaling
    int maxH = std::max({ideal, ascMean, descMean, randMean});
    maxH = std::max(maxH, 1);

    // axes (gray)
    drawRect(margin, H - margin, W - margin, H - margin + 2, 180); // x axis
    drawRect(margin, margin, margin + 2, H - margin, 180); // y axis

    auto yFromValue = [&](int v) {
        double t = (double)v / (double)maxH;
        int y = (int)std::round((H - margin) - t * (H - 2 * margin));
        return y;
    };

    // ideal line (light gray)
    int yIdeal = yFromValue(ideal);
    drawRect(margin, yIdeal, W - margin, yIdeal + 1, 200);

    // bars
    int barW = 120;
    int gap = 90;
    int x1 = margin + 80;
    int x2 = x1 + barW + gap;
    int x3 = x2 + barW + gap;

    auto drawBar = [&](int x, int value, unsigned char shade) {
        int yTop = yFromValue(value);
        int yBot = H - margin;
        drawRect(x, yTop, x + barW, yBot, shade);
        // outline darker
        drawRect(x, yTop, x + barW, yTop + 2, clamp(shade - 40));
        drawRect(x, yBot - 2, x + barW, yBot, clamp(shade - 40));
        drawRect(x, yTop, x + 2, yBot, clamp(shade - 40));
        drawRect(x + barW - 2, yTop, x + barW, yBot, clamp(shade - 40));
    };

    drawBar(x1, ascMean, 140);
    drawBar(x2, descMean, 110);
    drawBar(x3, randMean, 80);

    // save PPM (P6)
    std::ofstream out(filename, std::ios::binary);
    out << "P6\n" << W << " " << H << "\n255\n";
    out.write(reinterpret_cast<const char*>(img.data()), img.size());
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

    std::ofstream csv("height_trials.csv");
    csv << "trial,order,height,avg_depth\n";

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
            csv << t << ",ascending," << h << "," << d << "\n";
        }

        // Descending
        {
            Treap tr(seed);
            for (int k : descKeys) tr.insert(k);
            int h = tr.height();
            double d = tr.averageDepth();
            descHeights.push_back(h);
            descAvgDepth.push_back(d);
            csv << t << ",descending," << h << "," << d << "\n";
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
            csv << t << ",random," << h << "," << d << "\n";
        }
    }

    csv.close();

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

    // Create a tiny bar-chart image (PPM) and leave it in the folder.
    writePPMBarChart("height_bars.ppm", ideal,
                     (int)std::round(sAsc.mean),
                     (int)std::round(sDesc.mean),
                     (int)std::round(sRand.mean));

    std::cout << "Wrote: height_trials.csv and height_bars.ppm\n";
    return 0;
}
