#include "BaseCommandWriter.h"

namespace async {

    const std::string BLOCK_PREFIX = "bulk: ";
    const std::string COMMAND_DELIMITER = ", ";

    void BaseCommandWriter::writeToStream(std::ostream& ostream, const CommandBlock& commands) const {
        ostream << BLOCK_PREFIX << commands[0];
        for (std::size_t i = 1; i < commands.size(); ++i) {
            ostream << COMMAND_DELIMITER << commands[i];
        }
    }

}
