#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

enum { N = 500000 };

std::mutex m;
std::condition_variable ping_ready;
std::condition_variable pong_ready;
bool ping = 1;
bool pong = 0;

void Ping() {
    for (int i = 0; i < N; ++i) {
        std::unique_lock<std::mutex> lock(m);
        while(!ping) {
            ping_ready.wait(lock);
        }
        std::cout << "ping\n";
        ping = 0;
        pong = 1;
        lock.unlock();
        pong_ready.notify_one();
    }
}

void Pong() {
    for (int i = 0; i < N; ++i) {
        std::unique_lock<std::mutex> lock(m);
        while(!pong) {
            pong_ready.wait(lock);
        }
        std::cout << "pong\n";
        ping = 1;
        pong = 0;
        lock.unlock();
        ping_ready.notify_one();
    }
}
int main()
{
    std::thread ping(Ping);
    std::thread pong(Pong);
    ping.join();
    pong.detach();
    return 0;
}
