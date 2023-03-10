#include "ConsoleCommandWriter.h"
#include "Util.h"

#include <iostream>

namespace async {

    ConsoleCommandWriter::ConsoleCommandWriter() : AsyncCommandWriter(1) {
    }

    void ConsoleCommandWriter::write(const CommandBlock& commands, NumberOfThreads threadNumber) {
        writeCommandsToStream(commands, std::cout);
        std::cout << std::endl;
    }
}
