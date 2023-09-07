#pragma once

#include <list>
#include <string>

namespace ocppi::runtime
{

class Option {
    public:
        [[nodiscard]]
        virtual auto args() const noexcept -> std::list<std::string> = 0;
};

}
