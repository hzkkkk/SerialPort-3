#include "stdafx.h"
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    getchar();
    return r;
}
