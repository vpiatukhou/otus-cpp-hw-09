#pragma once

#include "BaseCommandWriter.h"

namespace async {

    class ConsoleCommandWriter : public BaseCommandWriter {
    public:
        ConsoleCommandWriter();

    protected:
        void write(const CommandBlock& commands, NumberOfThreads threadNumber) override;

    };

}
