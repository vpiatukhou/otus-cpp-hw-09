#pragma once

#include <queue>
#include <mutex>

namespace async {

class CommandQueue {
private:
    std::queue<std::vector<std::string>> queue;
    std::mutex queueMutex;
public:
    void push(const std::vector<std::string>& value);
    bool pop(std::vector<std::string>& out);
};

};
