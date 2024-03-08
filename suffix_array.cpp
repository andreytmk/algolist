#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

// Copy begin
class SuffixArray {
public:
    const int CHARS_COUNT = 256;

    vector<int> a;
    vector<int> classes;

    void Build(const std::string& s) {
        if (s.empty()) {
            a.clear();
            classes.clear();
            return;
        }

        const int N = s.size();
        a.resize(N);
        classes.resize(N);
        na.resize(N);
        nc.resize(N);

        vector<int> bucket(CHARS_COUNT, 0);
        for (int i = 0; i < N; ++i) {
            ++bucket[s[i]];
        }
        for (int i = 1; i < CHARS_COUNT; ++i) {
            bucket[i] = bucket[i - 1] + bucket[i];
        }
        for (int i = N - 1; i >= 0; --i) {
            --bucket[s[i]];
            a[bucket[s[i]]] = i;
        }
        classes[ a[0] ] = 0;
        for (int i = 1; i < N; ++i) {
            classes[ a[i] ] = classes[ a[i - 1] ];
            if (s[a[i - 1]] != s[a[i]]) {
                ++classes[ a[i] ];
            }
        }

        for (int h = 1; h <= N; h *= 2) {
            bucket.assign(N, 0);
            for (int i = 0; i < N; ++i) {
                ++bucket[classes[i]];
            }
            for (int i = 1; i < N; ++i) {
                bucket[i] = bucket[i - 1] + bucket[i];
            }
            for (int i = N - 1; i >= 0; --i) {
                int idx = (a[i] + N - h) % N;
                int cl = classes[idx];
                --bucket[cl];
                na[bucket[cl]] = idx;
            }

            nc[ na[0] ] = 0;
            for (int i = 1; i < N; ++i) {
                nc[ na[i] ] = nc[ na[i - 1] ];
                if (classes[na[i]] != classes[na[i - 1]] ||
                    classes[(na[i] + h) % N] != classes[(na[i-1] + h) % N])
                {
                    ++nc[na[i]];
                }
            }

            a.assign(na.begin(), na.end());
            classes.assign(nc.begin(), nc.end());
        }
    }

private:
    vector<int> na;
    vector<int> nc;
};
// Copy end

const int TEST_COUNT = 1000;

bool useExample() {
    const string s = "acab";
    SuffixArray suffixArray;
    suffixArray.Build(s);

    const vector<int> expectedA = { 2, 0, 3, 1 };
    const vector<int> expectedClasses = { 1, 3, 0, 2 };

    bool ok = true;
    ok &= std::equal(suffixArray.a.begin(), suffixArray.a.end(), expectedA.begin());
    ok &= std::equal(suffixArray.classes.begin(), suffixArray.classes.end(), expectedClasses.begin());
    return ok;
}

int random(int a, int b) {
    return (rand() % (b - a + 1)) + a;
}

struct StrSuffix {
    const std::string* str;
    int i;
};

struct StrSuffixCmp {
    bool operator()(const StrSuffix& left, const StrSuffix& right) const {
        const int N = left.str->size();
        for (int x = 0; x < N; ++x) {
            if (left.str->at((left.i + x) % N) == right.str->at((right.i + x) % N)) {
                continue;
            }
            return left.str->at((left.i + x) % N) < right.str->at((right.i + x) % N);
        }
        return false;
    }
};

void generateRndString(string& s) {
    const int N = random(1, 20);
    s.resize(N);
    for (int i = 0; i < N; ++i) {
        s[i] = 'a' + random(0, 2);
    }
}

void bruteForceSolve(const std::string& s, vector<int>& a, vector<int>& classes) {
    const int N = s.size();
    vector<StrSuffix> strSuf(N);
    std::string str(s.begin(), s.end());
    for (int i = 0; i < N; ++i) {
        strSuf[i].i = i;
        strSuf[i].str = &s;
    }
    std::sort(strSuf.begin(), strSuf.end(), StrSuffixCmp());

    a.resize(N);
    for (int i = 0; i < N; ++i) {
        a[i] = strSuf[i].i;
    }

    classes.resize(N);
    classes[ strSuf[0].i ] = 0;
    StrSuffixCmp cmp;
    for (int i = 1; i < N; ++i) {
        classes[strSuf[i].i] = classes[strSuf[i-1].i];
        if (cmp(strSuf[i], strSuf[i - 1]) || cmp(strSuf[i - 1], strSuf[i])) {
            ++classes[strSuf[i].i];
        }
    }
}

void printSuffixes(const string& s, const vector<int>& a) {
    const int N = s.size();
    for (int i = 0; i < N; ++i) {
        std::cout << s.substr(a[i]) << std::endl;
    }
}

bool test() {
    bool success = true;
    SuffixArray suffixArray;
    vector<int> a;
    vector<int> classes;
    for (int t = 0; t < TEST_COUNT && success; ++t) {
        string s;
        generateRndString(s);
        s = s + "$";
        bruteForceSolve(s, a, classes);
        suffixArray.Build(s);

        success &= std::equal(suffixArray.a.begin(), suffixArray.a.end(), a.begin());
        success &= std::equal(suffixArray.classes.begin(), suffixArray.classes.end(), classes.begin());
        if (!success) {
            std::cout << "Failed test:" << std::endl;
            std::cout << s << std::endl;
            std::cout << "Expected:" << std::endl;
            printSuffixes(s, a);
            std::cout << "Actual:" << std::endl;
            printSuffixes(s, suffixArray.a);
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
    ok &= processTest("useExample", useExample);
    ok &= processTest("test", test);
    if (ok) {
        return 0;
    }
    else {
        return 1;
    }
}