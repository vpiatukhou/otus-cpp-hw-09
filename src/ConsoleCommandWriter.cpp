#include "ConsoleCommandWriter.h"

#include <iostream>
#include <thread>
#include <mutex>

namespace async {

    ConsoleCommandWriter::ConsoleCommandWriter() {
        std::thread worker([this]() {
            CommandBlock commandBlock;
            while (isContinue) {
                commandBlocks.waitForData(commandBlock);
                writeToStream(std::cout, commandBlock);
                std::cout << std::endl;
            }

            isWorkerFinished = true;

            workerDoneCondition.notify_all();
        });

        worker.detach();
    }

    ConsoleCommandWriter::~ConsoleCommandWriter() {
        isContinue = false;

        std::mutex m;
        std::unique_lock<std::mutex> lock(m);
        workerDoneCondition.wait(lock, [this] { return isWorkerFinished == true; });
    }

    void ConsoleCommandWriter::onFlush(const CommandBlock& commands) {
        commandBlocks.push(commands);
    }

}
