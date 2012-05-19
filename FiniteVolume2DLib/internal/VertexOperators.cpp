/* Note: This file MUST be excluded from the project.
 *       It is included by Vertex.cpp ONLY!!!
 */

Vertex
operator+(Vertex const & lhs, Vertex const & rhs) {
    return Vertex(lhs.x() + rhs.x(), lhs.y() + rhs.y());
}

Vertex
operator/(Vertex const & in, double const value) {
    return Vertex(in.x() / value, in.y() / value);
}
