#pragma once

#include "BaseCommandWriter.h"
#include "CommandQueue.h"

namespace Homework {

class FileCommandWriter : public BaseCommandWriter {
public:
    void onFlush(const std::vector<std::string>& commands) override;

private:
    CommandQueue commandBlocks;

    void writeBlocksToFile(const std::string& filenameSuffix);
    void run();
};

};
