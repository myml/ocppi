#include <stddef.h> // for size_t

#include <bitset>    // for bitset, bitset<>::reference
#include <exception> // for exception
#include <iosfwd>    // for istream
#include <optional>  // for optional

#include "ocppi/runtime/config/types/Config.hpp"

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

enum class LoadOption { IgnoreIncompatibleVersionError, _ };

using LoadMode = std::bitset<static_cast<size_t>(LoadOption::_)>;

[[nodiscard]]
auto load(std::istream &, types::Config *,
          const LoadMode &mode = LoadMode()) noexcept
        -> std::optional<std::exception>;

}
