#include <cstdint>
#include <iostream>
#include <utility>

template<typename... Ts>
struct TList {};

template<typename TL>
concept TypeList = requires(TL t) 
{
    ([]<typename... Ts> (TList<Ts...>) {})(t);
};

template<typename T, TypeList TL>
bool is_in_list_v = []<typename... Ts>(TList<Ts...>){
    return (std::is_same_v<Ts, T> || ...);
}(TL{});

struct Paper;
struct Scissors;
struct Rock;

template<typename... Ts>
struct Beats {
    using beats = TList<Ts...>;
};

template<typename F>
concept Figure = requires(F)
{
    ([]<typename... Ts> (TList<Ts...>) {})(typename F::beats {});
};

using Figures = TList<Paper, Scissors, Rock>;

struct Rock : Beats<Scissors> {};
struct Paper : Beats<Rock> {};
struct Scissors  : Beats<Paper> {};

template<Figure LF, Figure RF>
bool operator>(LF, RF) 
{
    return is_in_list_v<RF, typename LF::beats>;
}

int main() {
    using Figures = TList<>;

    std::cout << (Rock{} > Paper{}) << std::endl;
    std::cout << (Rock{} > Scissors{}) << std::endl;
    std::cout << (Scissors{} > Paper{}) << std::endl;
}
