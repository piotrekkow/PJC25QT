#include "conflictgeometrycalculator.h"

std::vector<ConflictGeometryCalculator::ConflictPointBlueprint>
ConflictGeometryCalculator::calculatePathIntersectionPoints(const QPainterPath& path1,
                                                            const QPainterPath& path2,
                                                            qreal tolerance)
{
    std::vector<ConflictPointBlueprint> results;
    if (path1.isEmpty() || path2.isEmpty()) return results;

    auto segments1 = extractSegments(path1);
    auto segments2 = extractSegments(path2);

    qreal cumulative1 = 0.0;
    for (const auto& seg1 : segments1)
    {
        qreal segLen1 = seg1->length();

        qreal cumulative2 = 0.0;
        for (const auto& seg2 : segments2)
        {
            qreal segLen2 = seg2->length();

            auto pts = seg1->intersect(*seg2, tolerance);
            for (const auto& p : pts)
            {
                ConflictPointBlueprint cp;
                cp.point = p;
                cp.distanceAlongPath1 = cumulative1 + seg1->distanceToPoint(p);
                cp.distanceAlongPath2 = cumulative2 + seg2->distanceToPoint(p);
                results.push_back(cp);
            }

            cumulative2 += segLen2;
        }

        cumulative1 += segLen1;
    }

    return results;
}

std::vector<QPointF> ConflictGeometryCalculator::flattenSegment(const Segment &segment, int steps)
{
    std::vector<QPointF> points;
    points.reserve(steps + 1);
    for (int i = 0; i <= steps; ++i)
    {
        qreal t = static_cast<qreal>(i) / steps;
        points.push_back(segment.evaluateAt(t));
    }
    return points;
}

std::vector<QPointF> ConflictGeometryCalculator::intersectPolylines(const std::vector<QPointF> &poly1, const std::vector<QPointF> &poly2, qreal tol)
{
    std::vector<QPointF> intersections;
    if (poly1.size() < 2 || poly2.size() < 2)
    {
        return intersections;
    }

    for (size_t i = 0; i < poly1.size() - 1; ++i)
    {
        QLineF line1(poly1[i], poly1[i+1]);
        for (size_t j = 0; j < poly2.size() - 1; ++j)
        {
            QLineF line2(poly2[j], poly2[j+1]);
            QPointF intersectionPoint;

            if (line1.intersects(line2, &intersectionPoint) == QLineF::BoundedIntersection)
            {
                // Check if this point is too close to an existing one
                bool tooClose = false;
                for (const auto& existingPoint : intersections)
                {
                    if (QLineF(existingPoint, intersectionPoint).length() < tol)
                    {
                        tooClose = true;
                        break;
                    }
                }
                if (!tooClose)
                {
                    intersections.push_back(intersectionPoint);
                }
            }
        }
    }
    return intersections;
}

