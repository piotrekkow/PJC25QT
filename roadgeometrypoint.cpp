#include "roadgeometrypoint.h"

RoadGeometryPoint::RoadGeometryPoint(QPointF position, float medianWidth, float lateralMedianOffset)
    : position_{ position }
    , medianWidth_{ medianWidth }
    , medianOffset_{ lateralMedianOffset }
{

}
