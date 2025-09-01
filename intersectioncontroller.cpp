#include "intersectioncontroller.h"
#include "ConflictData.h"
#include "intersection.h"
#include "agent.h"
#include "lane.h"
#include "DebugHelpers.h"

std::vector<ConflictData> SignController::conflictsMustYieldTo(const Connection *conn, const std::vector<std::unique_ptr<Agent> > &agents, const GeometryManager* geometry) const
{
    std::vector<ConflictData> conflictsToResolve;
    for (const auto& cp : intersection_->conflictManager()->conflicts(conn))
    {
        if (cp->classify() == ConflictPoint::ConflictType::Diverging) continue;
        if (weHavePriorityInConflict(conn, cp)) continue;

        std::vector<PriorityAgentInfo> foundAgents;


        for (const auto& agent : agents)
        {

            const auto* traversable = agent->traversable();
            const auto* priorityConn = cp->priorityConnection();

            // filter irrelevant vehicles (not on conflict connection or approaching lane)
            if (traversable != priorityConn && traversable != priorityConn->source()) continue;

            qreal distance = (traversable == priorityConn)
                                 ? cp->distanceFrom(priorityConn) - agent->progress()
                                 : priorityConn->source()->length(geometry) - agent->progress() + cp->distanceFrom(priorityConn);

            if (distance < 0) continue;
            foundAgents.emplace_back(PriorityAgentInfo{agent.get(), distance});
        }

        if (!foundAgents.empty()) conflictsToResolve.emplace_back(ConflictData{cp, foundAgents});
    }

    qDebug() << "decisionData:" << conflictsToResolve;
    return conflictsToResolve;
}

bool IntersectionController::weHavePriorityInConflict(const Connection *us, const ConflictPoint *cp) const
{
    // consider relative regulatory (sign) priority based on whether we have right-hand rule priority
    return (cp->isRHPriority(us)) ? us->regulatoryPriority() >= cp->yieldConnection()->regulatoryPriority()
                                : us->regulatoryPriority() > cp->priorityConnection()->regulatoryPriority();
}
