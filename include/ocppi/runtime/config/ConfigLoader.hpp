#pragma once

#include <exception>
#include <iosfwd>

#include "tl/expected.hpp"

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
