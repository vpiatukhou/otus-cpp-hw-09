#pragma once

#include "CommandQueue.h"

namespace Homework {

void CommandQueue::push(const std::vector<std::string>& value) {
    std::lock_guard<std::mutex> lock(queueMutex);
    queue.push(value);
}

bool CommandQueue::pop(std::vector<std::string>& out) {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (queue.empty()) {
        return false;
    }
    queue.back();
    queue.pop();
    return true;
}

};
