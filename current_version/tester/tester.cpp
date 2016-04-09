#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <string>
#include <thread>
#include "viewer_client.h"

struct Visi {
    void operator()() {
        Visualizer::Instance().Initialize(0, 0);
        Visualizer::Instance().Visualize();
    }
};

void GameThread() {
    double x = 0, y = 0;
    while (1) {
        //std::cin >> x >> y;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        ++x;
        ++y;
        Visualizer::Instance().UpdateScene(x, y);
    }
}

int main() {

    std::thread threadVisi((Visi()));
    threadVisi.detach();
    GameThread();

    system("pause");
    return 0;
}
