#include "Goal.h"

Goal::Goal(const std::string& description, const std::string& exerciseName, double targetValue)
    : description(description), exerciseName(exerciseName), targetValue(targetValue), achieved(false) {}

const std::string& Goal::getDescription() const  { return description; }
const std::string& Goal::getExerciseName() const  { return exerciseName; }
double Goal::getTargetValue() const               { return targetValue; }
bool Goal::isAchieved() const                     { return achieved; }
void Goal::markAchieved()                         { achieved = true; }

std::string Goal::toString() const {
    std::string status = achieved ? "[DONE]" : "[    ]";
    return status + " " + description + " (" + exerciseName +
           " - target volume: " + std::to_string(targetValue) + " kg)";
}
