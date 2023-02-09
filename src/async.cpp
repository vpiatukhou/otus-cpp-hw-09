#include "async.h"
#include "CommandProcessor.h"
#include "ConsoleCommandWriter.h"
#include "FileCommandWriter.h"

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_map>

namespace async {

    std::vector<std::shared_ptr<FlushCommandListener>> commandWriters;
    std::unordered_map<handle_t, std::unique_ptr<CommandProcessor>> processors;
    std::mutex processorMutex;

    handle_t connect(std::size_t blockSize) {
        std::lock_guard<std::mutex> lock(processorMutex);
        if (commandWriters.empty()) {
            //if connect is called at the first time, creates the writers...
            commandWriters.push_back(std::make_shared<ConsoleCommandWriter>());
            commandWriters.push_back(std::make_shared<FileCommandWriter>());

            //... and run the threads
            for (auto& listener : commandWriters) {
                static_cast<AsyncCommandWriter*>(listener.get())->start();
            }
        }

        auto processor = std::make_unique<CommandProcessor>(commandWriters, blockSize);
        handle_t handle = processor.get(); //the pointer is used as a unique handle

        processors[handle] = std::move(processor);

        return handle;
    }

    void receive(handle_t handle, const char* data, std::size_t dataSize) {
        std::lock_guard<std::mutex> lock(processorMutex);

        std::string command(data, dataSize);
        processors[handle]->process(command);
    }

    void disconnect(handle_t handle) {
        std::lock_guard<std::mutex> lock(processorMutex);

        //flush the last block of commands and delete the processor
        processors[handle]->flush();
        processors.erase(handle);

        if (processors.empty()) { 
            //process remained commands, stop all threads and delete listeners
            for (auto& listener : commandWriters) {
                static_cast<AsyncCommandWriter*>(listener.get())->stop();
            }
            commandWriters.clear();
        }
    }
}
