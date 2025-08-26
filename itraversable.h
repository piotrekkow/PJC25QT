#pragma once
#include <vector>
#include <qtypes.h>
#include "PriorityType.h"

class QPainterPath;
class GeometryManager;
class Intersection;

class ITraversable
{
public:
    enum class TraversableType {
        Lane,
        Connection
    };

    virtual ~ITraversable() = default;

    virtual const QPainterPath& path(const GeometryManager* geometryManager) const = 0;
    virtual qreal length(const GeometryManager* geometryManager) const = 0;
    virtual TraversableType type() const = 0;
    virtual PriorityType roadwayPriority() const = 0;

    /**
     * @brief connections parent intersection or lane's target intersection getter
     * @return
     */
    virtual const Intersection* intersection() const = 0;

    /**
     * @brief next traversable element getter
     * @return
     */
    virtual std::vector<ITraversable*> next() const = 0;
};
