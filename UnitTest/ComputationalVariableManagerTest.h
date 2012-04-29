/*
 * Name  : ComputationalVariableManagerTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/29/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>


class ComputationalVariableManagerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ComputationalVariableManagerTest);
    CPPUNIT_TEST(basicUsageTest);
    CPPUNIT_TEST(createBeforeRegisterTest);
    CPPUNIT_TEST(registerAfterCreateTest);
    CPPUNIT_TEST(cellHasComputationalVariableTest);
    CPPUNIT_TEST(iteratorTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void basicUsageTest();
    void createBeforeRegisterTest();
    void registerAfterCreateTest();
    void cellHasComputationalVariableTest();
    void iteratorTest();
};
