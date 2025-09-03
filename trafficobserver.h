#pragma once

class Agent;


/**
 * @brief The Traffic class will notify any interested Observers when an agent is created or about to be destroyed. This is required due to Traffic marking and removing vehicles marked for removal in the same update() call. Decouples dynamic traffic logic from rendering logic.
 */
class TrafficObserver
{
public:
    virtual ~TrafficObserver() = default;
    virtual void onAgentAdded(const Agent* agent) = 0;
    virtual void onAgentRemoved(const Agent* agent) = 0;
};
