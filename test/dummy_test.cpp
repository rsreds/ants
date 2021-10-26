//
// Created by Jacopo Gasparetto on 26/10/21.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test/doctest.h"

int sum(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

TEST_CASE("Test sum") {
    CHECK_EQ(sum(1, 2), 3);
}

TEST_CASE("Test sub") {
    CHECK_EQ(sub(7, 10), -3);
}
