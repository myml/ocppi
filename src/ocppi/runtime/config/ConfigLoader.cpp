#include "ocppi/runtime/config/ConfigLoader.hpp"

#include <map>
#include <string>

#include <nlohmann/detail/iterators/iter_impl.hpp>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "ocppi/runtime/config/IncompatibleVersionError.hpp"
#include "ocppi/runtime/config/KeyMissingError.hpp"
#include "ocppi/runtime/config/WrongTypeError.hpp"
#include "ocppi/runtime/config/types/Config.hpp"
#include "ocppi/runtime/config/types/Generators.hpp" // IWYU pragma: keep
#include "ocppi/runtime/semver_range.hpp"
#include "semver.hpp"

namespace ocppi::runtime::config
{

namespace
{

[[nodiscard]]
auto doLoad(std::istream &input, const ConfigLoader &) -> types::Config
{
        auto j = nlohmann::json::parse(input);

        constexpr auto ociVersionKey = "ociVersion";
        auto itVersion = j.find(ociVersionKey);
        if (itVersion == j.end()) {
                throw KeyMissingError(ociVersionKey);
        }

        if (!itVersion->is_string()) {
                throw WrongTypeError(ociVersionKey);
        }

        auto version = itVersion->get<std::string>();

        const auto &[min, max] = support_spec_semver_range();
        auto semver = semver::from_string(version);
        if (semver < min || max < semver) {
                throw IncompatibleVersionError(version);
        }

        return j.get<types::Config>();
}

}

[[nodiscard]]
auto ConfigLoader::load(std::istream &input) const noexcept
        -> tl::expected<types::Config, std::exception_ptr>
try {
        return doLoad(input, *this);
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
