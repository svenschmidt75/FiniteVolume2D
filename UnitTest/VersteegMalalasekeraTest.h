/*
 * Name  : VersteegMalalasekeraTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 06/09/2012
 */
#pragma once

#include "internal/MeshBuilderMock.h"

#include "FiniteVolume2DLib/BoundaryConditionCollection.h"

#include <cppunit/extensions/HelperMacros.h>


class VersteegMalalasekeraTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(VersteegMalalasekeraTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(evaluateFluxesCell1Test);
    CPPUNIT_TEST(evaluateFluxesCell3Test);
    CPPUNIT_TEST(evaluateCell5ComputationalMoleculeTest);
    CPPUNIT_TEST(evaluateCell2ComputationalMoleculeTest);
    CPPUNIT_TEST(compMoleculeCell1AndCell5SimilarTest);
    CPPUNIT_TEST(compMoleculeCell6AndCell8SimilarTest);
    CPPUNIT_TEST(compMoleculeCell7AndCell9SimilarTest);
    CPPUNIT_TEST(variousRelationsTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void evaluateFluxesCell1Test();
    void evaluateFluxesCell3Test();
    void evaluateCell5ComputationalMoleculeTest();
    void evaluateCell2ComputationalMoleculeTest();
    void compMoleculeCell1AndCell5SimilarTest();
    void compMoleculeCell6AndCell8SimilarTest();
    void compMoleculeCell7AndCell9SimilarTest();
    void variousRelationsTest();

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
    static MeshBuilderMock             mesh_builder_;

    static BoundaryConditionCollection bc_;
};