std::vector<std::unique_ptr<ConflictGeometryCalculator::Segment> > ConflictGeometryCalculator::extractSegments(const QPainterPath &path)
{
    std::vector<std::unique_ptr<Segment>> segments;

    if (path.elementCount() == 0)
        return segments;

    QPointF current = path.elementAt(0); // starting point
    for (int i = 1; i < path.elementCount(); ++i)
    {
        QPainterPath::Element elem = path.elementAt(i);

        switch (elem.type)
        {
        case QPainterPath::LineToElement:
        {
            QPointF p1 = current;
            QPointF p2(elem.x, elem.y);
            segments.push_back(std::make_unique<LineSegment>(p1, p2));
            current = p2;
            break;
        }

        case QPainterPath::CurveToElement:
        {
            // CurveToElement means cubic or quadratic.
            // Qt encodes as:
            //   CurveToElement     = first control point
            //   CurveToDataElement = second control point (for cubic) OR end point (for quad)
            //   CurveToDataElement = end point (for cubic)

            if (i + 1 >= path.elementCount())
                break;

            QPainterPath::Element elem1 = path.elementAt(i);
            QPainterPath::Element elem2 = path.elementAt(i + 1);

            if (i + 2 < path.elementCount()
                && path.elementAt(i + 2).type == QPainterPath::CurveToDataElement)
            {
                // Cubic
                QPointF p0 = current;
                QPointF c1(elem1.x, elem1.y);
                QPointF c2(elem2.x, elem2.y);
                QPointF p3(path.elementAt(i + 2).x, path.elementAt(i + 2).y);

                segments.push_back(std::make_unique<CubicSegment>(p0, c1, c2, p3));
                current = p3;
                i += 2; // consumed three elements
            }
            else
            {
                // Quadratic (encoded as CurveToElement + CurveToDataElement)
                QPointF p0 = current;
                QPointF c(elem1.x, elem1.y);
                QPointF p2(elem2.x, elem2.y);

                segments.push_back(std::make_unique<QuadraticSegment>(p0, c, p2));
                current = p2;
                i += 1; // consumed two elements
            }
            break;
        }

        case QPainterPath::MoveToElement: {
            // restart a new subpath
            current = QPointF(elem.x, elem.y);
            break;
        }

        default:
            break;
        }
    }

    return segments;
}

qreal ConflictGeometryCalculator::LineSegment::distanceToPoint(const QPointF &p) const
{
    QLineF seg(p0, p1);
    qreal totalLen = seg.length();
    if (totalLen <= 1e-9) return 0.0;

    qreal dot = QPointF::dotProduct(p - p0, p1 - p0);
    qreal t = dot / (totalLen * totalLen);
    return std::clamp(t, 0.0, 1.0) * totalLen;
}


QPointF ConflictGeometryCalculator::QuadraticSegment::evaluateAt(qreal t) const
{
    qreal u = 1 - t;
    return u * u * p0 + 2 * u * t * c1 + t * t * p1;
}

qreal ConflictGeometryCalculator::QuadraticSegment::length() const
{
    const int N = 50;
    QPointF prev = p0;
    qreal len = 0.0;
    for (int i = 1; i <= N; ++i) {
        qreal t = (qreal)i / N;
        QPointF curr = evaluateAt(t);
        len += QLineF(prev, curr).length();
        prev = curr;
    }
    return len;
}

qreal ConflictGeometryCalculator::QuadraticSegment::distanceToPoint(const QPointF &p) const
{
    const int N = 50;
    QPointF prev = p0;
    qreal lenSoFar = 0.0;
    qreal bestDist = std::numeric_limits<qreal>::max();
    qreal bestLen = 0.0;

    for (int i = 1; i <= N; ++i)
    {
        qreal t = (qreal)i / N;
        QPointF curr = evaluateAt(t);

        qreal dist = QLineF(curr, p).length();
        if (dist < bestDist)
        {
            bestDist = dist;
            bestLen = lenSoFar + QLineF(prev, curr).length();
        }

        lenSoFar += QLineF(prev, curr).length();
        prev = curr;
    }
    return bestLen;
}

QPointF ConflictGeometryCalculator::CubicSegment::evaluateAt(qreal t) const
{
    qreal u = 1 - t;
    qreal u2 = u * u;
    qreal u3 = u2 * u;
    qreal t2 = t * t;
    qreal t3 = t2 * t;
    return u3 * p0 + 3 * u2 * t * c1 + 3 * u * t2 * c2 + t3 * p1;
}

qreal ConflictGeometryCalculator::CubicSegment::length() const
{
    const int N = 50;
    QPointF prev = p0;
    qreal len = 0.0;
    for (int i = 1; i <= N; ++i)
    {
        qreal t = (qreal)i / N;
        QPointF curr = evaluateAt(t);
        len += QLineF(prev, curr).length();
        prev = curr;
    }
    return len;
}

