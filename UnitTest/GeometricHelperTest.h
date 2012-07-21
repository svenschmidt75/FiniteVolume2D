/*
 * Name  : GeometricHelperTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 07/21/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class GeometricHelperTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GeometricHelperTest);
    CPPUNIT_TEST(LineSegmentIntersectionTest);
    CPPUNIT_TEST(LineSegmentRayIntersectionTest);
    CPPUNIT_TEST(LineSegmentLineIntersectionTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void LineSegmentIntersectionTest();
    void LineSegmentRayIntersectionTest();
    void LineSegmentLineIntersectionTest();
};
