/*
 * Name  : MeshBuilderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class MeshBuilderTest {
    CPPUNIT_TEST_SUITE(MeshBuilderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
};
