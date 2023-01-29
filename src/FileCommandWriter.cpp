#include "FileCommandWriter.h"

#include <chrono>
#include <fstream>
#include <thread>

namespace Homework {

const std::string FILENAME_PREFIX = "bulk";
const std::string FILE_EXTENSION = ".log";
const std::string FILENAME_SUFFIX_1 = "-t1";
const std::string FILENAME_SUFFIX_2 = "-t2";

void FileCommandWriter::onFlush(const std::vector<std::string>& commands) {
    commandBlocks.push(commands);
}

void FileCommandWriter::run() {
    auto executor = [this](std::string filenameSuffix) {
        std::vector<std::string> commandBlock;
        while (true) { //poll the queue in the infinite loop
            if (commandBlocks.pop(commandBlock)) { //if there is a new command block, write it to file
                auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
                auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count();

                std::string filename = FILENAME_PREFIX;
                filename += std::to_string(timestamp);
                filename += filenameSuffix;
                filename += FILE_EXTENSION;

                std::ofstream file(filename);
                writeToStream(file, commandBlock);
                file.flush();
                file.close();

                commandBlock.clear();
            }
        }
    };

    std::thread t1(executor, FILENAME_SUFFIX_1);
    std::thread t2(executor, FILENAME_SUFFIX_2);

    t1.detach();
    t2.detach();
}

};
