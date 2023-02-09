#pragma once

#include "CommandProcessor.h"
#include "FlushCommandListener.h"
#include "handler_t.h"

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>

namespace async {

    /**
     * Encapsulates command processing functionality. 
     * 
     * This class was introduced in order to reduce a number of global variables in async.cpp.
     */
    class CommandProcessingFacade {
    public:
        /**
         * Creates a new CommandProcessor.
         * 
         * @return a unique handle which is used to identify the CommandProcessor
         */
        handle_t connect(std::size_t blockSize);

        /**
         * Receives the given command and passes it to CommandProcessor with the given handler.
         * 
         * @param handle - uniquely identifies CommandProcessor
         * @param command - a command to be processed
         */
        void receive(handle_t handle, std::string command);

        /**
         * Destroyes CommandProcessor with the given handler.
         * 
         * NOTE: this call may be slow if the given handler is the last one (there are no more handlers)
         *       because the program will wait until the remaining commands are processed.
         */
        void disconnect(handle_t handle);

    private:
        std::vector<std::shared_ptr<FlushCommandListener>> commandWriters;
        std::unordered_map<handle_t, std::unique_ptr<CommandProcessor>> processors;
        std::mutex processorMutex;

        void init();
        void cleanUp();
    };
}