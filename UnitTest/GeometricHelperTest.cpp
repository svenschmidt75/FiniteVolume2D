#include "GeometricHelperTest.h"

#include "FiniteVolume2DLib/GeometricHelper.h"
#include "FiniteVolume2DLib/LineSegment.h"
#include "FiniteVolume2DLib/Ray.h"
#include "FiniteVolume2DLib/Line.h"


void
GeometricHelperTest::setUp() {}

void
GeometricHelperTest::tearDown() {}

void
GeometricHelperTest::LineSegmentIntersectionTest() {
    {
        LineSegment ls1(Vertex(0, 1), Vertex(1, 1));
        LineSegment ls2(Vertex(0, 0), Vertex(1, 2));

        // segments intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ls2);
        CPPUNIT_ASSERT_MESSAGE("Intersection point expected", bool(intersection_point_opt));

        Vertex intersection_point = *intersection_point_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 0.5, intersection_point.x(), 1E-10);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 1.0, intersection_point.y(), 1E-10);
    }

    {
        LineSegment ls1(Vertex(0, 1), Vertex(1, 1));
        LineSegment ls2(Vertex(0, 0), Vertex(2, 1));

        // segments do not intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ls2);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls1(Vertex(0, 1), Vertex(1, 1));
        LineSegment ls2(Vertex(0.5, 1), Vertex(2, 1));

        // segments overlap
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ls2);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls1(Vertex(0.5, 1), Vertex(1, 1));
        LineSegment ls2(Vertex(0, 1), Vertex(2, 1));

        // segment 1 contained in segment 2
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ls2);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls1(Vertex(0, 1), Vertex(1, 1));
        LineSegment ls2(Vertex(1.5, 1), Vertex(2, 1));

        // segments parallel
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ls2);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }
}

void 
GeometricHelperTest::LineSegmentRayIntersectionTest() {
    {
        LineSegment ls1(Vertex(0, 1), Vertex(1, 1));
        Ray ray(Vertex(0, 0), Vertex(1, 2));

        // segment/ray intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ray);
        CPPUNIT_ASSERT_MESSAGE("Intersection point expected", bool(intersection_point_opt));

        Vertex intersection_point = *intersection_point_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 0.5, intersection_point.x(), 1E-10);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 1.0, intersection_point.y(), 1E-10);
    }

    {
        LineSegment ls1(Vertex(0, 3), Vertex(1, 3));
        Ray ray(Vertex(0, 0), Vertex(0.25, 1));

        // segment/ray intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ray);
        CPPUNIT_ASSERT_MESSAGE("Intersection point expected", bool(intersection_point_opt));

        Vertex intersection_point = *intersection_point_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 0.75, intersection_point.x(), 1E-10);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 3.0, intersection_point.y(), 1E-10);
    }


    {
        LineSegment ls1(Vertex(0, 1), Vertex(1, 1));
        Ray ray(Vertex(0.25, 2), Vertex(1, 4));

        // segment/ray do not intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls1, ray);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Ray ray(Vertex(0, 0), Vertex(2, 1));

        // segments do not intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, ray);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Ray ray(Vertex(0.5, 1), Vertex(2, 1));

        // segment/ray overlap
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, ray);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0.5, 1), Vertex(1, 1));
        Ray ray(Vertex(0, 1), Vertex(2, 1));

        // segment contained in ray
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, ray);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Ray ray(Vertex(1.5, 1), Vertex(2, 1));

        // segment and ray parallel
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, ray);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }
}

void 
GeometricHelperTest::LineSegmentLineIntersectionTest() {
    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Line line(Vertex(0, 0), Vertex(1, 2));

        // segment/line intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("Intersection point expected", bool(intersection_point_opt));

        Vertex intersection_point = *intersection_point_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 0.5, intersection_point.x(), 1E-10);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 1.0, intersection_point.y(), 1E-10);
    }

    {
        LineSegment ls(Vertex(0, 3), Vertex(1, 3));
        Line line(Vertex(0, 0), Vertex(0.25, 1));

        // segment/line intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("Intersection point expected", bool(intersection_point_opt));

        Vertex intersection_point = *intersection_point_opt;
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 0.75, intersection_point.x(), 1E-10);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Intersection point error", 3.0, intersection_point.y(), 1E-10);
    }


    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Line line(Vertex(0.25, 2), Vertex(1, 4));

        // segment/line do not intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Line line(Vertex(0, 0), Vertex(2, 1));

        // segments do not intersect
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Line line(Vertex(0.5, 1), Vertex(2, 1));

        // segment/line overlap
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0.5, 1), Vertex(1, 1));
        Line line(Vertex(0, 1), Vertex(2, 1));

        // segment contained in line
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Line line(Vertex(1.5, 1), Vertex(2, 1));

        // segment and line parallel
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }

    {
        LineSegment ls(Vertex(0, 1), Vertex(1, 1));
        Line line(Vertex(1.5, 2), Vertex(2, 2));

        // segment and line parallel, but not collinear
        boost::optional<Vertex> intersection_point_opt = GeometricHelper::intersect(ls, line);
        CPPUNIT_ASSERT_MESSAGE("No intersection point expected", !bool(intersection_point_opt));
    }
}
