// Thish file is generated by /tools/codegen powered by quicktype.
// DO NOT EDIT IT.

// clang-format off

//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Features.hpp data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include <nlohmann/json.hpp>
#include <ocppi/runtime/features/types/helper.hpp>

#include <ocppi/runtime/features/types/Linux.hpp>

namespace ocppi {
namespace runtime {
namespace features {
namespace types {
/**
* Open Container Initiative Runtime Specification Runtime Features Schema
*/

using nlohmann::json;

/**
* Open Container Initiative Runtime Specification Runtime Features Schema
*/
struct Features {
std::optional<std::map<std::string, nlohmann::json>> annotations;
std::optional<std::vector<std::string>> hooks;
std::optional<Linux> linux_;
std::optional<std::vector<std::string>> mountOptions;
std::string ociVersionMax;
std::string ociVersionMin;
};
}
}
}
}

// clang-format on
