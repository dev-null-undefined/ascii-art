#include <iostream>
#include "Test.h"
#include <cstdio>
#include <execinfo.h>

Test::Test(std::string test_name) : m_test_name(std::move(test_name)) {
}

int Test::runAllTests() {
    int failed = 0;
    for (const auto & item : TESTS) {
        int status = 0;
        try {
            status = item->runTest();
        } catch (std::exception & ex) {
            status = 1;
            std::cerr << "Exception: " << ex.what() << std::endl;
        }
        if (status) {
            std::cerr << "Test (" << item->m_test_name << ") FAILED!" << std::endl;
            failed = status;
        } else {
            std::cout << "Test (" << item->m_test_name << ") SUCCESSFULLY" << std::endl;
        }
    }
    return failed;
}

void Test::testAssert(bool value) {
    if (!value) throw std::invalid_argument("Assert failed");
}

std::vector<std::shared_ptr<Test>> Test::TESTS = {};

std::string printVector(const Vector & value) {
    return "{ x:" + std::to_string(value.m_x) + ", y:" + std::to_string(value.m_y) + "}";
}

std::string printString(const std::string & value) {
    return value;
}

void print_trace() {
    char ** strings;
    size_t i, size;
    void * array[1024];
    size = backtrace(array, 1024);
    strings = backtrace_symbols(array, size);
    for (i = 0; i < size; i++)
        std::cerr << strings[i] << std::endl;
    free(strings);
}

void Test::assertEqualVector(const Vector & r, const Vector & l) {
    Test::assertEqual<Vector, printVector>(r, l);
}

void Test::assertEqualString(const std::string & r, const std::string & l) {
    Test::assertEqual<std::string, printString>(r, l);
}
