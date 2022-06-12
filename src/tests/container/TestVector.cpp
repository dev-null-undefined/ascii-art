#include "TestVector.h"

int TestVector::runTest() {
    Vector v{1, 1};
    Vector v2{1, 1};
    assertEqualVector(v, v2);
    assertEqualVector((v2 += Vector{1, 0}), Vector{2, 1});
    testAssert(v != v2);
    assertEqualVector((v *= 4.0), Vector{4, 4});
    size_t number = 4;
    assertEqualVector((v *= number), Vector{16, 16});
    assertEqualVector(v, Vector{16, 16});
    v = {1, 1};
    assertEqualVector((v * 4.0), Vector{4, 4});
    assertEqualVector(v, Vector{1, 1});
    assertEqualVector((v * number), Vector{4, 4});
    assertEqualVector(v, Vector{1, 1});
    v -= {0, 1};
    assertEqualVector(v, Vector{1, 0});
    v += {1, 1};
    assertEqualVector(v, Vector{2, 1});
    v = {1, 1};
    assertEqualVector(v, Vector{1, 1});
    assertEqualVector((v + Vector{1, 1}), Vector{2, 2});
    assertEqualVector(v, Vector{1, 1});
    assertEqualVector((v - Vector{1, 1}), Vector{0, 0});
    assertEqualVector(v, Vector{1, 1});

    Vector max = Vector{10, 10};
    Vector orig = Vector{13, 5};
    Vector result = Vector::resizeWithAspectRation(max, orig, 1.0);
    testAssert(result.m_x <= max.m_x);
    testAssert(result.m_y <= max.m_y);


    max = Vector{500, 500};
    orig = Vector{200, 200};
    result = Vector::resizeWithAspectRation(max, orig, 1.0);
    assertEqualVector(result, max);

    max = Vector{30, 40};
    orig = Vector{50, 60};
    result = Vector::resizeWithAspectRation(max, orig, 1.0);
    testAssert(result.m_x <= max.m_x);
    testAssert(result.m_y <= max.m_y);
    return 0;
}