qreal ConflictGeometryCalculator::CubicSegment::distanceToPoint(const QPointF &p) const
{
    const int N = 50;
    QPointF prev = p0;
    qreal lenSoFar = 0.0;
    qreal bestDist = std::numeric_limits<qreal>::max();
    qreal bestLen = 0.0;

    for (int i = 1; i <= N; ++i)
    {
        qreal t = (qreal)i / N;
        QPointF curr = evaluateAt(t);

        qreal dist = QLineF(curr, p).length();
        if (dist < bestDist)
        {
            bestDist = dist;
            bestLen = lenSoFar + QLineF(prev, curr).length();
        }

        lenSoFar += QLineF(prev, curr).length();
        prev = curr;
    }
    return bestLen;
}

// =================================================================================
//  LineSegment Intersection Implementations
// =================================================================================

/**
 * @brief Intersects two line segments.
 * Uses QLineF for a robust intersection calculation.
 */
std::vector<QPointF> ConflictGeometryCalculator::LineSegment::intersectWith(const LineSegment& other, qreal tol) const
{
    std::vector<QPointF> intersections;
    QLineF line1(p0, p1);
    QLineF line2(other.p0, other.p1);

    QPointF intersectionPoint;
    // Use QLineF's intersection method which is robust.
    // We check for BoundedIntersection to ensure the intersection is within both segments.
    if (line1.intersects(line2, &intersectionPoint) == QLineF::BoundedIntersection)
    {
        intersections.push_back(intersectionPoint);
    }
    Q_UNUSED(tol);
    return intersections;
}

/**
 * @brief Intersects a line with a quadratic Bezier curve.
 * This is handled by the QuadraticSegment's implementation.
 */
std::vector<QPointF> ConflictGeometryCalculator::LineSegment::intersectWith(const QuadraticSegment& other, qreal tol) const
{
    return other.intersectWith(*this, tol);
}

/**
 * @brief Intersects a line with a cubic Bezier curve.
 * This is handled by the CubicSegment's implementation.
 */
std::vector<QPointF> ConflictGeometryCalculator::LineSegment::intersectWith(const CubicSegment& other, qreal tol) const
{
    return other.intersectWith(*this, tol);
}

// =================================================================================
//  QuadraticSegment Intersection Implementations
// =================================================================================

/**
 * @brief Intersects a quadratic Bezier curve with a line segment.
 * This involves solving a quadratic equation derived from the curve and line formulas.
 */
