#ifndef ASCII_ART_TEST_H_B06231F4E01749C3B93A63F08DA9A239
#define ASCII_ART_TEST_H_B06231F4E01749C3B93A63F08DA9A239

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../container/Vector.h"

template<typename R>
std::string print(const R & value) {
    return std::to_string(value);
}

std::string printString(const std::string & value);

std::string printVector(const Vector & value);


class Test {
  public:
    Test(std::string test_name);

    virtual ~Test() = default;

    virtual int runTest() = 0;

    static void testAssert(bool value);

    template<typename R, std::string(* toString)(const R &) = print<R>>
    static void assertEqual(const R &, const R &);

    static void assertEqualVector(const Vector & r, const Vector & l);

    static void assertEqualString(const std::string & r, const std::string & l);

    static int runAllTests();

    template<typename T, typename ...Args>
    static void addTest(Args && ...args);

    static std::vector<std::shared_ptr<Test>> TESTS;
  protected:
    std::string m_test_name;

};

template<typename T, typename ...Args>
void Test::addTest(Args && ... args) {
    TESTS.push_back(std::make_shared<T>(std::forward<Args>(args)...));
}

void print_trace();

template<typename R, std::string(* toString)(const R &)>
void Test::assertEqual(const R & r, const R & l) {
    if (r == l) {
        return;
    }
    print_trace();
    throw std::invalid_argument("Not equal: " + toString(r) + " != " + toString(l));
}


#endif //ASCII_ART_TEST_H_B06231F4E01749C3B93A63F08DA9A239
