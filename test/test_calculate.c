#include <ctest.h>
#include <math.h>

#include "libgeometry/parser.h"

CTEST(perimeter_suite, circle)
{
    double radius = 5.0;
    double expected_perimeter = 2 * M_PI * radius;
    double real_perimeter;
    calculate_perimeter(radius, &real_perimeter);
    ASSERT_DBL_NEAR(expected_perimeter, real_perimeter);
}

CTEST(area_suite, circle)
{
    double radius = 5.0;
    double expected_area = M_PI * radius * radius;
    double real_area;
    calculate_area(radius, &real_area);
    ASSERT_DBL_NEAR(expected_area, real_area);
}

CTEST(parser_suite, check_circle_intersecting)
{
    char input[] = "circle(0 0, 2) circle(0 3, 2)";
    int expected = 0;
    int result = check_circle(input);
    ASSERT_EQUAL(expected, result);
}

CTEST(parser_suite, check_circle_not_intersecting)
{
    char input[] = "circle(0 0, 1) circle(0 3, 1)";
    int expected = 0;
    int result = check_circle(input);
    ASSERT_EQUAL(expected, result);
}

CTEST(parser_suite, check_circle_containing)
{
    char input[] = "circle(0 0, 2) circle(0 1, 0.5)";
    int expected = 0;
    int result = check_circle(input);
    ASSERT_EQUAL(expected, result);
}