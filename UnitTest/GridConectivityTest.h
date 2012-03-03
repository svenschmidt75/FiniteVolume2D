/*
 * Name  : GridConectivityTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class GridConectivityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(GridConectivityTest);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testInit();
};
