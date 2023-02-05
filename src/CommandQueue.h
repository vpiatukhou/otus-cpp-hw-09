#pragma once

#include "CommandBlock.h"

#include <condition_variable>
#include <queue>
#include <mutex>

namespace async {

    /**
     * A queue which contains blocks of commands. All methods are thread-safe.
     */
    class CommandQueue {
    public:
        /**
         * Pushes a given command block into the queue.
         * 
         * @param commandBlock - a command block
         */
        void push(const CommandBlock& commandBlock);

        /**
         * Removes the command block from the queue. If the queue is empty, waits for the data (the current thread is blocked).
         * 
         * @param target - the command block will be written into this parameter.
         */
        void waitForData(CommandBlock& target);

    private:
        std::queue<CommandBlock> queue;
        std::mutex queueMutex;
        std::condition_variable queueNotEmptyCondition;
    };

}
