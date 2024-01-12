#pragma once

#include <list>
#include <string>

namespace ocppi::runtime
{

class Option {
    private:
        std::list<std::string> arg;

    public:
        // 实现 args() 函数
        [[nodiscard]]
        auto args() const noexcept -> std::list<std::string>
        {
                return arg;
        }
        void add(std::string argName, std::string argValue = "")
        {
                arg.push_back(argName);
                if (!argValue.empty()) {
                        arg.push_back(argValue);
                }
        }
};

}
