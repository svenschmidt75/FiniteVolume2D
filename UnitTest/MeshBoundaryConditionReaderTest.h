/*
 * Name  : MeshBoundaryConditionReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 04/07/2012
 */
#pragma once

#include "MeshBuilderMock.h"

#include <cppunit/extensions/HelperMacros.h>


class MeshBoundaryConditionReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(MeshBoundaryConditionReaderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(testNumberOfDirichletBoundaryConditions);
    CPPUNIT_TEST(testNumberOfNeumannBoundaryConditions);
    CPPUNIT_TEST(testDirichlet);
    CPPUNIT_TEST(testNeumann);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void testNumberOfDirichletBoundaryConditions();
    void testNumberOfNeumannBoundaryConditions();
    void testDirichlet();
    void testNeumann();

private:
    class MockMeshBuilder : public IMeshBuilder {
    public:
        struct BCStr {
            BCStr() {}
            BCStr(BoundaryConditionCollection::Type bc_type, double bc_value) : bc_type_(bc_type), bc_value_(bc_value) {}

            BoundaryConditionCollection::Type bc_type_;
            double                  bc_value_;
        };

    public:
        bool
        buildNode(IGeometricEntity::Id_t node_id, bool on_boundary, double x, double y) {
            return true;
        }

        bool
        buildFace(IGeometricEntity::Id_t face_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & node_ids) {
            return true;
        }

        bool
        buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
            return true;
        }

        bool
        buildBoundaryCondition(IGeometricEntity::Id_t face_id, BoundaryConditionCollection::Type bc_type, double bc_value) {
            bcs_[face_id] = BCStr(bc_type, bc_value);
            return true;
        }

        boost::optional<Mesh::Ptr>
        getMesh() const {
            return boost::optional<Mesh::Ptr>();
        }

    public:
        typedef std::map<IGeometricEntity::Id_t, BCStr> BCMap_t;
        BCMap_t bcs_;
    };

private:
    void initMesh();

private:
    typedef Mesh::Ptr MeshPtr;

private:
    std::string            mesh_filename_;
    static MockMeshBuilder mock_builder_;
};
