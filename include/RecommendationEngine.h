#pragma once
#include <vector>
#include <string>
#include "WorkoutSession.h"

class RecommendationEngine
{
public:
    void generateRecommendation(const std::vector<WorkoutSession> &history,
                                const std::string &exerciseName) const;
};
