/**
 * An entry point of the libasync.lib
 */
#pragma once

#include "handler_t.h"

#include <cstddef>

namespace async {

    handle_t connect(std::size_t blockSize);
    void receive(handle_t handle, const char* data, std::size_t dataSize);
    void disconnect(handle_t handle);
}
