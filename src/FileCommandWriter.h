#pragma once

#include "BaseCommandWriter.h"
#include "CommandQueue.h"

namespace async {

class FileCommandWriter : public BaseCommandWriter {
public:
    FileCommandWriter();

    void onFlush(const std::vector<std::string>& commands) override;

private:
    CommandQueue commandBlocks;
};

};
