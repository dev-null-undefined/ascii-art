#ifdef __TESTING__

#include "Test.h"
#include "container/TestVector.h"
#include "container/TestMatrix.h"
#include "TestFileManager.h"
#include "ascii-art/sources/image-formats/TestPNGImage.h"
#include "ascii-art/sources/image-formats/TestJPEGImage.h"

int main() {
    Test::addTest<TestVector>();
    Test::addTest<TestMatrix>();
    Test::addTest<TestFileManager>();
    Test::addTest<TestPNGImage>();
    Test::addTest<TestJPEGImage>();
    return Test::runAllTests();
}

#endif