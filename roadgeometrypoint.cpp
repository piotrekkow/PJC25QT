#include "roadgeometrypoint.h"

RoadGeometryPoint::RoadGeometryPoint(QPointF position, qreal medianWidth, qreal lateralMedianOffset)
    : position_{ position }
    , medianWidth_{ medianWidth }
    , medianOffset_{ lateralMedianOffset }
{

}
