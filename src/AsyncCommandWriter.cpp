#include "AsyncCommandWriter.h"

#include <thread>

namespace async {

    AsyncCommandWriter::AsyncCommandWriter(NumberOfThreads numberOfThreads_) : numberOfThreads(numberOfThreads_) {
    }

    void AsyncCommandWriter::start() {
        auto worker = [this](NumberOfThreads threadNumber) {
            CommandBlock block;

            auto hasCommandsOrStopThread = [this] { return !commandBlocks.empty() || !isContinueProcessing; };

            while (isContinueProcessing || !isCommandQueueEmpty()) {
                {
                    std::unique_lock<std::mutex> lock(workerMutex);
                    continueProcessing.wait(lock, hasCommandsOrStopThread);

                    if (!commandBlocks.empty()) {
                        block = std::move(commandBlocks.front());
                        commandBlocks.pop();
                    }
                }

                if (!block.empty()) {
                    write(block, threadNumber);
                }
            }
            ++numberOfFinishedThreads;
            blockProcessed.notify_all();
        };

        for (NumberOfThreads i = 0; i < numberOfThreads; ++i) {
            std::thread newThread(worker, i);
            newThread.detach();
        }
    }

    void AsyncCommandWriter::stop() {
        isContinueProcessing = false;
        continueProcessing.notify_all();

        //do not destroy object until all remained commands are processed
        std::unique_lock<std::mutex> lock(workerMutex);
        blockProcessed.wait(lock, [this] { return numberOfFinishedThreads == numberOfThreads; });
    }

    void AsyncCommandWriter::onFlush(const CommandBlock& commands) {
        std::lock_guard<std::mutex> lock(workerMutex);
        commandBlocks.push(commands);
    }

    bool AsyncCommandWriter::isCommandQueueEmpty() {
        std::lock_guard<std::mutex> lock(workerMutex);
        return commandBlocks.empty();
    }

}
