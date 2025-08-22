#pragma once

#include <QPainterPath>
#include <QPointF>
#include <QLineF>
#include <vector>
#include <memory>

class ConflictGeometryCalculator {
public:
    struct ConflictPointBlueprint {
        QPointF point;
        qreal distanceAlongPath1;
        qreal distanceAlongPath2;
    };

    /// Main entry point – calculate all intersections between two paths
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

        virtual QPointF evaluateAt(qreal t) const = 0;
        virtual qreal length() const = 0;
        virtual qreal distanceToPoint(const QPointF& p) const = 0;

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

    // ===== Helpers =====
    static std::vector<std::unique_ptr<Segment>> extractSegments(const QPainterPath& path);
    static std::vector<QPointF> flattenSegment(const Segment& segment, int steps = 50);
    static std::vector<QPointF> intersectPolylines(const std::vector<QPointF>& poly1, const std::vector<QPointF>& poly2, qreal tol);
};
