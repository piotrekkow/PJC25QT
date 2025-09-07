#pragma once
#include <QPainterPath>
#include "connection.h"

class GeometryManager;

/**
 * @brief Conflict point between two connections at an intersection
 */
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
    const Connection* priorityConnection_;
    const Connection* yieldConnection_;

    const QPointF position_;
    const qreal distanceFromPriority_;
    const qreal distanceFromYield_;

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

    /**
     * @return The connection that has the right-of-way in this conflict based on the right hand rule
     */
    const Connection* priorityConnection() const { return priorityConnection_; }

    /**
     * @return The connection that has to yield in this conflict based on the right hand rule
     */
    const Connection* yieldConnection() const { return yieldConnection_; }

    /**
     * @return position in {m, m}
     */
    QPointF position() const { return position_; }

    /**
     * @return distance from the start of connection that has the right-of-way based on the right hand rule along it's path
     */
    qreal distanceFromPriority() const { return distanceFromPriority_; }

    /**
     * @return distance from the start of connection that has to yield based on the right hand rule along it's path
     */
    qreal distanceFromYield() const { return distanceFromYield_; }

    /**
     * @return does connection c have right of way based on the right hand rule
     */
    bool isRHPriority(const Connection* c) const { return c == priorityConnection_; }

    /**
     * @return distance from the start of connection along it's path
     */
    qreal distanceFrom(const Connection* connection) const
    {
        if (connection == priorityConnection_) return distanceFromPriority_;
        if (connection == yieldConnection_) return distanceFromYield_;
        throw std::invalid_argument("Provided connection not part of this conflict point.");
    }

    /**
     * @return classification based on connection relationship - merging, diverging, crossing
     */
    ConflictType classify() const
    {
        const bool sameSrc = priorityConnection_->source() == yieldConnection_->source();
        const bool sameDst = priorityConnection_->destination() == yieldConnection_->destination();
        if (sameSrc && sameDst) throw std::runtime_error("Overlap conflict not supported.");
        if (sameDst) return ConflictType::Merging;
        if (sameSrc) return ConflictType::Diverging;
        return ConflictType::Crossing;
    }

    /**
     * @return returns the other connection in this conflict that is not provided connection
     */
    const Connection* otherConnection(const Connection* connection)
    {
        if (connection == priorityConnection_) return yieldConnection_;
        if (connection == yieldConnection_) return priorityConnection_;
        throw std::invalid_argument("Provided connection not part of this conflict point.");
    }
};
