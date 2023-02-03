#pragma once

#include <string>
#include <vector>

namespace async {

class FlushCommandListener {
public:
    virtual ~FlushCommandListener() {
    }

    virtual void onFlush(const std::vector<std::string>& commands) = 0;
};

};
