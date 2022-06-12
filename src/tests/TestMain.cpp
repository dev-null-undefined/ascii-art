#ifdef __TESTING__

#include "Test.h"
#include "container/TestVector.h"
#include "container/TestMatrix.h"
#include "TestFileManager.h"

int main() {
    Test::addTest<TestVector>();
    Test::addTest<TestMatrix>();
    Test::addTest<TestFileManager>();
    return Test::runAllTests();
}

#endif