#include "pch.h"

#include "../STL generator/XValues.h"
#include "../STL generator/StlGeneration.h"
TEST(TestXValues, Values4) {
    Values4 values4{ 1,2,3,4 };
    const Values4& cref{ values4 };

    // non-const overload
    EXPECT_EQ(values4[0], 1);
    // const overload
    EXPECT_EQ(cref[0], 1);

    EXPECT_TRUE (values4 == Values4(1, 2, 3, 4));

    EXPECT_FALSE(values4 == Values4(0, 2, 3, 4));
    EXPECT_FALSE(values4 == Values4(1, 0, 3, 4));
    EXPECT_FALSE(values4 == Values4(1, 2, 0, 4));
    EXPECT_FALSE(values4 == Values4(1, 2, 3, 0));

    
}

TEST(TestXValues, Point3) {
    Point3 p1{ 1,2,3 };
    Point3 p2{ 4,5,6 };

    ASSERT_EQ(p1.x, 1);
    ASSERT_EQ(p1.y, 2);
    ASSERT_EQ(p1.z, 3);

    Point3 sub{ p2 - p1 };
    EXPECT_EQ(sub.x, 3);
    EXPECT_EQ(sub.y, 3);
    EXPECT_EQ(sub.z, 3);

    Point3 mul{ p2 * p1 };
    EXPECT_EQ(mul.x, 4);
    EXPECT_EQ(mul.y, 10);
    EXPECT_EQ(mul.z, 18);

    auto p1_copy{ p1 };
    p1_copy *= p2;
    EXPECT_EQ(p1_copy.x, 4);
    EXPECT_EQ(p1_copy.y, 10);
    EXPECT_EQ(p1_copy.z, 18);

    Point3 div_l{ 3, 6, 9 };
    Point3 div{ div_l / 3 };

    EXPECT_EQ(div.x, 1);
    EXPECT_EQ(div.y, 2);
    EXPECT_EQ(div.z, 3);

    Point3 add{ p2 + p1 };
    EXPECT_EQ(add.x, 5);
    EXPECT_EQ(add.y, 7);
    EXPECT_EQ(add.z, 9);

    EXPECT_EQ(p1.sum(), 6);

    EXPECT_FALSE(p1 == p2);
}

TEST(ObjectGeneration, make_facet_string) {
    auto facet
    {
        make_facet_string
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

