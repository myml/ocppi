#include "ocppi/runtime/config/load.hpp"

#include <initializer_list> // for initial...
#include <map>              // for operator==
#include <stdexcept>        // for runtime...
#include <string>           // for string
#include <vector>           // for vector

#include <bits/utility.h>                          // for tuple_e...
#include <nlohmann/detail/iterators/iter_impl.hpp> // for iter_impl
#include <nlohmann/detail/json_pointer.hpp>        // for json_po...
#include <nlohmann/json.hpp>                       // for basic_j...
#include <nlohmann/json_fwd.hpp>                   // for json

#include "ocppi/runtime/config/IncompatibleVersionError.hpp" // for Incompa...
#include "ocppi/runtime/config/KeyMissingError.hpp"          // for KeyMiss...
#include "ocppi/runtime/config/WrongTypeError.hpp"           // for WrongTy...
#include "ocppi/runtime/config/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/semver.hpp"                  // for support...
#include "ocppi/utils/ewrap/EWRAP.hpp"               // for EWRAP
#include "semver.hpp"                                // for operator<

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
namespace
{

void doLoad(std::istream &input, types::Config *cfg, const LoadMode &mode)
try {
        auto j = nlohmann::json::parse(input);

        constexpr auto versionKey = "ociVersion";
        auto versionIt = j.find(versionKey);
        if (versionIt == j.end()) {
                throw KeyMissingError(versionKey);
        }

        if (!versionIt->is_string()) {
                throw WrongTypeError(versionKey);
        }

        auto version = versionIt->get<std::string>();
        if (!mode.test(static_cast<size_t>(
                    LoadOption::IgnoreIncompatibleVersionError))) {
                const auto &[min, max] = support_spec_semver_range();
                auto semver = semver::from_string(version);
                if (semver < min || max < semver) {
                        throw IncompatibleVersionError(version);
                }
        }

        j.get_to(*cfg);
        return;
} catch (const std::exception &) {
        EWRAP(std::runtime_error("load"));
}
}

[[nodiscard]]
auto load(std::istream &input, types::Config *cfg,
          const LoadMode &mode) noexcept -> std::optional<std::exception>
try {
        doLoad(input, cfg, mode);
        return std::nullopt;
} catch (const std::exception &e) {
        return e;
}

}
