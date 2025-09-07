#pragma once

#include <QPainterPath>
#include <QPointF>
#include <QLineF>
#include <vector>
#include <memory>

/**
 * @brief Stateless calculator used for calculating conflict point positions based on connection geometry
 */
class ConflictGeometryCalculator {
public:
    struct ConflictPointBlueprint {
        QPointF point;
        qreal distanceAlongPath1;
        qreal distanceAlongPath2;
    };

    ///
    /**
     * @brief Main entry point – calculate all intersections between two paths
     * @param path1
     * @param path2
     * @param tolerance for merging close intersection points.
     * @return Blueprint consisting of calculated conflict point position, distance to conflict point along path 1 and path 2.
     */
    static std::vector<ConflictGeometryCalculator::ConflictPointBlueprint> calculatePathIntersectionPoints(const QPainterPath& path1,
                                                                                                           const QPainterPath& path2,
                                                                                                           qreal tolerance);

private:
    class LineSegment;
    class QuadraticSegment;
    class CubicSegment;

    class Segment {
    public:
        virtual ~Segment() = default;

        /**
         * @brief evaluate position at parameter
         * @param t parameter [0, 1]
         * @return position
         */
        virtual QPointF evaluateAt(qreal t) const = 0;
        virtual qreal length() const = 0;

        /**
         * @brief distanceToPoint
         * @param p point
         * @return distance in m along the segment
         */
        virtual qreal distanceToPoint(const QPointF& p) const = 0;

        /**
         * @brief intersects this segment with another
         * @param other segment
         * @param tol tolerance for merging close intersection points
         * @return vector of intersection points
         */
        virtual std::vector<QPointF> intersect(const Segment& other, qreal tol) const = 0;
        virtual std::vector<QPointF> intersectWith(const LineSegment& other, qreal tol) const = 0;
        virtual std::vector<QPointF> intersectWith(const QuadraticSegment& other, qreal tol) const = 0;
        virtual std::vector<QPointF> intersectWith(const CubicSegment& other, qreal tol) const = 0;
    };

    class LineSegment : public Segment {
    public:
        QPointF p0, p1;
        LineSegment(QPointF s, QPointF e) : p0(s), p1(e) {}

        QPointF evaluateAt(qreal t) const override { return (1 - t) * p0 + t * p1; }
        qreal length() const override { return QLineF(p0, p1).length(); }
        qreal distanceToPoint(const QPointF& p) const override;

        std::vector<QPointF> intersect(const Segment& other, qreal tol) const override { return other.intersectWith(*this, tol); }
        std::vector<QPointF> intersectWith(const LineSegment& other, qreal tol) const override;
        std::vector<QPointF> intersectWith(const QuadraticSegment& other, qreal tol) const override;
        std::vector<QPointF> intersectWith(const CubicSegment& other, qreal tol) const override;
    };

    class QuadraticSegment : public Segment {
    public:
        QPointF p0, c1, p1;
        QuadraticSegment(QPointF start, QPointF control, QPointF end)
            : p0(start), c1(control), p1(end) {}

        QPointF evaluateAt(qreal t) const override;
        qreal length() const override;
        qreal distanceToPoint(const QPointF& p) const override;

        std::vector<QPointF> intersect(const Segment& other, qreal tol) const override { return other.intersectWith(*this, tol); }
        std::vector<QPointF> intersectWith(const LineSegment& other, qreal tol) const override;
        std::vector<QPointF> intersectWith(const QuadraticSegment& other, qreal tol) const override;
        std::vector<QPointF> intersectWith(const CubicSegment& other, qreal tol) const override;
    };

    class CubicSegment : public Segment {
    public:
        QPointF p0, c1, c2, p1;
        CubicSegment(QPointF start, QPointF control1, QPointF control2, QPointF end)
            : p0(start), c1(control1), c2(control2), p1(end) {}

        QPointF evaluateAt(qreal t) const override;
        qreal length() const override;
        qreal distanceToPoint(const QPointF& p) const override;

        std::vector<QPointF> intersect(const Segment& other, qreal tol) const override { return other.intersectWith(*this, tol); }
        std::vector<QPointF> intersectWith(const LineSegment& other, qreal tol) const override;
        std::vector<QPointF> intersectWith(const QuadraticSegment& other, qreal tol) const override;
        std::vector<QPointF> intersectWith(const CubicSegment& other, qreal tol) const override;
    };

    // Helpers

    /**
     * @brief Extract segments from a QPainterPath object
     * @param path The path from which to extract
     * @return vector of segment objects - either lines, quad curves or cubic curves
     */
    static std::vector<std::unique_ptr<Segment>> extractSegments(const QPainterPath& path);

    /**
    * @brief Flattens a curve segment into a series of line segments (a polyline).
    * @param segment The curve segment to flatten.
    * @param steps The number of line segments to create.
    * @return A vector of points representing the polyline.
    */
    static std::vector<QPointF> flattenSegment(const Segment& segment, int steps = 50);

    /**
     * @brief Intersects two polylines.
     * @param poly1 The first polyline (vector of points).
     * @param poly2 The second polyline.
     * @param tol Tolerance for merging close intersection points.
     * @return A vector of unique intersection points.
     */
    static std::vector<QPointF> intersectPolylines(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2, qreal tol);
};
