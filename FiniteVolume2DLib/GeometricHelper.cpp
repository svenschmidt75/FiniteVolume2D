#include "GeometricHelper.h"

#include "Vertex.h"
#include "LineSegment.h"
#include "Ray.h"
#include "ParametrizedLineSegment.h"
#include "Math.h"


// disable "warning C4482: nonstandard extension used: enum 'GeometricHelper::IntersectionType' used in qualified name"
#pragma warning(disable:4482)


namespace {
    
    typedef std::tuple<GeometricHelper::IntersectionType, Vertex, Vertex> SegmentIntersection_t;

    std::tuple<int, double, double>
    findIntersectionInterval(double u0, double u1, double v0, double v1) {
        /* Intersect two intervals.
         * This code is based on p. 245,
         * Geometric Tools for Computer Graphics,
         * Philip J. Schneider; David H. Eberly
         * 
         * |-----------| |---------------|
         * u0         u1 v0             v1
         * 
         * 1. (u0, u1, v0, v1)   : no overlap
         * 2. (v0, v1, u0, u1)   : no overlap
         * 3. (v0, u0, v1, u1)
         * 4. (v0, u0, u1, v1)   : (u0, u1) contained in (v0, v1)
         * 5. (u0, v0, u1, v1)
         * 6. (u0, v0, v1, u1)   : (v0, v1) contained in (u0, u1)
         */


        // 1 or 2
        if (u1 < v0 || u0 > v1)
            // no overlap
            return std::make_tuple(0, 0, 0);

        // 3 to 6
        if (u1 > v0) {
            // 3 to 6
            if (u0 < v1) {
                double p0 = u0 < v0 ? v0 : u0;
                double p1 = u1 > v1 ? v1 : u1;

                return std::make_tuple(2, p0, p1);
            }
            else
                // u0 == v1
                return std::make_tuple(1, u0, u0);
        }
        else
            // u1 == v0
            return std::make_tuple(1, u1, u1);

        // will never happen
    }

    SegmentIntersection_t
    intersectLineSegmentBase(LineSegment const & ls1, LineSegment const & ls2) {
        /* Intersect two line segments.
         * This code is based on p. 244,
         * Geometric Tools for Computer Graphics,
         * Philip J. Schneider; David H. Eberly
         * 
         * LineSegment: (p0, p1)
         */
        ParametrizedLineSegment pls1(ls1);
        ParametrizedLineSegment pls2(ls2);

        Vector E = pls2.p0() - pls1.p0();

        static double eps = 1E-10;

        double kross = Math::cross(pls1.dir(), pls2.dir());
        double sqrKross = kross * kross;
        double sqrLen1 = pls1.length();
        double sqrLen2 = pls2.length();

        if (sqrKross > eps * sqrLen1 * sqrLen2) {
            // line segments are not parallel
            double s = Math::cross(E, pls2.dir()) / kross;
            if (s < 0 || s > 1)
                // intersection point is not on segment 1
                return std::make_tuple(GeometricHelper::EMPTY, Vertex(), Vertex());

            double t = Math::cross(E, pls1.dir()) / kross;
            if (t < 0 || t > 1)
                // intersection point is not on segment 2
                return std::make_tuple(GeometricHelper::EMPTY, Vertex(), Vertex());

            // both segments intersect
            return std::make_tuple(GeometricHelper::UNIQUE_INTERSECTION, pls1.get(2), Vertex());
        }

        // both segments are parallel

        double sqrLenE = E.norm();
        kross = Math::cross(E, pls1.dir());
        sqrKross = kross * kross;
        if (sqrKross > eps * sqrLen1 * sqrLenE)
            // segments are different
            return std::make_tuple(GeometricHelper::EMPTY_PARALLEL, Vertex(), Vertex());

        
        // line segments overlap

        double s0 = Math::dot(pls1.dir(), E) / sqrLen1;
        double s1 = s0 + Math::dot(pls1.dir(), pls2.dir()) / sqrLen1;
        double smin = std::min(s0, s1);
        double smax = std::max(s0, s1);
        double u0;
        double u1;
        int imax;
        std::tie(imax, u0, u1) = findIntersectionInterval(0.0, 1.0, smin, smax);

        Vertex p0;
        Vertex p1;

        if (imax--) {
            p0 = pls1.get(u0);

        if (imax)
            p1 = pls1.get(u1);
        }

        return std::make_tuple(GeometricHelper::OVERLAP, p0, p1);
    }

}

boost::optional<Vertex>
GeometricHelper::intersect(LineSegment const & ls1, LineSegment const & ls2) {
    Vertex intersection_point;
    GeometricHelper::IntersectionType type;

    std::tie(type, intersection_point) = intersectLineSegmentBase(ls1, ls2);

    if (type == IntersectionType::UNIQUE_INTERSECTION)
        return boost::optional<Vertex>(intersection_point);
    return boost::optional<Vertex>();
}

boost::optional<Vertex>
GeometricHelper::intersect(LineSegment const & ls, Ray const & ray) {

}

#pragma warning(default:4482)
