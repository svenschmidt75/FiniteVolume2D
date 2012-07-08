/*
 * Name  : ASCIIMeshReaderTest
 * Path  : 
 * Use   : 
 * Author: Sven Schmidt
 * Date  : 03/03/2012
 */
#pragma once

#include "FiniteVolume2DLib/IMeshBuilder.h"

#include <cppunit/extensions/HelperMacros.h>


class ASCIIMeshReaderTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ASCIIMeshReaderTest);
    CPPUNIT_TEST(testMeshFileExists);
    CPPUNIT_TEST(testNumberOfVertices);
    CPPUNIT_TEST(testVertex1);
    CPPUNIT_TEST(testVertex2);
    CPPUNIT_TEST(testNumberOfFaces);
    CPPUNIT_TEST(testFace);
    CPPUNIT_TEST(testNumberOfCells);
    CPPUNIT_TEST(testCell);
    CPPUNIT_TEST_SUITE_END();

private:
    class MockMeshBuilder : public IMeshBuilder {
    public:
        struct NodeStr {
            NodeStr() {}
            NodeStr(IGeometricEntity::Entity_t entity_type, double x, double y) : entity_type_(entity_type), x_(x), y_(y) {}

            IGeometricEntity::Entity_t entity_type_;
            double                     x_;
            double                     y_;
        };

        struct FaceStr {
            FaceStr() {}
            FaceStr(IGeometricEntity::Entity_t entity_type, std::vector<IGeometricEntity::Id_t> const & node_ids) : entity_type_(entity_type), node_ids_(node_ids) {}

            IGeometricEntity::Entity_t          entity_type_;
            std::vector<IGeometricEntity::Id_t> node_ids_;
        };

    public:
        bool
        buildNode(IGeometricEntity::Id_t node_id, IGeometricEntity::Entity_t entity_type, double x, double y) {
            NodeMap_t::const_iterator it = nodes_.find(node_id);
            if (it != nodes_.end())
                CPPUNIT_FAIL("Duplicate node!");
            nodes_[node_id] = NodeStr(entity_type, x, y);
            return true;
        }

        bool
        buildFace(IGeometricEntity::Id_t face_id, IGeometricEntity::Entity_t entity_type, std::vector<IGeometricEntity::Id_t> const & node_ids) {
            FaceMap_t::const_iterator it = faces_.find(face_id);
            if (it != faces_.end())
                CPPUNIT_FAIL("Duplicate face!");
            faces_[face_id] = FaceStr(entity_type, node_ids);
            return true;
        }

        bool
        buildCell(IGeometricEntity::Id_t cell_id, std::vector<IGeometricEntity::Id_t> const & face_ids) {
            CellMap_t::const_iterator it = cells_.find(cell_id);
            if (it != cells_.end())
                CPPUNIT_FAIL("Duplicate cell!");
            cells_[cell_id] = face_ids;
            return true;
        }

        void
        outputReport(std::ostream & target) const {}

        boost::optional<Mesh::Ptr>
        getMesh() const {
            return boost::optional<Mesh::Ptr>();
        }

    public:
        typedef std::map<IGeometricEntity::Id_t, NodeStr> NodeMap_t;
        NodeMap_t nodes_;

        typedef std::map<IGeometricEntity::Id_t, FaceStr> FaceMap_t;
        FaceMap_t faces_;

        typedef std::map<IGeometricEntity::Id_t, std::vector<IGeometricEntity::Id_t>> CellMap_t;
        CellMap_t cells_;
    };

public:
    void setUp();
    void tearDown();

protected:
    void testMeshFileExists();
    void testNumberOfVertices();
    void testVertex1();
    void testVertex2();
    void testNumberOfFaces();
    void testFace();
    void testNumberOfCells();
    void testCell();

private:
    void initMesh();

private:
    std::string            mesh_filename_;
    static MockMeshBuilder mock_builder_;
};
