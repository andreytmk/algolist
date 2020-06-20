#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

// Copy begin
class HungarianAlgo {
private:
    const int HUNGARIAN_INF = static_cast<int>(1.0e9);
    std::vector<int> u;
    std::vector<int> v;
    std::vector<int> m;
    std::vector<bool> z1;
    std::vector<bool> z2;
    std::vector<bool> xHasEdge;
    int hasEdgeCount;

    bool runKuhn(const std::vector< std::vector<int> >& a, int x) {
        if (z1[x]) {
            return false;
        }
        z1[x] = true;

        for (int y = 0; y < a[x].size(); ++y) {
            if (u[x] + v[y] == a[x][y] && m[y] != x) {
                z2[y] = true;
                if (m[y] == -1 || runKuhn(a, m[y])) {
                    m[y] = x;
                    if (!xHasEdge[x]) {
                        xHasEdge[x] = true;
                        ++hasEdgeCount;
                    }
                    return true;
                }
            }
        }
        return false;
    }

public:
    int hungarianSolve(const std::vector< std::vector<int> >& a) {
        const int X = a.size();
        const int Y = a[0].size();

        u.assign(X, 0);
        v.assign(Y, 0);
        m.assign(Y, -1);
        xHasEdge.assign(X, false);
        hasEdgeCount = 0;
        do {
            bool kuhnUpdated = false;
            do {
                z1.assign(X, false);
                z2.assign(Y, false);
                kuhnUpdated = false;
                for (int x = 0; x < X; ++x) {
                    if (!xHasEdge[x]) {
                        kuhnUpdated |= runKuhn(a, x);
                    }
                }
            } while (kuhnUpdated);

            int delta = HUNGARIAN_INF;
            for (int x = 0; x < X; ++x) {
                for (int y = 0; y < Y; ++y) {
                    if (z1[x] && !z2[y]) {
                        delta = std::min(delta, a[x][y] - u[x] - v[y]);
                    }
                }
            }
            for (int x = 0; x < X; ++x) {
                if (z1[x]) {
                    u[x] += delta;
                }
            }
            for (int y = 0; y < Y; ++y) {
                if (z2[y]) {
                    v[y] -= delta;
                }
            }
        } while (hasEdgeCount < X);

        int ans = 0;
        for (int x = 0; x < X; ++x) {
            ans += u[x];
        }
        for (int y = 0; y < Y; ++y) {
            ans += v[y];
        }
        return ans;
    }
};
// Copy end

const int INF = static_cast<int>(1.0e9);
const int MINVAL = 1000;
const int MAXVAL = -1000;

const int TEST_COUNT = 100;

bool useExample() {
    vector<vector<int>> a = { {1, 2}, {3, 8} };
    HungarianAlgo hungarianAlgo;
    int hungarianRes = hungarianAlgo.hungarianSolve(a);
    return (hungarianRes == 5);
}

int random(int a, int b) {
    return (rand() % (b - a + 1)) + a;
}

vector<vector<int>> generateWorkTable(int minX, int maxX, int maxY) {
    int X = random(minX, maxX);
    int Y = random(X, maxY);
    vector<vector<int>> a(X, vector<int>(Y));
    for (int x = 0; x < X; ++x) {
        for (int y = 0; y < Y; ++y) {
            a[x][y] = random(MINVAL, MAXVAL);
        }
    }
    return a;
}

int bruteForceSolve(const vector<vector<int>>& a) {
    const int X = a.size();
    const int Y = a[0].size();
    vector<int> p(Y);
    for (int y = 0; y < Y; ++y) {
        p[y] = y;
    }

    int res = INF;
    do {
        int sum = 0;
        for (int x = 0; x < X; ++x) {
            sum += a[x][p[x]];
        }
        res = min(res, sum);
    } while (std::next_permutation(p.begin(), p.end()));

    return res;
}

bool test() {
    bool success = true;
    HungarianAlgo hungarianAlgo;
    for (int t = 0; t < TEST_COUNT && success; ++t) {
        vector<vector<int>> a = generateWorkTable(1, 5, 5);
        if (t == 0) {
            a = { {1, 2}, {3, 8} };
        }
        int bfRes = bruteForceSolve(a);
        int hungarianRes = hungarianAlgo.hungarianSolve(a);
        if (bfRes != hungarianRes) {
            success = false;
        }
    }
    return success;
}

bool bigOtest() {
    HungarianAlgo hungarianAlgo;
    const int MAXN = 1000;
    vector<vector<int>> a = generateWorkTable(MAXN, MAXN, MAXN);
    int bigHungarianRes = hungarianAlgo.hungarianSolve(a);
    return true;
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
    ok &= processTest("useExample", useExample);
    ok &= processTest("test", test);
    ok &= processTest("bigOtest", bigOtest);
    if (ok) {
        return 0;
    }
    else {
        return 1;
    }
}
