#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector <int64_t> getTokens (const string &sin){
    vector <int64_t> ans = {};
    int64_t tmp = 0;
    int64_t len = sin.size();
    char sign = 1;
    for (unsigned i = 0; i < len; i++, tmp = 0) {
        if (sin[i] >= '0' && sin[i] <= '9') {
            while (i < len && sin[i] >= '0' && sin[i] <= '9') {
                tmp *= 10;
                tmp += sin[i++] - '0';
            }
            tmp *= sign;
            sign = 1;
            i--;
            ans.push_back(tmp);
        }
        else if (sin[i] == '+' || sin[i] == '-' || sin[i] == '*' || sin[i] == '/') {
            if (sin[i] == '-')
                tmp = -1;
            else if (sin[i] == '+')
                tmp = -2;
            else if (sin[i] == '*')
                tmp = -3;
            else
                tmp = -4;
            ans.push_back(tmp);
        }
        else if (sin[i] != ' ')
            return {};
    }
    return ans;
}

bool isRight (const vector <int64_t> &tokens) {
    int64_t len = tokens.size();
    bool div = 0;
    if (tokens.empty() || tokens[len - 1] < 0 || tokens[0] < -1)
        return 0;
    for (unsigned i = 0; i < len - 1; i++) {
        if (tokens[i] >= 0 && tokens[i + 1] >= 0)
            return 0;
        if (tokens[i] < 0 && tokens[i + 1] < -1)
            return 0;
        if (i + 2 < len && tokens[i] < 0 && tokens[i + 1] < 0 && tokens[i + 2] < 0)
            return 0;
        if (tokens[i] == -4)
            div = 1;
        else if (tokens[i] < 0 && tokens[i] > -3)
            div = 0;
        else if (tokens[i] == 0 && div)
            return 0;
    }
    if (div && !tokens[len - 1])
        return 0;
    return 1;
}

vector <int64_t> solveUnaryBinaryMinus (const vector <int64_t> &tokens) {
    vector <int64_t> ans = {};
    int64_t len = tokens.size();
    char sign = 1;
    for (unsigned i = 0; i < len; i++) {
        if (tokens[i] >= 0) {
            ans.push_back (tokens[i] * sign);
            sign = 1;
        }
        else if (i == 0 || (tokens[i] == -1 && tokens[i - 1] < 0))
            sign = -1;
        else if (i + 1 < len && tokens[i] == -1 &&
                 (tokens[i + 1] >= 0 || tokens[i + 1] == -1)) {
            if (tokens[i + 1] >= 0)
                sign = -1;
            else
                i++;
            ans.push_back(-2);
        }
        else
            ans.push_back (tokens[i]);
    }
    return ans;
}

int64_t calculate (const vector <int64_t> &tokens, unsigned j) {
    int64_t len = tokens.size();
    if (j + 1 < len) {
        if (tokens[j + 1] == -2)
            return tokens[j] + calculate (tokens, j + 2);
        int64_t tmp = tokens[j];
        while (j + 2 < len && tokens[j + 1] < -2) {
            if (tokens[j + 1] == -3)
                tmp *= tokens[j + 2];
            else
                tmp /= tokens[j + 2];
            j += 2;
        }
        if (j + 1 < len)
            return tmp + calculate (tokens, j + 2);
        else
            return tmp;
    }
    return tokens[j];
}

int main (int argc, char *argv[])
{
    if (argc != 2) {
        cout << "error";
        return 1;
    }
    vector <int64_t> tokens = getTokens (argv[1]);
    if (!isRight (tokens)) {
        cout << "error";
        return 1;
    }
    tokens = solveUnaryBinaryMinus (tokens);
    cout << calculate (tokens, 0);
    return 0;
}
