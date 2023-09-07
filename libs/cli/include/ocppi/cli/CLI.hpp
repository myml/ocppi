#pragma once

#include <experimental/propagate_const>

#include "ocppi/runtime/Runtime.hpp"

namespace ocppi::cli
{

class CLI : public runtime::Runtime {
    protected:
        struct impl;
        friend struct impl;
        std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;

    public:
        CLI(const std::filesystem::path &) noexcept;
        virtual ~CLI();
};

}
