#include "ocppi/cli/crun/Crun.hpp"

#include <iostream>

#include "ocppi/cli/crun/CrunImpl.hpp"

namespace ocppi::cli::crun
{

Crun::Crun(const std::filesystem::path &bin)
        : CLI(bin)
        , pImpl(new Crun::impl{})
{
}

Crun::~Crun() = default;

auto Crun::state(const runtime::ContainerID &id,
                 const std::vector<runtime::StateOption> &opts) const noexcept
        -> tl::expected<runtime::state::types::State, std::exception_ptr>
try {
        return pImpl->doState(*this, id, opts);
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto Crun::create(const runtime::ContainerID &id,
                  const std::filesystem::path &pathToBundle,
                  const std::vector<runtime::CreateOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        pImpl->doCreate(*this, id, pathToBundle, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto Crun::start(const runtime::ContainerID &id,
                 const std::vector<runtime::StartOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        pImpl->doStart(*this, id, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto Crun::kill(const runtime::ContainerID &id, const runtime::Signal &signal,
                const std::vector<runtime::KillOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        pImpl->doKill(*this, id, signal, opts);
        return {};

} catch (...) {
        return tl::unexpected(std::current_exception());
}

auto Crun::delete_(const runtime::ContainerID &id,
                   const std::vector<runtime::DeleteOption> &opts) noexcept
        -> tl::expected<void, std::exception_ptr>
try {
        pImpl->doDelete(*this, id, opts);
        return {};
} catch (...) {
        return tl::unexpected(std::current_exception());
}

}
