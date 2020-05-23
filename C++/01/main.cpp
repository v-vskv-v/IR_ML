#include "numbers.dat"
#include <iostream>

using namespace std;

bool isPrime(int num)
{
    if (num <= 1)
        return 0;
    for (int i = 2; i * i <= num; i++)
        if (!(num % i))
            return 0;
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc == 1 || (argc + 1) & 1)
        return -1;

    for (int i = 1; i < argc; i += 2) {
        int lx = atoi(argv[i]);
        int rx = atoi(argv[i + 1]);
        int left, right;
        left = right = -1;

        for (int i = 0; i < Size; i++) {
            if (lx == Data[i]) {
                left = i;
                break;
            }
        }

        for (int i = Size - 1; i >= 0; i--) {
            if (rx == Data[i]) {
                right = i;
                break;
            }
        }

        int ans = 0;
        if (left != -1 && right != -1) {
            for (int i = left; i <= right; i++)
                if (isPrime(Data[i]))
                    ans++;
        }
        cout << ans << '\n';
    }

    return 0;
}
