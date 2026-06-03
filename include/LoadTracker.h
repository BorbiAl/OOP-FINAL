#pragma once
#include <vector>
#include "WorkoutSession.h"

class LoadTracker {
public:
    void analyzeLoad(const std::vector<WorkoutSession>& history, int windowSize = 4) const;
};
