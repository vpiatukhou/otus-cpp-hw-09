#include "ConsoleCommandWriter.h"
#include "Util.h"

#include <iostream>

namespace async {

    ConsoleCommandWriter::ConsoleCommandWriter() : AsyncCommandWriter(1) {
    }

    void ConsoleCommandWriter::write(const CommandBlock& commands, NumberOfThreads threadNumber) {
        writeCommandsToStream(std::cout, commands);
        std::cout << std::endl;
    }
}
