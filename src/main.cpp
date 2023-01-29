#include "CommandProcessor.h"
#include "ConsoleCommandWriter.h"
#include "FileCommandWriter.h"

#include <iostream>

using namespace Homework;

const std::size_t NUMBER_OF_PARAMS_MIN = 2;
const std::size_t BLOCK_SIZE_PARAM_INDEX = 1;

int main(int argc, char* argv[]) {
    if (argc < NUMBER_OF_PARAMS_MIN) {
        std::cerr << "Please provide a block size." << std::endl;
        return -1;
    }

    auto consoleWriter = std::make_shared<ConsoleCommandWriter>();
    consoleWriter->run();

    std::vector<std::shared_ptr<FlushCommandListener>> flushListeners = {
        consoleWriter, std::make_shared<FileCommandWriter>()
    };

    try {
        std::size_t blockSize = std::stoi(argv[BLOCK_SIZE_PARAM_INDEX]);
        CommandProcessor commandProcessor(flushListeners, blockSize);

        std::string command;
        while (std::getline(std::cin, command)) {
            commandProcessor.process(command);
        }
        if (std::cin.eof()) {
            commandProcessor.flush();
        }
    } catch (const std::logic_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
