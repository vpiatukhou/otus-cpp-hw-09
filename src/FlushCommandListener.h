#pragma once

#include "CommandBlock.h"

#include <string>
#include <vector>

namespace async {

    class FlushCommandListener {
    public:
        virtual ~FlushCommandListener() {
        }

        virtual void onFlush(const CommandBlock& commands) = 0;
    };

}
