#include "ConsoleCommandWriter.h"

#include <iostream>

namespace async {

    ConsoleCommandWriter::ConsoleCommandWriter() : BaseCommandWriter(1) {
    }

    void ConsoleCommandWriter::write(const CommandBlock& commands, NumberOfThreads threadNumber) {
        writeToStream(std::cout, commands);
        std::cout << std::endl;
    }
}
