#pragma once
/**
 * @brief Priority type is set at the level of a roadway and used for establishing
 *        priorities of connections at the next intersection.
 */
enum class PriorityType {
    Priority,   // has right of way
    Yield,      // must yield to priority
    Stop        // must stop then yield
};
