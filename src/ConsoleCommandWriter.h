#pragma once

#include "AsyncCommandWriter.h"

namespace async {

    class ConsoleCommandWriter : public AsyncCommandWriter {
    public:
        ConsoleCommandWriter();

    protected:
        void write(const CommandBlock& commands, NumberOfThreads threadNumber) override;

    };

}
