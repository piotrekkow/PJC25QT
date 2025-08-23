#pragma once
#include <vector>
#include <qtypes.h>

class QPainterPath;
class GeometryManager;

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

    /**
     * @brief next traversable element getter
     * @return
     */
    virtual std::vector<ITraversable*> next() const = 0;

};