std::vector<QPointF> ConflictGeometryCalculator::QuadraticSegment::intersectWith(const LineSegment& other, qreal tol) const
{
    std::vector<QPointF> intersections;

    // Bezier curve: B(t) = (1-t)^2*P0 + 2(1-t)t*C1 + t^2*P1
    // Line: L(s) = (1-s)*L0 + s*L1
    // Set B(t).x = L(s).x and B(t).y = L(s).y and solve for t.

    // Rearrange Bezier into At^2 + Bt + C form
    QPointF A = p0 - 2 * c1 + p1;
    QPointF B = 2 * (c1 - p0);
    QPointF C = p0;

    // Line direction vector
    QPointF lineDir = other.p1 - other.p0;

    // We use a perpendicular vector to the line to create an implicit line equation:
    // (P - L0) . Perp = 0
    QPointF linePerp(-lineDir.y(), lineDir.x());

    // Substitute Bezier equation into implicit line equation:
    // (A*t^2 + B*t + C - L0) . Perp = 0
    // This gives a quadratic equation in t: a*t^2 + b*t + c = 0
    qreal a = QPointF::dotProduct(A, linePerp);
    qreal b = QPointF::dotProduct(B, linePerp);
    qreal c = QPointF::dotProduct(C - other.p0, linePerp);

    // Solve the quadratic equation for t
    if (std::abs(a) < 1e-9) // It's actually a linear equation
    {
        if (std::abs(b) > 1e-9)
        {
            qreal t = -c / b;
            if (t >= -tol && t <= 1.0 + tol)
            {
                intersections.push_back(evaluateAt(std::clamp(t, 0.0, 1.0)));
            }
        }
    }
    else
    {
        qreal discriminant = b * b - 4 * a * c;
        if (discriminant >= 0)
        {
            qreal sqrt_d = std::sqrt(discriminant);
            qreal t1 = (-b + sqrt_d) / (2 * a);
            qreal t2 = (-b - sqrt_d) / (2 * a);

            if (t1 >= -tol && t1 <= 1.0 + tol)
            {
                intersections.push_back(evaluateAt(std::clamp(t1, 0.0, 1.0)));
            }
            if (t2 >= -tol && t2 <= 1.0 + tol)
            {
                intersections.push_back(evaluateAt(std::clamp(t2, 0.0, 1.0)));
            }
        }
    }

    // Filter out points that are not on the line segment
    std::vector<QPointF> final_points;
    QLineF line(other.p0, other.p1);
    for (const auto& p : intersections)
    {
        // Project point onto the line to find its parameter 's'
        qreal dx = line.dx(), dy = line.dy();
        qreal s = 0.0;
        if (std::abs(dx) > std::abs(dy))
        {
            s = (p.x() - other.p0.x()) / dx;
        }
        else if (std::abs(dy) > 1e-9)
        {
            s = (p.y() - other.p0.y()) / dy;
        }
        // Check if the parameter 's' is within the line segment bounds [0, 1]
        if (s >= -tol && s <= 1.0 + tol)
        {
            final_points.push_back(p);
        }
    }

    return final_points;
}

/**
 * @brief Intersects two quadratic Bezier curves by flattening them.
 */
std::vector<QPointF> ConflictGeometryCalculator::QuadraticSegment::intersectWith(const QuadraticSegment& other, qreal tol) const
{
    auto poly1 = flattenSegment(*this);
    auto poly2 = flattenSegment(other);
    return intersectPolylines(poly1, poly2, tol);
}

/**
 * @brief Intersects a quadratic and a cubic Bezier curve by flattening them.
 */
std::vector<QPointF> ConflictGeometryCalculator::QuadraticSegment::intersectWith(const CubicSegment& other, qreal tol) const
{
    auto poly1 = flattenSegment(*this);
    auto poly2 = flattenSegment(other);
    return intersectPolylines(poly1, poly2, tol);
}


// =================================================================================
//  CubicSegment Intersection Implementations
// =================================================================================

/**
 * @brief Intersects a cubic Bezier curve with a line segment.
 * This is more complex, involving solving a cubic equation. For simplicity and
 * robustness, we'll use the flattening approach here as well.
 */
std::vector<QPointF> ConflictGeometryCalculator::CubicSegment::intersectWith(const LineSegment& other, qreal tol) const
{
    // An analytical solution requires solving a cubic equation, which can be complex.
    // Flattening is a simpler and numerically stable approach that fits the project's style.
    auto poly1 = flattenSegment(*this, 100); // Use higher resolution for cubic
    std::vector<QPointF> poly2 = {other.p0, other.p1};
    return intersectPolylines(poly1, poly2, tol);
}

/**
 * @brief Intersects a cubic and a quadratic Bezier curve by flattening them.
 */
std::vector<QPointF> ConflictGeometryCalculator::CubicSegment::intersectWith(const QuadraticSegment& other, qreal tol) const
{
    auto poly1 = flattenSegment(*this);
    auto poly2 = flattenSegment(other);
    return intersectPolylines(poly1, poly2, tol);
}

/**
 * @brief Intersects two cubic Bezier curves by flattening them.
 */
std::vector<QPointF> ConflictGeometryCalculator::CubicSegment::intersectWith(const CubicSegment& other, qreal tol) const
{
    auto poly1 = flattenSegment(*this);
    auto poly2 = flattenSegment(other);
    return intersectPolylines(poly1, poly2, tol);
}
