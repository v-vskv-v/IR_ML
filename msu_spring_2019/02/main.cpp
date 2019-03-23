#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector <int64_t> getTokens (string s){
    vector <int64_t> ans = {};
    int64_t tmp = 0;
    int64_t len = s.size();
    char sign = 1;
    for (unsigned i = 0; i < len; i++, tmp = 0) {
        if (s[i] >= '0' && s[i] <= '9') {
            while (i < len && s[i] >= '0' && s[i] <= '9') {
                tmp *= 10;
                tmp += s[i++] - '0';
            }
            tmp *= sign;
            sign = 1;
            i--;
            ans.push_back(tmp);
        }
        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
            if (s[i] == '-')
                tmp = -1;
            else if (s[i] == '+')
                tmp = -2;
            else if (s[i] == '*')
                tmp = -3;
            else
                tmp = -4;
            ans.push_back(tmp);
        }
        else if (s[i] != ' ')
            return {};
    }
    return ans;
}

bool isRight (vector <int64_t> v) {
    int64_t len = v.size();
    bool div = 0;
    if (v.empty() || v[len - 1] < 0 || v[0] < -1)
        return 0;
    for (unsigned i = 0; i < len - 1; i++) {
        if (v[i] >= 0 && v[i + 1] >= 0)
            return 0;
        if (v[i] < 0 && v[i + 1] < -1)
            return 0;
        if (i + 2 < len && v[i] < 0 && v[i + 1] < 0 && v[i + 2] < 0)
            return 0;
        if (v[i] == -4)
            div = 1;
        else if (v[i] < 0 && v[i] > -3)
            div = 0;
        else if (v[i] == 0 && div)
            return 0;
    }
    if (div && !v[len - 1])
        return 0;
    return 1;
}

vector <int64_t> solveUnaryBinaryMinus (vector <int64_t> v) {
    vector <int64_t> ans = {};
    int64_t len = v.size();
    char sign = 1;
    for (unsigned i = 0; i < len; i++) {
        if (v[i] >= 0) {
            ans.push_back (v[i] * sign);
            sign = 1;
        }
        else if (i == 0 || (v[i] == -1 && v[i - 1] < 0))
            sign = -1;
        else if (i + 1 < len && v[i] == -1 &&
                 (v[i + 1] >= 0 || v[i + 1] == -1)) {
            if (v[i + 1] >= 0)
                sign = -1;
            else
                i++;
            ans.push_back(-2);
        }
        else
            ans.push_back (v[i]);
    }
    return ans;
}

int64_t calculate (vector <int64_t> v, unsigned j) {
    int64_t len = v.size();
    if (j + 1 < len) {
        if (v[j + 1] == -2)
            return v[j] + calculate (v, j + 2);
        int64_t tmp = v[j];
        while (j + 2 < len && v[j + 1] < -2) {
            if (v[j + 1] == -3)
                tmp *= v[j + 2];
            else
                tmp /= v[j + 2];
            j += 2;
        }
        if (j + 1 < len)
            return tmp + calculate (v, j + 2);
        else
            return tmp;
    }
    return v[j];
}

int main (int argc, char *argv[])
{
    if (argc != 2) {
        cout << "error";
        return 1;
    }
    string sin = argv[1];
    vector <int64_t> v = getTokens (sin);
    if (!isRight (v)) {
        cout << "error";
        return 1;
    }
    v = solveUnaryBinaryMinus (v);
    cout << calculate (v, 0);
    return 0;
}
