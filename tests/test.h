
#ifndef UT
#define UT

#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <iostream>


namespace ut {

struct TestCase 
{
    std::string suite;
    std::string name;
    std::function<void()> fn;

};

inline std::vector<TestCase> &registry()
{
    static std::vector<TestCase> r;
    return r;
}

inline bool match_filter(const std::string &suite, const std::string &name, const std::string &f) {
    if (f.empty() || f=="*") return true;
    auto full = suite + "." + name;
    if (f == full || f == suite + ".*") return true;
    return full.find(f) != std::string::npos;
}

inline int run_all(const std::string &filter, bool list_only) {
    int ran = 0;
    for (auto &t : registry()) {
        std::cout << t.name << std::endl;
        std::cout << t.suite << std::endl;
        if (!match_filter(t.suite, t.name, filter)) continue;
        if (list_only) { std::cout << t.suite << "." << t.name << "\n"; continue; }
        std::cout << "[ RUN ] " << t.suite << "." << t.name << "\n";
        t.fn();
        std::cout << "[ OK  ] " << t.suite << "." << t.name << "\n";
        ran++;
    }
    if (!list_only) std::cout << "[ DONE ] ran=" << ran << "\n";
    return 0; 
}


struct Registrar 
{
    Registrar(const char *s, const char *n, std::function<void()> fn) {
        registry().push_back({s, n, std::move(fn)});
    }
};

} // namespace ut

#define TEST(SUITE, NAME) \
    static void testfn_##SUITE##_##NAME(); \
    static ::ut::Registrar registrar_##SUITE##_##NAME(#SUITE, #NAME, testfn_##SUITE##_##NAME); \
    static void testfn_##SUITE##_##NAME() \



#endif // UT


