#include "BaseCommandWriter.h"

namespace Homework {

const std::string BLOCK_PREFIX = "bulk: ";
const std::string COMMAND_DELIMITER = ", ";

void BaseCommandWriter::writeToStream(std::ostream& ostream, const std::vector<std::string>& commands) const {
    ostream << BLOCK_PREFIX << commands[0];
    for (std::size_t i = 1; i < commands.size(); ++i) {
        ostream << COMMAND_DELIMITER << commands[i];
    }
}

};
