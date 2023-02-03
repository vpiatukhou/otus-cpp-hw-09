#include "async.h"
#include "CommandProcessor.h"
#include "ConsoleCommandWriter.h"
#include "FileCommandWriter.h"

#include <iostream>
#include <unordered_map>
#include <memory>

namespace async {

    std::vector<std::shared_ptr<FlushCommandListener>> flushListeners = { 
        std::make_shared<ConsoleCommandWriter>(),
        std::make_shared<FileCommandWriter>() };

    std::unordered_map<handle_t, std::unique_ptr<CommandProcessor>> processors;

    handle_t connect(std::size_t blockSize) {
        CommandProcessor* processorPtr = new CommandProcessor(flushListeners, blockSize);
        handle_t handle = processorPtr; //an address is used as a unique handle

        processors[handle] = std::unique_ptr<CommandProcessor>(processorPtr);
        return handle;
    }

    void receive(handle_t handle, const char* data, std::size_t dataSize) {
        std::string block(data, dataSize);
        processors[handle]->process(block);
    }

    void disconnect(handle_t handle) {
        processors[handle]->flush();
        processors.erase(handle);
    }
};
