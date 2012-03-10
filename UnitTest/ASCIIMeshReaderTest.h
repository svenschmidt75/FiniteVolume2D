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
        struct VertexStr {
            VertexStr() {}
            VertexStr(bool on_boundary, double x, double y) : on_boundary_(on_boundary), x_(x), y_(y) {}

            bool   on_boundary_;
            double x_;
            double y_;
        };

        struct FaceStr {
            FaceStr() {}
            FaceStr(bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids) : on_boundary_(on_boundary), vertex_ids_(vertex_ids) {}

            bool                                on_boundary_;
            std::vector<IGeometricEntity::Id_t> vertex_ids_;
        };

    public:
        bool
        buildVertex(IGeometricEntity::Id_t vertex_id, bool on_boundary, double x, double y) {
            VertexMap_t::const_iterator it = vertices_.find(vertex_id);
            if (it != vertices_.end())
                CPPUNIT_FAIL("Duplicate vertex!");
            vertices_[vertex_id] = VertexStr(on_boundary, x, y);
            return true;
        }

        bool
        buildFace(IGeometricEntity::Id_t face_id, bool on_boundary, std::vector<IGeometricEntity::Id_t> const & vertex_ids) {
            FaceMap_t::const_iterator it = faces_.find(face_id);
            if (it != faces_.end())
                CPPUNIT_FAIL("Duplicate face!");
            faces_[face_id] = FaceStr(on_boundary, vertex_ids);
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

        boost::optional<Mesh::Ptr>
        getMesh() const {
            return boost::optional<Mesh::Ptr>();
        }

    public:
        typedef std::map<IGeometricEntity::Id_t, VertexStr> VertexMap_t;
        VertexMap_t vertices_;

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
    std::string mesh_filename_;
    static MockMeshBuilder mock_builder_;
};
