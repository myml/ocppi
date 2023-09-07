#pragma once

#include <exception>

#define EWRAP(e)                                                  \
        if (ocppi::utils::ewrap::wrap) {                          \
                std::throw_with_nested(e);                        \
        } else {                                                  \
                std::rethrow_exception(std::current_exception()); \
        }

namespace ocppi::utils::ewrap
{

static bool wrap = false;

}
