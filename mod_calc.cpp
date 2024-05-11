#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MOD = 97;

// Copy begin
int fastPower(int base, int power) {
    int result = 1;
    while (power > 0) {
        if (power % 2 == 1) {
            result = (result * base) % MOD;
        }
        power /= 2;
        base = (base * base) % MOD;
    }
    return result;
}
// Copy end

// Copy begin
vector<int> fact;
vector<int> factInv;
vector<int64_t> comb;

void buildFact(const int N) {
    fact.resize(N + 1);
    fact[0] = 1;
    for (int i = 1; i <= N; ++i) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
}

void buildFactInv(const int N) {
    factInv.resize(N + 1);
    for (int i = 0; i <= N; ++i) {
        factInv[i] = fastPower(fact[i], MOD - 2);
    }
}

void buildComb(const int N) {
    comb.resize(N + 1);
    comb[0] = 1;
    for (int64_t i = 1; i <= N; ++i) {
        comb[i] = (comb[i - 1] * (N - i + 1)) % MOD;
        comb[i] = (comb[i] * fastPower(i, MOD - 2)) % MOD;
    }
}

int calcC(int k, int n) {
    int result = 1;
    result = (result * fact[n]) % MOD;
    result = (result * factInv[k]) % MOD;
    result = (result * factInv[n - k]) % MOD;
    return result;
}
// Copy end

const int TEST_COUNT = 1000;

int random(int a, int b) {
    return (rand() % (b - a + 1)) + a;
}

int naivePower(int base, int power) {
    int result = 1;
    for (int i = 0; i < power; ++i) {
        result = (result * base) % MOD;
    }
    return result;
}

int factNaive(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result = (result * i);
    }
    return result;
}

int cNaive(int k, int n) {
    int result = factNaive(n);
    result /= factNaive(k);
    result /= factNaive(n - k);
    return result;
}

bool fastPowerTest() {
    bool success = true;
    for (int t = 0; t < TEST_COUNT && success; ++t) {
        int base = random(0, 100);
        int power = random(0, 10000);
        int fastAns = fastPower(base, power);
        int naiveAns = naivePower(base, power);
        if (fastAns != naiveAns) {
            std::cout
                << "Wrong Answer. base=" << base << " power=" << power
                << " Expected: " << naiveAns << " Actual: " << fastAns
                << std::endl;
            success = false;
        }
    }
    return success;
}

bool calcC_Test() {
    bool success = true;
    const int MAXN = 12;
    buildFact(MAXN);
    buildFactInv(MAXN);
    for (int t = 0; t < TEST_COUNT && success; ++t) {
        int n = random(1, MAXN);
        buildComb(n);
        int k = random(1, n);
        int actual = calcC(k, n);

        int expected = cNaive(k, n) % MOD;
        if (actual != expected) {
            std::cout
                << "Wrong Answer. k=" << k << " n=" << n
                << " Expected: " << expected << " Actual: " << actual
                << std::endl;
            success = false;
        }

        int prebuild = comb[k];
        if (expected != prebuild) {
            std::cout
                << "Wrong Answer. k=" << k << " n=" << n
                << " Expected: " << expected << " Prebuild: " << prebuild
                << std::endl;
            success = false;
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
    srand(1537);

    bool ok = true;
    ok &= processTest("fastPowerTest", fastPowerTest);
    ok &= processTest("calcC_Test", calcC_Test);
    if (ok) {
        return 0;
    }
    else {
        return 1;
    }
}
