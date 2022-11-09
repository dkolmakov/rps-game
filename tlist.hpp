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

template<typename TypeTransformer, typename... TTs>
using apply_to_all = TList< typename TypeTransformer::template make<TTs> ... >;

template<TypeList TL, typename TypeTransformer>
using transform = decltype( [] <typename... Ts> (TList<Ts...>) {
    return apply_to_all<TypeTransformer, Ts...>{};
}(TL{}));

struct test_no_transform {
    template<typename T>
    using make = T;
};

static_assert(std::is_same_v<
                      transform<TList<bool, int>, test_no_transform>,
                      TList<bool, int>
>);

