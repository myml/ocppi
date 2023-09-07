#pragma once

#include <list>

#include "fmt/format.h"

template <typename Element>
class fmt::formatter<std::list<Element>> {
    public:
        constexpr auto parse(format_parse_context &ctx)
        {
                return ctx.begin();
        }
        template <typename Context>
        constexpr auto format(std::list<Element> const &list,
                              Context &ctx) const
        {
                format_to(ctx.out(), "[ ");
                for (const auto &e : list) {
                        format_to(ctx.out(), "{} ", e);
                }
                return format_to(ctx.out(), "]");
        }
};
