#pragma once
#include <vector>
#include <string>
#include "WorkoutSession.h"

class RecommendationEngine {
public:
    // Generates training recommendations for a given exercise based on session history.
    void generateRecommendation(const std::vector<WorkoutSession>& history,
                                const std::string& exerciseName) const;
};
