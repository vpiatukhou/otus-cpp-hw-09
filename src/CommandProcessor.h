#pragma once

#include "FlushCommandListener.h"

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

namespace Homework {

class CommandProcessor {

public:
    CommandProcessor(std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_, std::size_t blockSize_);

    void process(const std::string& command);
    void flush();

private:
    std::size_t blockSize;
    std::vector<std::string> commandBlock;
    size_t openedDynamicBlockCounter = 0;

    std::vector<std::shared_ptr<FlushCommandListener>> flushListeners;
};

};
