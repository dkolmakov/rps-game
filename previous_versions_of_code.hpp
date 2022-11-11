
//int get_winner(char user_choice, char comp_choice) {
//    int acc = 0;
//    Shapes::for_each(do_if(figure_symbol_equals(user_choice),
//                            [&]<Shape LF> (LF f) {
//                                Shapes::for_each(do_if(figure_symbol_equals(comp_choice),
//                                                        [&]<Shape RF> (RF) { acc += decide_winner(LF{}, RF{}); }
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
//                    [r]<Shape LF> (LF f) {
//                            return do_for_each_and_sum(
//                            do_if(
//                                figure_symbol_equals(r),
//                                []<Shape RF> (RF) { return decide_winner(LF{}, RF{}); }
//                                ),
//                                TL{});
//                        }
//                ),
//                TL{});
//}

//template<TypeList TL>
//int get_winner(char l, char r) {
//    return [l, r] <typename... Ts> (TList<Ts...>) {
//        auto internal = [l, r] <Shape LF, typename... TTs> (LF lf, TList<TTs...>) {
//            if (l == LF::symbol) {
//                auto internal = [r]<Shape RF>(const LF lf, const RF rf) {
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


//template<typename Other>
//struct test_transform_to_other {
//    template<typename T>
//    using make = Other;
//};
//
//static_assert(std::is_same_v<
//        transform<TList<bool, int>, test_transform_to_other<double>>,
//        TList<double, double>
//>);
//
//template<typename Other>
//struct test_transform_to_pair {
//    template<typename T>
//    using make = std::pair<Other, T>;
//};
//
//static_assert(std::is_same_v<
//        transform<TList<bool, int>, test_transform_to_pair<double>>,
//        TList<std::pair<double, bool>, std::pair<double, int>>
//>);
//
//template<typename TL>
//struct test_inner_transform {
//    template<typename T>
//    using make = transform<TL, T>;
//};
//
//static_assert(std::is_same_v<
//        transform<TList<bool, int>, test_inner_transform<TList<bool, int>>>,
//        TList<std::pair<double, bool>, std::pair<double, int>>
//>);
//
//template<typename First, typename Second>
//using make_pair = TList<First, Second>;
//
//template<typename First>
//struct pair_with {
//    template<typename Second>
//    using make = make_pair<First, Second>;
//};
//
//template<TypeList TL>
//struct pair_with_each_in_list {
//    template<typename T>
//    using make = transform<TL, pair_with<T>>;
//};
//
//template<TypeList TL>
//using pair_combinations = transform<TL, pair_with_each_in_list<TL>>;

//
//static_assert(std::is_same_v<
//    TList<TList<bool, bool>, TList<bool, int>, TList<int, bool>, TList<int, int>>,
////        pair_combinations<TList<bool, int>>
//        transform<TList<bool, int>, pair_with_each_in_list<TList<bool, int>>>
//>);