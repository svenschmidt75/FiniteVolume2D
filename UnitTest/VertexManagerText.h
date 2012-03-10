/*
 * Name  : VertexManagerText
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class VertexManagerText : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(VertexManagerText);
    CPPUNIT_TEST(testTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testTest();
};
