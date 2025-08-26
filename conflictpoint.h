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
    // --- RENAMED MEMBERS ---
    const Connection* priorityConnection_; // The connection that has the right-of-way
    const Connection* yieldConnection_;    // The connection that must yield

    const QPointF position_;
    const qreal distanceFromPriority_; // Distance along the priority connection's path
    const qreal distanceFromYield_;    // Distance along the yielding connection's path

public:
    ConflictPoint(const Connection* priorityConnection,
                  const Connection* yieldConnection,
                  const QPointF& position,
                  qreal distanceFromPriority,
                  qreal distanceFromYield)
        : priorityConnection_(priorityConnection)
        , yieldConnection_(yieldConnection)
        , position_(position)
        , distanceFromPriority_(distanceFromPriority)
        , distanceFromYield_(distanceFromYield)
    {}

    // --- UPDATED ACCESSORS ---
    const Connection* priorityConnection() const { return priorityConnection_; }
    const Connection* yieldConnection() const { return yieldConnection_; }
    QPointF position() const { return position_; }
    qreal distanceFromPriority() const { return distanceFromPriority_; }
    qreal distanceFromYield() const { return distanceFromYield_; }
    bool isPriority(const Connection* c) const { return c == priorityConnection_; }

    qreal distanceFrom(const Connection* connection) const
    {
        if (connection == priorityConnection_) return distanceFromPriority_;
        if (connection == yieldConnection_) return distanceFromYield_;
        throw std::invalid_argument("Provided connection not part of this conflict point.");
    }

    ConflictType classify() const
    {
        const bool sameSrc = priorityConnection_->source() == yieldConnection_->source();
        const bool sameDst = priorityConnection_->destination() == yieldConnection_->destination();
        if (sameSrc && sameDst) throw std::runtime_error("Overlap conflict not supported.");
        if (sameDst) return ConflictType::Merging;
        if (sameSrc) return ConflictType::Diverging;
        return ConflictType::Crossing;
    }
};
