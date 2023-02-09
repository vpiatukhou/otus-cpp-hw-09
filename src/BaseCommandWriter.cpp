#include "BaseCommandWriter.h"

#include <thread>

namespace async {

    const std::string BaseCommandWriter::BLOCK_PREFIX = "bulk: ";
    const std::string BaseCommandWriter::COMMAND_DELIMITER = ", ";

    BaseCommandWriter::BaseCommandWriter(NumberOfThreads numberOfThreads_) : numberOfThreads(numberOfThreads_) {
    }

    BaseCommandWriter::~BaseCommandWriter() {
    }

    void BaseCommandWriter::start() {
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
            std::thread t(worker, i);
            t.detach();
        }
    }

    bool BaseCommandWriter::isCommandQueueEmpty() {
        std::lock_guard<std::mutex> lock(workerMutex);
        return commandBlocks.empty();
    }

    void BaseCommandWriter::stop() {
        isContinueProcessing = false;
        continueProcessing.notify_all();

        //do not destroy object until all remained commands are processed
        std::unique_lock<std::mutex> lock(workerMutex);
        blockProcessed.wait(lock, [this] { return numberOfFinishedThreads == numberOfThreads; });
    }

    void BaseCommandWriter::onFlush(const CommandBlock& commands) {
        std::lock_guard<std::mutex> lock(workerMutex);
        commandBlocks.push(commands);
    }

    void BaseCommandWriter::writeToStream(std::ostream& ostream, const CommandBlock& commands) const {
        ostream << BLOCK_PREFIX << commands[0];
        for (std::size_t i = 1; i < commands.size(); ++i) {
            ostream << COMMAND_DELIMITER << commands[i];
        }
    }

}
