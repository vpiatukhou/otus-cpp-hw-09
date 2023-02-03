#include "CommandQueue.h"

#include <utility>
#include <iterator>

namespace async {

void CommandQueue::push(const std::vector<std::string>& value) {
    std::lock_guard<std::mutex> lock(queueMutex);
    queue.push(value);
}

bool CommandQueue::pop(std::vector<std::string>& out) {
    std::lock_guard<std::mutex> lock(queueMutex);
    if (queue.empty()) {
        return false;
    }
    auto& block = queue.back();
    out.reserve(out.size() + block.size());
    std::move(std::begin(block), std::end(block), std::back_inserter(out));
    queue.pop();
    return true;
}

};
