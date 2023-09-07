#include "ocppi/runtime/config/ConfigLoader.hpp"

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
#include "semver.hpp"                                // for operator<

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
