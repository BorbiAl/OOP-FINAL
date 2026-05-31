#pragma once
#include <vector>
#include <string>
#include "WorkoutSession.h"

class PlateauDetector {
public:
    // Returns true if the last `windowSize` sessions show no improvement
    // in total volume for the given exercise. Prints a warning when detected.
    bool detectPlateau(const std::vector<WorkoutSession>& history,
                       const std::string& exerciseName,
                       int windowSize = 3) const;
};
