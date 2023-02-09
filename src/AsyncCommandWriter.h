#pragma once

#include "FlushCommandListener.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

namespace async {

    /**
     * The base class for an asynchronous command writer.
     *
     * Encapsulates thread management.
     */
    class AsyncCommandWriter : public FlushCommandListener {
    public:
        AsyncCommandWriter(const AsyncCommandWriter&) = delete;
        AsyncCommandWriter(AsyncCommandWriter&&) = delete;

        AsyncCommandWriter& operator=(const AsyncCommandWriter&) = delete;
        AsyncCommandWriter& operator=(AsyncCommandWriter&&) = delete;

        /**
         * Starts threads.
         */
        void start();

        /**
         * Waits until remained command blocks have been written and stops the threads.
         *
         * ATTENTION: it is not guaranteed that the threads will be stopped immidiately.
         *
         * They may be stopped in some time after the method returns control.
         *
         * It is just guaranteed, that the threads will release the mutex and finish data processing.
         */
        void stop();

        void onFlush(const CommandBlock& commands) override;

    protected:
        using NumberOfThreads = unsigned int; //unsigned int has been chosen because it is returned by std::thread::hardware_concurrency()

        AsyncCommandWriter(NumberOfThreads numberOfThreads_);

        /**
         * Writes the given command block.
         *
         * @param commands      - the command block which will be written
         * @param threadNumber  - a number to identify the thread
         */
        virtual void write(const CommandBlock& commands, NumberOfThreads threadNumber) = 0;

    private:
        bool isCommandQueueEmpty();

        NumberOfThreads numberOfThreads;
        std::queue<CommandBlock> commandBlocks;
        std::mutex workerMutex;

        std::condition_variable continueProcessing;
        std::condition_variable blockProcessed;

        std::atomic<NumberOfThreads> numberOfFinishedThreads{0};
        std::atomic_bool isContinueProcessing{true};
    };

}
