#pragma once

#include "CommandQueue.h"
#include "BaseCommandWriter.h"

namespace async {

class ConsoleCommandWriter : public BaseCommandWriter {
public:
    ConsoleCommandWriter();

    void onFlush(const std::vector<std::string>& commands) override;

private:
    CommandQueue commandBlocks;
};

};
