#include <assert.h>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

std::default_random_engine randEng;
std::normal_distribution<float> generator(10, 3);

void printHistogram(const std::vector<std::vector<int>> &histogram) {
    for (auto &bin : histogram) {
        std::cout << bin.size() << " ";
    }
    std::cout << std::endl;
}

void partBubble(std::vector<std::vector<int>> &histogram, int binNum, int chooseNum) {
    assert(chooseNum > 0 && binNum > chooseNum && "参数不合法");
    for (int step = 0; step < chooseNum; ++step) {
        for (int binID = binNum - 1; binID > step; --binID) {
            if (histogram[binID].size() > histogram[binID - 1].size()) {
                std::swap(histogram[binID], histogram[binID - 1]);
            }
        }
        printHistogram(histogram);
    }
}

void initHistogram(std::vector<std::vector<int>> &histogram, int binNum) {
    histogram.resize(binNum);
    for (int idx = 0; idx < binNum; ++idx) {
        int num = std::round(generator(randEng));
        for (int i = 0; i < num; ++i)
            histogram[idx].push_back(i);
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::vector<int>> histogram;
    initHistogram(histogram, 30);
    printHistogram(histogram);
    partBubble(histogram, 30, 3);

    return 0;
}
