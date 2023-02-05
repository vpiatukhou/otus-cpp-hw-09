#pragma once

#include "BaseCommandWriter.h"
#include "CommandQueue.h"

#include <atomic>
#include <condition_variable>

namespace async {

    class FileCommandWriter : public BaseCommandWriter {
    public:
        FileCommandWriter();
        ~FileCommandWriter();

        void onFlush(const CommandBlock& commands) override;

    private:
        CommandQueue commandBlocks;
        std::condition_variable allWorkersDoneCondition;
        std::atomic_bool isContinue = true;
        std::atomic_ushort numberOfFinishedThreads = 0;
    };

}
