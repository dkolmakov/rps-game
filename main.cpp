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

template<Figure LF, Figure RF>
bool operator>(LF, RF) { return LF::beats::template contains_v<RF>; }

template<Figure LF, Figure RF>
bool operator==(LF, RF) { return std::is_same_v<RF, LF>; }

template<Figure LF, Figure RF>
int decide_winner(LF lf, RF rf) {
    if (lf == rf) { return 0; }
    else if (lf > rf) { return 1; }
    else { return -1; }
}

using Figures = TList<Paper, Scissors, Rock>;

auto figure_symbol_equals(char sym) {
    return [sym]<Figure LF> (LF) { return LF::symbol == sym; };
}

template<typename Action>
auto apply_to_figures(Action&& action) {
    return [&](char user_choice, char comp_choice) {
        Figures::for_each(do_if(figure_symbol_equals(user_choice),
                                [&]<Figure LF>(LF) {
                                    Figures::for_each(do_if(figure_symbol_equals(comp_choice),
                                                            [&]<Figure RF>(RF) { action(LF{}, RF{}); }
                                    ));
                                }
        ));
    };
}

int get_winner(char user_choice, char comp_choice) {
    int acc = 0;
    auto pair_to_result = apply_to_figures([&acc]<Figure LF, Figure RF> (LF lf, RF rf) {
        std::cout << lf.label << " " << decide_winner(LF{}, RF{}) << " " << rf.label << std::endl;
        acc += decide_winner(LF{}, RF{});
    });
    pair_to_result(user_choice, comp_choice);
    return acc;
}

int main() {
    std::cout << get_winner('r', 's') << std::endl;
    std::cout << get_winner('s', 'r') << std::endl;
    std::cout << get_winner('s', 's') << std::endl;
}
