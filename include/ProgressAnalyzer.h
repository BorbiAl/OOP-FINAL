#pragma once
#include <vector>
#include <string>
#include "WorkoutSession.h"

class ProgressAnalyzer {
public:
     void analyzeProgress(const std::vector<WorkoutSession>& history,
                         const std::string& exerciseName) const;
};
