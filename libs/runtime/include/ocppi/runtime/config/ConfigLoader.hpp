#pragma once

#include <bitset>    // for bitset, bitset<>::reference
#include <cstddef>   // for size_t
#include <exception> // for exception
#include <iosfwd>    // for istream
#include <list>
#include <optional> // for optional

#include "ocppi/runtime/config/types/Config.hpp"
#include "tl/expected.hpp"

namespace ocppi::runtime::config
{

struct ConfigLoader {
        auto load(std::istream &in) const noexcept
                -> tl::expected<types::Config, std::exception_ptr>;
};

}
