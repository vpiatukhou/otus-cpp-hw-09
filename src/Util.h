#pragma once

#include "CommandBlock.h"

#include <ostream>

namespace async {

    void writeCommandsToStream(std::ostream& ostream, const CommandBlock& commands);

}
