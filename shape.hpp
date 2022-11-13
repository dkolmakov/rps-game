#pragma once

#include <algorithm>
#include "tlist.hpp"

template<std::size_t N>
struct StringLiteral {
    char value[N];
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
};

template<typename... Ts>
struct Beats {
    using beats = TList<Ts...>;
};

template<StringLiteral sl>
struct Label {
    constexpr static std::string_view get_label() { return std::string_view(sl.value); }
};

template<char sym>
struct Symbol {
    static constexpr char symbol = sym;
};

template<typename F>
concept Shape = requires(F)
{
    ([]<typename... Ts> (TList<Ts...>) {})(typename F::beats {});
    ([](std::string_view) {})( F::get_label() );
    ([](const char&) {})( F::symbol );
};

