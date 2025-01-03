#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::pair<int, int>> readItems(int& itemCount, int& capacity) {
    std::cin >> itemCount >> capacity;
    std::vector<std::pair<int, int>> items(itemCount);
    for (int idx = 0; idx < itemCount; ++idx) {
        std::cin >> items[idx].first >> items[idx].second;
    }
    return items;
}

std::tuple<unsigned long long, int, std::vector<int>> calculateMaxValue(
    const std::vector<std::pair<int, int>>& items, 
    int itemCount, 
    int capacity
) {
    std::vector<std::vector<std::vector<unsigned long long>>> dp(
        itemCount + 1, 
        std::vector<std::vector<unsigned long long>>(
            itemCount + 1, 
            std::vector<unsigned long long>(capacity + 1, 0)
        )
    );

    for (int curr = 1; curr <= itemCount; ++curr) {
        for (int count = 1; count <= curr; ++count) {
            for (int cap = 1; cap <= capacity; ++cap) {
                if (items[curr - 1].first <= cap) {
                    dp[curr][count][cap] = std::max(
                        dp[curr - 1][count][cap], 
                        dp[curr - 1][count - 1][cap - items[curr - 1].first] + items[curr - 1].second
                    );

                    if (!dp[curr - 1][count - 1][cap - items[curr - 1].first] &&
                        count != 1 && 
                        dp[curr][count][cap] == items[curr - 1].second) {
                        dp[curr][count][cap] = dp[curr - 1][count][cap];
                    }
                } else {
                    dp[curr][count][cap] = dp[curr - 1][count][cap];
                }
            }
        }
    }

    unsigned long long maxResult = 0;
    int optimalCount = 0;

    for (int count = 0; count <= itemCount; ++count) {
        if (dp[itemCount][count][capacity] * count > maxResult) {
            maxResult = dp[itemCount][count][capacity] * count;
            optimalCount = count;
        }
    }

    std::vector<int> path;
    int curr = itemCount, count = optimalCount, cap = capacity;

    while (dp[curr][count][cap] != 0) {
        if (dp[curr][count][cap] == dp[curr - 1][count][cap]) {
            --curr;
        } else {
            path.push_back(curr);
            cap -= items[curr - 1].first;
            --curr;
            --count;
        }
    }

    return {maxResult, optimalCount, path};
}

void printResult(unsigned long long maxValue, const std::vector<int>& path) {
    std::cout << maxValue << "\n";
    for (int idx = path.size() - 1; idx >= 0; --idx) {
        std::cout << path[idx] << " ";
    }
    std::cout << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int itemCount, capacity;
    auto items = readItems(itemCount, capacity);

    auto [maxValue, optimalCount, path] = calculateMaxValue(items, itemCount, capacity);

    printResult(maxValue, path);

    return 0;
}
