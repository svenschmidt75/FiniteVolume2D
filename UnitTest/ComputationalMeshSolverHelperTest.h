/*
 * Name  : ComputationalMeshSolverHelperTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 06/23/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class ComputationalMeshSolverHelperTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ComputationalMeshSolverHelperTest);
    CPPUNIT_TEST(matrixConstructionTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void matrixConstructionTest();
};
