#include "TestMatrix.h"
#include "../../container/Matrix.h"

int TestMatrix::runTest() {
    Matrix<int> intm(10, 10);
    assertEqual(intm.at(9, 9), 0);
    try {
        intm.at(9, 10);
        intm.at(10, 9);
        intm.at(10, 10);
        testAssert(false); // should throw exception
    } catch (...) {}

    return 0;
}
