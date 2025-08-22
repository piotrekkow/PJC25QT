#pragma once
#include <QPainterPath>
#include "connection.h"

class GeometryManager;

class ConflictPoint
{
public:
    enum class ConflictType
    {
        Crossing,
        Merging,
        Diverging
    };

private:
    const Connection* connectionA_;
    const Connection* connectionB_;
    const QPointF position_;
    const qreal distanceFromA_;
    const qreal distanceFromB_;

public:
    ConflictPoint(const Connection* connectionA,
                  const Connection* connectionB,
                  const QPointF& position,
                  qreal distanceFromA,
                  qreal distanceFromB)
        : connectionA_(connectionA)
        , connectionB_(connectionB)
        , position_(position)
        , distanceFromA_(distanceFromA)
        , distanceFromB_(distanceFromB)
    {}

    const Connection* connectionA() const { return connectionA_; }
    const Connection* connectionB() const { return connectionB_; }
    QPointF position() const { return position_; }
    qreal distanceFromA() const { return distanceFromA_; }
    qreal distanceFromB() const { return distanceFromB_; }

    qreal distanceFrom(const Connection* connection) const
    {
        if (connection == connectionA_) return distanceFromA_;
        if (connection == connectionB_) return distanceFromB_;
        throw std::invalid_argument("Provided connection not part of this conflict point.");
    }

    ConflictType classify() const
    {
        const bool sameSrc = connectionA_->source() == connectionB_->source();
        const bool sameDst = connectionA_->destination() == connectionB_->destination();
        if (sameSrc && sameDst) throw std::runtime_error("Overlap conflict not supported.");
        if (sameDst) return ConflictType::Merging;
        if (sameSrc) return ConflictType::Diverging;
        return ConflictType::Crossing;
    }
};
