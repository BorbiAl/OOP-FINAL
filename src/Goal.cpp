#include "Goal.h"
using namespace std;

Goal::Goal(const string& description, const string& exerciseName, double targetValue)
    : description(description), exerciseName(exerciseName), targetValue(targetValue), achieved(false) {}

const string& Goal::getDescription() const  { return description; }
const string& Goal::getExerciseName() const  { return exerciseName; }
double Goal::getTargetValue() const          { return targetValue; }
bool Goal::isAchieved() const               { return achieved; }
void Goal::markAchieved()                   { achieved = true; }

string Goal::toString() const {
    string status = achieved ? "[DONE]" : "[    ]";
    return status + " " + description + " (" + exerciseName +
           " - target volume: " + to_string(targetValue) + " kg)";
}
