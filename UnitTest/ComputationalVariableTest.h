/*
 * Name  : ComputationalVariableTest
 * Path  : 
 * Use   : Computational variable used in the computational
 *         molecules.
 * Author: Sven Schmidt
 * Date  : 04/28/2012
 */
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "FiniteVolume2DLib/Mesh.h"
#include "MeshBuilderMock.h"


class ComputationalVariableTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ComputationalVariableTest);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void test();

private:
    void initMesh();

private:
    typedef Mesh::Ptr MeshPtr;

private:
    std::string            mesh_filename_;
    static MeshPtr         mesh_;

    /* If we were to use MeshBuilder directly, we would need a default constructor
     * for MeshBuilder, because we need to declare it a static data member.
     * There is no easy way to define a default constructor because of the
     * EntityManager. Hence, this embedded mesh builder.
     */
    static MeshBuilderMock mesh_builder_;
};
