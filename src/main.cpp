#include "async.h"

#include <iostream>
#include <string>
#include <thread>

/**
 * All commands are sent from a single thread.
 */
void testSingleClientThread() {
    auto h1 = async::connect(3);

    async::receive(h1, "1-1", 3);
    async::receive(h1, "1-2", 3);
    async::receive(h1, "1-3", 3); //the end of the block: 1-1, 1-2, 1-3

    auto h2 = async::connect(2);

    async::receive(h1, "1-4", 3);
    async::receive(h1, "1-5", 3);

    async::receive(h2, "2-1", 3);
    async::receive(h2, "2-2", 3); //end of block: 2-1, 2-2

    auto h3 = async::connect(4);
    async::receive(h1, "1-6", 3); //end of block: 1-4, 1-5, 1-6

    async::receive(h3, "3-1", 3);
    async::receive(h3, "3-2", 3);

    async::receive(h1, "1-7", 3);
    async::receive(h1, "1-8", 3); //end of block: 1-7, 1-8

    async::disconnect(h1);
    async::disconnect(h2);

    async::receive(h3, "3-3", 3);
    async::receive(h3, "3-4", 3); //end of block: 3-1, 3-2, 3-3, 3-4
    async::receive(h3, "{", 1);
    async::receive(h3, "3-5", 3); //end of block: 3-5
    async::receive(h3, "}", 1);
    async::receive(h3, "3-6", 3); //end of block: 3-6

    async::disconnect(h3);
}

/**
 * A lot of commands are sent from multiple threads.
 */
void testMultipleClientThreads() {
    const std::size_t numberOfCommandsPerThread = 1000;

    auto worker = [numberOfCommandsPerThread](std::size_t threadNumber, std::size_t blockSize) {
        auto handle = async::connect(blockSize);
        for (std::size_t i = 0; i < numberOfCommandsPerThread; ++i) {
            std::string command = std::to_string(threadNumber);
            command += '-';
            command += std::to_string(i);
            async::receive(handle, command.data(), command.size());
        }
        async::disconnect(handle);
    };

    std::thread t1(worker, 0, 1);
    std::thread t2(worker, 1, 3);
    std::thread t3(worker, 2, 5);

    t1.join();
    t2.join();
    t3.join();
}

int main(int argc, char* argv[]) {
    std::cout << "===\n=== Send commands from a single thread ===\n===\n" << std::endl;
    testSingleClientThread();

    std::cout << "\n===\n=== Send commands from multiple threads ===\n===\n" << std::endl;
    testMultipleClientThreads();
    return 0;
}
