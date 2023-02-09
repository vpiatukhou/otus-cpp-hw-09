#pragma once

#include "FlushCommandListener.h"

#include <ostream>
#include <string>
#include <vector>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace async {

    /**
     * The base class for asynchronous command writers.
     *
     * Encapsulates thread management.
     */
    class BaseCommandWriter : public FlushCommandListener {
    public:
        BaseCommandWriter(const BaseCommandWriter&) = delete;
        BaseCommandWriter(BaseCommandWriter&&) = delete;

        BaseCommandWriter& operator=(const BaseCommandWriter&) = delete;
        BaseCommandWriter& operator=(BaseCommandWriter&&) = delete;

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

        BaseCommandWriter(NumberOfThreads numberOfThreads_);
        virtual ~BaseCommandWriter();

        /**
         * Writes the given command block.
         *
         * @param commands      - the command block which will be written
         * @param threadNumber  - a number to identify the thread
         */
        virtual void write(const CommandBlock& commands, NumberOfThreads threadNumber) = 0;
        void writeToStream(std::ostream& ostream, const CommandBlock& commands) const;

    private:
        static const std::string BLOCK_PREFIX;
        static const std::string COMMAND_DELIMITER;

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
