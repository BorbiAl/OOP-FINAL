#pragma once
#include <vector>
#include "WorkoutSession.h"

class LoadTracker {
public:
    // Compares total volume of the last session against the rolling average
    // of the previous `windowSize` sessions. Warns on overtraining (>50% spike)
    // or undertraining (<50% drop).
    void analyzeLoad(const std::vector<WorkoutSession>& history, int windowSize = 4) const;
};
