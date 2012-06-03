/*
 * Name  : MeshCheckerTest
 * Path  : 
 * Use   : Check mesh test
 * Author: Sven Schmidt
 * Date  : 06/03/2012
 */
#pragma once

#include "internal/MeshBuilderMock.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshCheckerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshCheckerTest);
    CPPUNIT_TEST(testMissingBoundaryCondition);
    CPPUNIT_TEST(testWrongBoundaryCondition);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMissingBoundaryCondition();
    void testWrongBoundaryCondition();

private:
    MeshBuilderMock             mock_builder_;
    BoundaryConditionCollection bc_;
};
