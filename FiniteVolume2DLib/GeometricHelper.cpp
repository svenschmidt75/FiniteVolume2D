#include "GeometricHelper.h"

#include "Vertex.h"
#include "LineSegment.h"
#include "Ray.h"
#include "ParametrizedLineSegment.h"
#include "Math.h"

#include <cassert>


// disable "warning C4482: nonstandard extension used: enum 'GeometricHelper::IntersectionType' used in qualified name"
#pragma warning(disable:4482)


namespace {
    
    typedef std::tuple<GeometricHelper::IntersectionType, Vertex, Vertex> SegmentIntersection_t;

    std::tuple<int, double, double>
    find_intersection_interval(double u0, double u1, double v0, double v1) {
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
    intersect_line_segments_base(LineSegment const & ls1, LineSegment const & ls2) {
        /* Intersect two line segments.
         * This code is based on p. 244,
         * Geometric Tools for Computer Graphics,
         * Philip J. Schneider; David H. Eberly
         * 
         * ls1: P_{0} + \lambda \vec{d0}
         * ls2: P_{1} + \mu     \vec{d1}
         * 
         * ls1 = ls2 => P_{0} - P_{1} = \mu \vec{d1} - \lambda \vec{d0}
         * 
         * lambda: cross with \vec{d1} =>
         * (P_{0} - P_{1}) x \vec{d1} = - \lambda (\vec{d0} x \vec{d1}) = \lambda (\vec{d1} x \vec{d0}) 
         * 
         * mu: cross with \vec{d0} =>
         * (P_{0} - P_{1}) x \vec{d0} = \mu (\vec{d1} x \vec{d0})
         */
        ParametrizedLineSegment pls1(ls1);
        ParametrizedLineSegment pls2(ls2);

        Vector E = pls2.p0() - pls1.p0();

        static double eps = 1E-10;

        double kross = Math::cross(pls1.dir(), pls2.dir());
        double sqrKross = kross * kross;
        double sqrLen1 = pls1.length();
        double sqrLen2 = pls2.length();

        // relative test!
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
            return std::make_tuple(GeometricHelper::UNIQUE_INTERSECTION, pls1.get(s), Vertex());
        }

        // both segments are parallel

        double sqrLenE = E.norm();
        kross = Math::cross(E, pls1.dir());
        sqrKross = kross * kross;
        if (sqrKross > eps * sqrLen1 * sqrLenE)
            // segments are different
            return std::make_tuple(GeometricHelper::EMPTY_PARALLEL, Vertex(), Vertex());

        
        // line segments overlap

        // compute the two point marking the overlap region
        double s0 = Math::dot(pls1.dir(), E) / sqrLen1;
        double s1 = s0 + Math::dot(pls1.dir(), pls2.dir()) / sqrLen1;
        double smin = std::min(s0, s1);
        double smax = std::max(s0, s1);
        double u0;
        double u1;
        int imax;
        std::tie(imax, u0, u1) = find_intersection_interval(0.0, 1.0, smin, smax);

        Vertex p0;
        Vertex p1;

        assert(imax);

        if (imax--) {
            p0 = pls1.get(u0);
            p1 = imax ? pls1.get(u1) : p0;
        }

        return std::make_tuple(GeometricHelper::OVERLAP, p0, p1);
    }

    
    SegmentIntersection_t
    intersect_line_segment_ray_base(LineSegment const & ls, Ray const & ray) {
        /* Intersect one line segment with a ray.
         * This code is based on p. 244,
         * Geometric Tools for Computer Graphics,
         * Philip J. Schneider; David H. Eberly
         */
        ParametrizedLineSegment pls(ls);

        Vector E = ray.p0() - pls.p0();

        static double eps = 1E-10;

        double kross = Math::cross(pls.dir(), ray.dir());
        double sqrKross = kross * kross;
        double sqrLen1 = pls.length();
        double sqrLen2 = ray.dir().norm();

        if (sqrKross > eps * sqrLen1 * sqrLen2) {
            // line segments are not parallel
            double s = Math::cross(E, ray.dir()) / kross;
            if (s < 0 || s > 1)
                // intersection point is not on segment 1
                return std::make_tuple(GeometricHelper::EMPTY, Vertex(), Vertex());

            // both segments intersect
            return std::make_tuple(GeometricHelper::UNIQUE_INTERSECTION, pls.get(s), Vertex());
        }

        // the line segment and the ray are parallel,
        // hence they overlap
        return std::make_tuple(GeometricHelper::OVERLAP, pls.p0(), pls.p1());
    }

}

boost::optional<Vertex>
GeometricHelper::intersect(LineSegment const & ls1, LineSegment const & ls2) {
    Vertex intersection_point;
    GeometricHelper::IntersectionType type;

    std::tie(type, intersection_point) = intersect_line_segments_base(ls1, ls2);

    if (type == IntersectionType::UNIQUE_INTERSECTION)
        return boost::optional<Vertex>(intersection_point);
    return boost::optional<Vertex>();
}

boost::optional<Vertex>
GeometricHelper::intersect(LineSegment const & ls, Ray const & ray) {
    Vertex intersection_point;
    GeometricHelper::IntersectionType type;

    std::tie(type, intersection_point) = intersect_line_segment_ray_base(ls, ray);

    if (type == IntersectionType::UNIQUE_INTERSECTION)
        return boost::optional<Vertex>(intersection_point);
    return boost::optional<Vertex>();
}

#pragma warning(default:4482)
