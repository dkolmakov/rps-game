
//int get_winner(char user_choice, char comp_choice) {
//    int acc = 0;
//    Figures::for_each(do_if(figure_symbol_equals(user_choice),
//                            [&]<Figure LF> (LF f) {
//                                Figures::for_each(do_if(figure_symbol_equals(comp_choice),
//                                                        [&]<Figure RF> (RF) { acc += decide_winner(LF{}, RF{}); }
//                                ));
//                            }
//    ));
//    return acc;
//}
//
//template<typename Action, typename... Ts>
//int do_for_each_and_sum(Action op, TList<Ts...>) {
//    return (op(Ts{}) + ... );
//}
//template<TypeList TL>
//int get_winner(char l, char r) {
//    return do_for_each_and_sum(
//            do_if(
//                     figure_symbol_equals(l),
//                    [r]<Figure LF> (LF f) {
//                            return do_for_each_and_sum(
//                            do_if(
//                                figure_symbol_equals(r),
//                                []<Figure RF> (RF) { return decide_winner(LF{}, RF{}); }
//                                ),
//                                TL{});
//                        }
//                ),
//                TL{});
//}

//template<TypeList TL>
//int get_winner(char l, char r) {
//    return [l, r] <typename... Ts> (TList<Ts...>) {
//        auto internal = [l, r] <Figure LF, typename... TTs> (LF lf, TList<TTs...>) {
//            if (l == LF::symbol) {
//                auto internal = [r]<Figure RF>(const LF lf, const RF rf) {
//                    if (r == RF::symbol) {
//                        if (lf == rf) { return 0; }
//                        else if (lf > rf) { return 1; }
//                        else { return -1; }
//                    }
//                    return 0;
//                };
//                return (internal(lf, TTs{}) + ... );
//            }
//            return 0;
//        };
//
//        return (internal(Ts{}, TL{}) + ... );
//    } (TL{});
//}
