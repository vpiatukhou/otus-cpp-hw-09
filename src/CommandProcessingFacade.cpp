#include "CommandProcessingFacade.h"
#include "ConsoleCommandWriter.h"
#include "FileCommandWriter.h"

#include <string>

namespace async {

    handle_t CommandProcessingFacade::connect(std::size_t blockSize) {
        std::lock_guard<std::mutex> lock(processorMutex);
        if (commandWriters.empty()) {
            //"connect" has been called the first time => do initialization
            init();
        }

        auto processor = std::make_unique<CommandProcessor>(commandWriters, blockSize);
        handle_t handle = processor.get(); //use the pointer as a unique handle

        processors[handle] = std::move(processor);

        return handle;
    }

    void CommandProcessingFacade::receive(handle_t handle, std::string command) {
        std::lock_guard<std::mutex> lock(processorMutex);
        processors[handle]->process(command);
    }

    void CommandProcessingFacade::disconnect(handle_t handle) {
        std::lock_guard<std::mutex> lock(processorMutex);

        //flush the last block of commands and delete the processor
        processors[handle]->flush();
        processors.erase(handle);

        if (processors.empty()) {
            cleanUp();
        }
    }

    void CommandProcessingFacade::init() {
        //create writes...
        commandWriters.push_back(std::make_shared<ConsoleCommandWriter>());
        commandWriters.push_back(std::make_shared<FileCommandWriter>());

        //... and run the threads
        for (auto& listener : commandWriters) {
            static_cast<AsyncCommandWriter*>(listener.get())->start();
        }
    }

    void CommandProcessingFacade::cleanUp() {
        //process remained commands, stop all threads and delete listeners
        for (auto& listener : commandWriters) {
            static_cast<AsyncCommandWriter*>(listener.get())->stop();
        }
        commandWriters.clear();
    }
}
