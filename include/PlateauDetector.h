#pragma once
#include <vector>
#include <string>
#include "WorkoutSession.h"

class PlateauDetector {
public:
    bool detectPlateau(const std::vector<WorkoutSession>& history,
                       const std::string& exerciseName,
                       int windowSize = 3) const;
};
