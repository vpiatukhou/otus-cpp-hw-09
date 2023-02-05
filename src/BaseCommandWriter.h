#pragma once

#include "FlushCommandListener.h"

#include <ostream>
#include <string>
#include <vector>

namespace async {

    class BaseCommandWriter : public FlushCommandListener {
    protected:
        void writeToStream(std::ostream& ostream, const CommandBlock& commands) const;
    };

}
