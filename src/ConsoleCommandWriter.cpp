#include "ConsoleCommandWriter.h"

#include <iostream>
#include <thread>

namespace Homework {

void ConsoleCommandWriter::onFlush(const std::vector<std::string>& commands) {
    commandBlocks.push(commands);
}

void ConsoleCommandWriter::run() {
    std::thread executor([this]() {
        std::vector<std::string> commandBlock;
        while (true) {
            if (commandBlocks.pop(commandBlock)) {
                writeToStream(std::cout, commandBlock);
                std::cout << std::endl;
            }
        }
    });

    executor.detach();
}

};
