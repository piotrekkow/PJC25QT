#include "roadwayutils.h"
#include "roadway.h"
#include "lane.h"
#include <stdexcept>
#include <algorithm>

size_t RoadwayUtils::getLaneIndex(const Lane* lane, const Roadway* roadway)
{
    if (!roadway || !lane)
    {
        throw std::invalid_argument("Provided lane or roadway is null.");
    }

    const auto& lanes = roadway->lanes();
    auto it = std::find_if(lanes.begin(), lanes.end(),
                           [lane](const std::unique_ptr<Lane>& ptr) {
                               return ptr.get() == lane;
                           });

    if (it == lanes.end())
    {
        throw std::runtime_error("Lane not found in parent roadway.");
    }

    return static_cast<size_t>(std::distance(lanes.begin(), it));
}
