#pragma once
#include <vector>
#include <string>
#include "WorkoutSession.h"

class ProgressAnalyzer {
public:
    // Returns average volume per session for a given exercise across all sessions.
    // Prints a per-session trend showing improvement or decline.
    void analyzeProgress(const std::vector<WorkoutSession>& history,
                         const std::string& exerciseName) const;
};
