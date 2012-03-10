/*
 * Name  : MeshConnectivityTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/10/2012
 */
#pragma once

#include "FiniteVolume2DLib/IMeshBuilder.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshConnectivityTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshConnectivityTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();

private:
    void initMeshConnectivity();

private:
    std::string mesh_filename_;
    static MockMeshBuilder mock_builder_;
};
