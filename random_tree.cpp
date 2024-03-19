#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

// Copy begin
int random(int a, int b) {
    return (rand() % (b - a + 1)) + a;
}

void generateRandomTree(int N, vector< vector<int> >& g) {
    g.assign(N, vector<int>());
    vector<int> a(N);
    for (int i = 0; i < N; ++i) {
        a[i] = i;
    }
    random_shuffle(a.begin(), a.end());

    int freeV = 1;
    for (int i = 0; i < N && freeV < N; ++i) {
        int edgeCnt = random(1, 3);
        for (int x = 0; x < edgeCnt && freeV < N; ++x) {
            g[a[x]].push_back(a[freeV]);
            g[a[freeV]].push_back(a[x]);
            ++freeV;
        }
    }
}
// Copy end

const int TEST_COUNT = 100;

bool randomTest() {
    bool success = true;
    vector< vector<int> > g;
    for (int t = 0; t < TEST_COUNT && success; ++t) {
        const int N = random(1, 10);
        generateRandomTree(N, g);

        vector<bool> used(N, false);
        int usedCnt = 0;
        int edgeCnt = 0;
        for (int a = 0; a < N; ++a) {
            if (!used[a]) {
                used[a] = true;
                ++usedCnt;
            }
            for (int x = 0; x < g[a].size(); ++x) {
                int b = g[a][x];
                if (a < b) {
                    ++edgeCnt;
                }
                if (!used[b]) {
                    used[b] = true;
                    ++usedCnt;
                }
            }
        }

        success &= (edgeCnt == N - 1);
        success &= (usedCnt == N);

        if (!success) {
            cout << "Test Failed. N = " << N << endl;
            for (int a = 0; a < N; ++a) {
                for (int x = 0; x < g[a].size(); ++x) {
                    int b = g[a][x];
                    cout << a << " " << b << endl;
                }
            }
        }
    }
    return success;
}

bool processTest(const std::string& testName, std::function<bool()> testFun) {
    bool testRes = testFun();
    if (testRes) {
        std::cout << testName << " -> SUCCESS" << std::endl;
    }
    else {
        std::cout << testName << " -> FAILED" << std::endl;
    }
    return testRes;
}

int main() {
    bool ok = true;
    ok &= processTest("randomTest", randomTest);
    if (ok) {
        return 0;
    }
    else {
        return 1;
    }
}