#pragma once

#include "AsyncCommandWriter.h"

namespace async {

    class FileCommandWriter : public AsyncCommandWriter {
    public:
        FileCommandWriter();

    protected:
        void write(const CommandBlock& commands, NumberOfThreads threadNumber) override;

    private:
        static const std::string FILENAME_PREFIX;
        static const std::string FILENAME_THREAD;
        static const std::string FILE_EXTENSION;

        /**
         * @return a filename in the format: <timespamp sec>-th-<thread number>[-<file number>].log
         */
        std::string createFilename(NumberOfThreads threadNumber) const;

    };

}
