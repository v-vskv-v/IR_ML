#include <iostream>
#include <chrono>

enum { N = 10000 };

class Timer {
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us =
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};

using namespace std;

int main (void){

    int** a = (int**) malloc (N * sizeof (int*));
    a[0] = (int*) malloc (N * N * sizeof (int));

    for (int i = 1; i < N; i++)
        a[i] = a[0] + N * i;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            a[i][j] = i + j;

    {
        unsigned long long sum = 0;
        Timer s;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                sum += a[i][j];
    }
    {
        unsigned long long sum = 0;
        Timer s;
        for (int j = 0; j < N; j++)
            for (int i = 0; i < N; i++)
                sum += a[i][j];
    }

    return 0;
}

