#pragma once

#include <cstddef> // For size_t

// Forward declarations
class Lane;
class Roadway;

/**
 * @brief A stateless utility class for Roadway-related queries and operations.
 */
class RoadwayUtils
{
public:
    RoadwayUtils() = delete;

    /**
     * @brief Finds the numerical index of a Lane within its parent Roadway.
     * @param lane The lane to find.
     * @param roadway The parent roadway containing the lane.
     * @return The zero-based index of the lane.
     * @throws std::runtime_error if the lane is not found in the roadway.
     */
    static size_t getLaneIndex(const Lane* lane, const Roadway* roadway);
};
