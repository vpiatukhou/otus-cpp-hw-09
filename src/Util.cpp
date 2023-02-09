#include "Util.h"

const std::string COMMAND_BLOCK_PREFIX = "bulk: ";
const std::string COMMAND_DELIMITER = ", ";

void async::writeCommandsToStream(std::ostream& ostream, const CommandBlock& commands) {
    ostream << COMMAND_BLOCK_PREFIX << commands[0];
    for (std::size_t i = 1; i < commands.size(); ++i) {
        ostream << COMMAND_DELIMITER << commands[i];
    }
}