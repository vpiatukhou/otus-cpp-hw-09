#pragma once

#include "CommandQueue.h"
#include "BaseCommandWriter.h"

#include <atomic>
#include <condition_variable>

namespace async {

    class ConsoleCommandWriter : public BaseCommandWriter {
    public:
        ConsoleCommandWriter();
        ~ConsoleCommandWriter();

        void onFlush(const CommandBlock& commands) override;

    private:
        CommandQueue commandBlocks;

        std::atomic_bool isContinue = true;
        std::atomic_bool isWorkerFinished = false;
        std::condition_variable workerDoneCondition;
    };

}
