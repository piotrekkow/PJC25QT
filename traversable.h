#pragma once
#include <vector>
#include <qtypes.h>
#include <memory>
#include "PriorityType.h"

class QPainterPath;
class GeometryManager;
class Intersection;
class Roadway;
class NavigationStrategy;
class Vehicle;
class Traffic;

/**
 * @brief Base class along which agents travel
 */
class Traversable
{
public:
    virtual ~Traversable() = default;

    /**
     * @return traversables path geometry
     */
    virtual const QPainterPath& path(const GeometryManager* geometryManager) const = 0;
    virtual qreal length(const GeometryManager* geometryManager) const = 0;

    /**
     * @brief possible next traversables after reaching the end of this one
     */
    virtual std::vector<const Traversable*> next() const = 0;

    virtual std::unique_ptr<NavigationStrategy> createNavigationStrategy(
        Vehicle* vehicle,
        const Traffic* traffic,
        const GeometryManager* geometry
        ) const = 0;
};

/**
 * @brief Base class along which vehicles travel
 */
class VehicleTraversable : public Traversable
{
public:
    virtual ~VehicleTraversable() = default;
    virtual const QPainterPath& path(const GeometryManager* geometryManager) const = 0;
    virtual qreal length(const GeometryManager* geometryManager) const = 0;
    virtual std::vector<const Traversable*> next() const = 0;
    virtual PriorityType regulatoryPriority() const = 0;
    virtual qreal speedLimit() const = 0;

    /**
     * @brief intersection through which this traversable passes or is about to pass
     */
    virtual const Intersection* intersection() const = 0;

    virtual std::unique_ptr<NavigationStrategy> createNavigationStrategy(
        Vehicle* vehicle,
        const Traffic* traffic,
        const GeometryManager* geometry
        ) const = 0;
};

// PedestrianTraversable :
// Footpath
// Crossing

// class PedestrianCrossing : public Traversable
// {
//     qreal width_;

// public:
//     qreal width() const { return width_; }
//     void width(qreal width) { width_ = width; }

//     const QPainterPath& path(const GeometryManager* geometryManager) const override;
//     qreal length(const GeometryManager* geometryManager) const override;
//     std::vector<const Traversable*> next() const override;

//     std::unique_ptr<NavigationStrategy> createNavigationStrategy(
//         Vehicle* vehicle,
//         const Traffic* traffic,
//         const GeometryManager* geometry
//         ) const override;
// };
