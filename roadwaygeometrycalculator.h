#pragma once

#include <vector>
#include <cstddef> // For size_t
#include <QPointF>

// Forward declarations to keep the header lightweight and reduce compile times
class Roadway;
class RoadGeometry;

struct OrientedPoint
{
    QPointF position;
    QPointF normal; // The smooth normals from the original road centerline
};

/**
 * @brief A stateless calculator for determining the baseline geometry of a Roadway.
 *
 * This class calculates the reference line for a specific roadway (e.g., northbound lanes),
 * taking into account the road's centerline and any median offsets. The resulting list of points
 * and their normals serves as the baseline from which individual lanes are then offset.
 */
class RoadwayGeometryCalculator
{
public:
    // This class is not meant to be instantiated
    RoadwayGeometryCalculator() = delete;

    /**
     * @brief Calculates the full baseline geometry for a given roadway.
     * @param roadGeometry The parent road's geometry (provides centerline and median info).
     * @param roadway The roadway for which to calculate the baseline (provides direction).
     * @return A vector of OrientedPoint structs, representing the ordered points and
     * normals that define the roadway's edge.
     */
    static std::vector<OrientedPoint> calculateBaseline(const RoadGeometry& roadGeometry, const Roadway* roadway);

private:
    /**
     * @brief Calculates a single point and its corresponding normal for the roadway baseline.
     * @param roadGeometry The parent road's geometry.
     * @param roadway The specific roadway.
     * @param pointIndex The index of the point along the road's centerline to calculate.
     * @return A OrientedPoint struct for the specified point.
     */
    static OrientedPoint calculateBaselinePoint(const RoadGeometry& roadGeometry, const Roadway* roadway, size_t pointIndex);
};
