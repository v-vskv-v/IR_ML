#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

enum { N = 500000 };

std::mutex m;
std::condition_variable ready;
bool check = true;

void Ping()
{
    for (int i = 0; i < N; ++i) {
        std::unique_lock<std::mutex> lock(m);
        while (!check) {
            ready.wait(lock);
        }
        std::cout << "ping\n";
        check = false;
        ready.notify_one();
    }
}

void Pong()
{
    for (int i = 0; i < N; ++i) {
        std::unique_lock<std::mutex> lock(m);
        while (check) {
            ready.wait(lock);
        }
        std::cout << "pong\n";
        check = true;
        ready.notify_one();
    }
}

int main()
{
    std::thread ping(Ping);
    std::thread pong(Pong);
    ping.join();
    pong.join();
    return 0;
}
