#pragma once

#include "CommandBlock.h"

namespace async {

    class FlushCommandListener {
    public:
        virtual ~FlushCommandListener() {
        }

        virtual void onFlush(const CommandBlock& commands) = 0;
    };

}
