#pragma once

#include <exception> // for exception_ptr
#include <iosfwd>    // for istream

#include "tl/expected.hpp" // for expected

namespace ocppi
{
namespace runtime
{
namespace config
{
namespace types
{
struct Config;
} // namespace types
} // namespace config
} // namespace runtime
} // namespace ocppi

namespace ocppi::runtime::config
{

struct ConfigLoader {
        [[nodiscard]]
        auto load(std::istream &in) const noexcept
                -> tl::expected<types::Config, std::exception_ptr>;
};

}
