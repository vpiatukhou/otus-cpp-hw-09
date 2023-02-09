#include "CommandProcessor.h"

#include <stdexcept>
#include <thread>
#include <iostream>

namespace async {

    const std::string BEGIN_DYNAMIC_BLOCK_COMMAND = "{";
    const std::string END_DYNAMIC_BLOCK_COMMAND = "}";

    CommandProcessor::CommandProcessor(std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_, std::size_t blockSize_)
        : flushListeners(flushListeners_), blockSize(blockSize_) {
        if (blockSize_ == 0) {
            throw std::invalid_argument("Block size must be greater than 0.");
        }
        commandBlock.reserve(blockSize_);
    }

    void CommandProcessor::process(const std::string& command) {
        if (command == BEGIN_DYNAMIC_BLOCK_COMMAND) {
            flush();
            ++openedDynamicBlockCounter;
        } else if (command == END_DYNAMIC_BLOCK_COMMAND) {
            --openedDynamicBlockCounter;
            flush();
        } else {
            commandBlock.push_back(command);
            if (commandBlock.size() == blockSize) {
                flush();
            }
        }
    }

    void CommandProcessor::flush() {
        //check if there is no an opened dynamic block
        if (openedDynamicBlockCounter == 0 && !commandBlock.empty()) {
            for (auto& listener : flushListeners) {
                listener->onFlush(commandBlock);
            }
            commandBlock.clear();
        }
    }

}
