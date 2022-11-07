#pragma once

#include <utility>

template<typename... Ts>
struct TList {
    template<typename Action>
    static void for_each(Action op) {
        (op(Ts{}), ... );
    }

    template<typename T>
    static constexpr bool contains_v = [](){
        return (std::is_same_v<Ts, T> || ...);
    }();
};

template<typename TL>
concept TypeList = requires(TL t)
{
    ([]<typename... Ts> (TList<Ts...>) {})(t);
};

template<typename Predicate, typename Action>
auto do_if(Predicate&& p, Action&& op) {
    return [p, op] (auto&& ... args) {
        if (p(args...)) {
            return op(args...);
        }
    };
}
