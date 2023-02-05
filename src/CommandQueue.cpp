#include "CommandQueue.h"

#include <utility>
#include <iterator>

namespace async {

    void CommandQueue::push(const CommandBlock& commandBlock) {
        if (!commandBlock.empty()) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                queue.push(commandBlock);
            }
            queueNotEmptyCondition.notify_one();
        }
    }

    void CommandQueue::waitForData(CommandBlock& target) {
        bool isRead = false;
        while (!isRead) {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueNotEmptyCondition.wait(lock, [this] { return !queue.empty(); });

            auto& commandBlock = queue.back();
            if (!commandBlock.empty()) {
                target.reserve(target.size() + commandBlock.size());
                std::move(std::begin(commandBlock), std::end(commandBlock), std::back_inserter(target));
                queue.pop();

                isRead = true;
            }
        }
    }

}
