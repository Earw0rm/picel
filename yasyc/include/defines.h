#ifndef _DEFINES_H_
#define _DEFINES_H_

#define API [[gnu::visibility("default")]]
#include <stdint.h>

// TODO for macros debug purpose use -E -P args of clang
// -E stop after preprocessing
// -P delete #line dirrectives




//Calculation number of variatic arguments
#if defined(__GNUC__) || defined(__clang__) // supports passing 0 arguments using gcc/clang non-standard features
#define VA_NARGS_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(_, ## __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#else
#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N // 1 or more arguments only
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#endif


#define SIZEOF_1(a) sizeof(a)
#define SIZEOF_2(a, b) sizeof(a), sizeof(b)
#define SIZEOF_3(a, b, c) sizeof(a), sizeof(b), sizeof(c)
#define SIZEOF_4(a, b, c, d) sizeof(a), sizeof(b), sizeof(c), sizeof(d)
#define SIZEOF_5(a, b, c, d, e) sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e)
#define SIZEOF_6(a, b, c, d, e, f) sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f)
#define SIZEOF_7(a, b, c, d, e, f, g) sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f), sizeof(g)
#define SIZEOF_8(a, b, c, d, e, f, g, h) sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f), sizeof(g), sizeof(h)
#define SIZEOF_9(a, b, c, d, e, f, g, h, i) sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f), sizeof(g), sizeof(h), sizeof(i)
#define SIZEOF_10(a, b, c, d, e, f, g, h, i, j) sizeof(a), sizeof(b), sizeof(c), sizeof(d), sizeof(e), sizeof(f), sizeof(g), sizeof(h), sizeof(i), sizeof(j)

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define EXPAND(x) x

#define SIZEOF_N(N, ...) CONCAT(SIZEOF_, N)(__VA_ARGS__)
#define SIZEOF_ALL(...) SIZEOF_N(VA_NARGS(__VA_ARGS__), __VA_ARGS__)





#endif 