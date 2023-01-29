#pragma once

#include "FlushCommandListener.h"

#include <ostream>
#include <string>
#include <vector>

namespace Homework {

class BaseCommandWriter : public FlushCommandListener {
protected:
    void writeToStream(std::ostream& ostream, const std::vector<std::string>& commands) const;
};

};
