#include <iostream>
#include <utility>

#include "tlist.hpp"
#include "shape.hpp"

struct Paper;
struct Scissors;
struct Rock;

struct Rock : Beats<Scissors>, Label<"Rock">, Symbol<'r'> {};
struct Paper : Beats<Rock>, Label<"Paper">, Symbol<'p'> {};
struct Scissors  : Beats<Paper>, Label<"Scissors">, Symbol<'s'> {};

template<Shape LF, Shape RF>
constexpr bool operator>(LF, RF) { return LF::beats::template contains_v<RF>; }

template<Shape LF, Shape RF>
constexpr bool operator==(LF, RF) { return std::is_same_v<RF, LF>; }

using Shapes = TList<Paper, Scissors, Rock>;

auto figure_symbol_equals(char sym) {
    return [sym]<Shape LF> (LF) { return LF::symbol == sym; };
}

template<typename Action>
auto apply_to_shape_pair(Action&& action) {
    return [&](char user_choice, char comp_choice) {
        Shapes::for_each( do_if( figure_symbol_equals(user_choice),
                                 [&]<Shape UserShape>(UserShape) {
                                      Shapes::for_each( do_if( figure_symbol_equals(comp_choice),
                                            [&]<Shape CompShape>(CompShape) { action(UserShape{}, CompShape{}); }
                                      ));
                                 }
        ));
    };
}

template<Shape LF, Shape RF>
constexpr auto decide_winner(LF lf, RF rf) {
    if (lf == rf) { return std::tuple{ 0, "same as", "Tie!" }; }
    else if (lf > rf) { return std::tuple{ 1, "beats", "User wins!" }; }
    else { return std::tuple{ -1, "is beaten by", "Comp wins!" }; }
}

int main() {
    struct GameHistory {
        size_t gameTotal = 0;
        size_t userWins = 0;
        size_t compWins = 0;
    } history;

    auto game_action = [&history]<Shape UserShape, Shape CompShape> (UserShape user, CompShape comp) {
        ++history.gameTotal;
        auto [val, sep, result] = decide_winner(user, comp);

        if (val == 1) ++history.userWins;
        else if (val == -1) ++history.compWins;

        std::cout << user.label << " " << sep << " " << comp.label << ". " << result << std::endl;

    };

    auto game_engine = apply_to_shape_pair(game_action);

    game_engine('r', 's');
    game_engine('s', 'r');
    game_engine('s', 's');
}
