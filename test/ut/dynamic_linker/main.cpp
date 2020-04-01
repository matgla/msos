#include <gtest/gtest.h>

const char* parent_path;

int main(int argc, char **argv)
{
    parent_path = argv[0];
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
