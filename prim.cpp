#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

const int INF = 1.0e9;

// Copy begin
int primMinTreeWeightNaive(const vector<vector<int>>& g) {
    const int N = g.size();
    vector<bool> used(N, false);
    used[0] = true;
    int minTreeWeight = 0;
    for (int i = 0; i < N - 1; ++i) {
        int minWeight = INF;
        int minA = -1;
        int minB = -1;
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if ((used[x] && !used[y]) || (used[y] && !used[x])) {
                    if (g[x][y] < minWeight) {
                        minWeight = g[x][y];
                        minA = x;
                        minB = y;
                    }
                }
            }
        }
        if (minA >= 0 && minB >= 0) {
            minTreeWeight += minWeight;
            used[minA] = true;
            used[minB] = true;
        }
    }
    return minTreeWeight;
}
// Copy end

// Copy begin
int primMinTreeWeightEffective(const vector<vector<int>>& g) {
    const int N = g.size();
    vector<bool> used(N, false);
    vector<int> w(N, INF);
    vector<int> p(N, INF);
    int minTreeWeight = 0;
    int current = 0;
    while (current >= 0) {
        used[current] = true;
        for (int i = 0; i < N; ++i) {
            if (used[i]) {
                continue;
            }
            if (g[current][i] < w[i]) {
                w[i] = g[current][i];
                p[i] = current;
            }
        }

        current = -1;
        int minEdge = INF;
        for (int i = 0; i < N; ++i) {
            if (!used[i] && w[i] < minEdge) {
                minEdge = w[i];
                current = i;
            }
        }
        if (current >= 0) {
            minTreeWeight += minEdge;
        }
    }
    return minTreeWeight;
}
// Copy end

const int TEST_COUNT = 1000;

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

void generateTestGraph(int N, vector<vector<int>>& g, int& minTreeWeight) {
    g.assign(N, vector<int>(N, INF));
    for (int i = 0; i < N; ++i) {
        g[i][i] = 0;
    }
    minTreeWeight = 0;

    vector<vector<int>> tree;
    generateRandomTree(N, tree);
    for(int i = 0; i < tree.size(); ++i) {
        for (int x = 0; x < tree[i].size(); ++x) {
            int a = i;
            int b = tree[i][x];
            if (g[a][b] == INF && g[b][a] == INF) {
                int weight = random(1, 5);
                g[a][b] = weight;
                g[b][a] = weight;
                minTreeWeight += weight;
            }
        }
    }

    const int additionalEdgeCount = random(10, 20);
    for (int i = 0; i < additionalEdgeCount; ++i) {
        int a = random(0, N - 1);
        int b = random(0, N - 1);
        if (g[a][b] == INF && g[b][a] == INF) {
            int weight = random(6, 10);
            g[a][b] = weight;
            g[b][a] = weight;
        }
    }
}

bool trivialTest() {
    const int N = 1;
    vector<vector<int>> g(N, vector<int>(N, 0));
    int weight = primMinTreeWeightNaive(g);
    return weight == 0;
}

void traceGraph(const vector< vector<int> >& g, int minTreeWeight) {
    const int N = g.size();
    cout << "minTreeWeight=" << minTreeWeight << endl;
    for (int i = 0; i < N; ++i) {
        for (int x = 0; x < N; ++x) {
            cout << setfill(' ') << setw(3) << g[i][x] << " ";
        }
        cout << endl;
    }
}

/*
void traceRandomGraph() {
    const int N = 4;
    vector<vector<int>> g;
    int minTreeWeight;
    generateTestGraph(N, g, minTreeWeight);
    traceGraph(g, minTreeWeight);
}
*/

bool stressTest() {
    bool ok = true;
    for (int t = 0; t < TEST_COUNT && ok; ++t) {
        const int N = random(2, 20);
        vector<vector<int>> g;
        int expectedWeight;
        generateTestGraph(N, g, expectedWeight);
        int actualWeight = primMinTreeWeightNaive(g);
        ok &= expectedWeight == actualWeight;
        if (!ok) {
            cout << "actualWeight=" << actualWeight << endl;
            traceGraph(g, expectedWeight);
        }

        actualWeight = primMinTreeWeightEffective(g);
        ok &= expectedWeight == actualWeight;
        if (!ok) {
            cout << "actualWeight=" << actualWeight << endl;
            traceGraph(g, expectedWeight);
        }
    }
    return ok;
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
    srand(1537);

    bool ok = true;
    ok &= processTest("trivialTest", trivialTest);
    ok &= processTest("stressTest", stressTest);
    if (ok) {
        return 0;
    }
    else {
        return 1;
    }
}
