#include "FileCommandWriter.h"

#include <chrono>
#include <fstream>
#include <mutex>
#include <thread>

namespace async {

    const std::string FILENAME_PREFIX = "bulk";
    const std::string FILE_EXTENSION = ".log";
    const std::string FILENAME_SUFFIX_1 = "-t1";
    const std::string FILENAME_SUFFIX_2 = "-t2";

    const unsigned short NUMBER_OF_THREADS = 2;

    FileCommandWriter::FileCommandWriter() {
        auto worker = [this](const std::string& filenameSuffix) {
            CommandBlock commandBlock;
            while (isContinue) {
                commandBlocks.waitForData(commandBlock);

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

            ++numberOfFinishedThreads;

            allWorkersDoneCondition.notify_all();
        };

        std::thread t1(worker, FILENAME_SUFFIX_1);
        std::thread t2(worker, FILENAME_SUFFIX_2);

        t1.detach();
        t2.detach();
    }

    FileCommandWriter::~FileCommandWriter() {
        isContinue = false; //break the loop in the threads

        std::mutex destructorMutex;
        std::unique_lock<std::mutex> lock(destructorMutex);
        //wait until the threads process remained commands
        allWorkersDoneCondition.wait(lock, [this] { return numberOfFinishedThreads == NUMBER_OF_THREADS; });
    }

    void FileCommandWriter::onFlush(const CommandBlock& commands) {
        commandBlocks.push(commands);
    }

}
