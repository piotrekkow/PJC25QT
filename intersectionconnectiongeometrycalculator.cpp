#include "intersectionconnectiongeometrycalculator.h"
#include "road.h"

QLineF IntersectionConnectionGeometryCalculator::calculateGeometry(const QPointF &sourceEndPoint, const IntersectionConnection *connection)
{
    if (!connection)
        std::invalid_argument("Invalid connection provided.");

    Lane* destination = connection->destination();

    if (!destination)
        std::runtime_error("Provided connection has no valid destination");

    const Roadway* destinationRoadway = destination->roadway();
    const RoadGeometry destinationRoadGeometry = destinationRoadway->road()->geometry();
    auto destinationRoadwayBaseline = RoadwayGeometryCalculator::calculateBaseline(destinationRoadGeometry, destinationRoadway);
    QPointF destinationStartPoint = LaneGeometryCalculator::calculateStartPoint(destinationRoadwayBaseline, destination, destinationRoadway);

    return { sourceEndPoint, destinationStartPoint };
}
