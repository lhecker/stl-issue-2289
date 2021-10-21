#include <string_view>

#include <benchmark/benchmark.h>

static const char* volatile narrow1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char* volatile narrow2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
static const wchar_t* volatile wide1 = L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const wchar_t* volatile wide2 = L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";

static volatile bool val = true;

__declspec(noinline) bool equal_std_string_view(const std::string_view& a, const std::string_view& b) {
    return a == b;
}

__declspec(noinline) bool equal_std_wstring_view(const std::wstring_view& a, const std::wstring_view& b) {
    return a == b;
}

__declspec(noinline) bool equal_builtin_wmemcmp(const std::wstring_view& a, const std::wstring_view& b) {
    return a.size() == b.size() && __builtin_wmemcmp(a.data(), b.data(), a.size()) == 0;
}

__declspec(noinline) bool equal_wmemcmp(const std::wstring_view& a, const std::wstring_view& b) {
    return a.size() == b.size() && wmemcmp(a.data(), b.data(), a.size()) == 0;
}

__declspec(noinline) bool equal_builtin_memcmp(const std::wstring_view& a, const std::wstring_view& b) {
    return a.size() == b.size() && __builtin_memcmp(a.data(), b.data(), a.size() * sizeof(wchar_t)) == 0;
}

#define DEFINE_BENCHMARK(name, fn, var, minSize, maxSize) \
    static void BM_##fn(benchmark::State& state) {        \
        const size_t size = state.range(0);               \
        for (auto _ : state) {                            \
            if (val) {                                    \
                val = fn({var##1, size}, {var##2, size}); \
                benchmark::ClobberMemory();               \
            }                                             \
        }                                                 \
    }                                                     \
    BENCHMARK(BM_##fn)->Name(name)->RangeMultiplier(2)->Range(minSize, maxSize);

DEFINE_BENCHMARK("std::string_view::operator==", equal_std_string_view, narrow, 4, 128);
DEFINE_BENCHMARK("std::wstring_view::operator==", equal_std_wstring_view, wide, 2, 64);
DEFINE_BENCHMARK("__builtin_wmemcmp", equal_builtin_wmemcmp, wide, 2, 64);
DEFINE_BENCHMARK("wmemcmp", equal_wmemcmp, wide, 2, 64);
DEFINE_BENCHMARK("__builtin_memcmp", equal_builtin_memcmp, wide, 2, 64);

BENCHMARK_MAIN();
