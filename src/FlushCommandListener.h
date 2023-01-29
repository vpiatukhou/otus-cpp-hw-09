#pragma once

#include <string>
#include <vector>

namespace Homework {

class FlushCommandListener {
public:
    virtual ~FlushCommandListener() {
    }

    virtual void onFlush(const std::vector<std::string>& commands) = 0;
};

};
