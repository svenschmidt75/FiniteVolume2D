/*
 * Name  : ComputationalMoleculeTest
 * Path  : 
 * Use   :
 * Author: Sven Schmidt
 * Date  : 04/22/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class ComputationalMoleculeTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ComputationalMoleculeTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
};
