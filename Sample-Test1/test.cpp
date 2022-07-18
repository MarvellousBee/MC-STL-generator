#include "pch.h"

#include "../STL generator/XValues.h"
#include "../STL generator/stl generation.h"
TEST(TestXValues, Values4) {
    Values4 values4{ 1,2,3,4 };
    EXPECT_EQ(values4[0], 1);
    EXPECT_EQ(values4.at(0), 1);
    EXPECT_TRUE(values4 == Values4(1, 2, 3, 4));
}

TEST(TestXValues, Point3f) {
    Point3f p1{ 1,2,3 };
    Point3f p2{ 4,5,6 };

    ASSERT_EQ(p1.x, 1);
    ASSERT_EQ(p1.y, 2);
    ASSERT_EQ(p1.z, 3);

    EXPECT_EQ(p1.sum(), 6);

    p1 *= p2;
    EXPECT_EQ(p1.x, 4);
    EXPECT_EQ(p1.y, 10);
    EXPECT_EQ(p1.z, 18);

    EXPECT_FALSE(p1 == p2);
}

TEST(ObjectGeneration, make_facet) {
    auto facet
    { 
        make_facet
        (
            {
              {1.000000e+00, 1.028657e-15, 2.465190e-31 },
              {1.110223e-15, 1.000000e+00, -7.053318e-16},
              {1.000000e+00, 1.000000e+00, -2.057314e-16}
            },
            {
               4.996004e-16, -2.057314e-16, -1.000000e+00
            }
        ) 
    };
    std::string expected_str
    {
         "  facet normal 0.000000 -0.000000 1.000000\n"
         "  outer loop\n"
         "    vertex   3.250000 0.000000 0.000000\n"
         "    vertex   0.000000 3.250000 -0.000000\n"
         "    vertex   3.250000 3.250000 -0.000000\n"
         "  endloop\n"
         "endfacet\n"
    };
    EXPECT_EQ(facet, expected_str);
}

    