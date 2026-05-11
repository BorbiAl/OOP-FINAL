#include "SetLog.h"

SetLog::SetLog(const std::string& exerciseName, double weight, int reps)
    : exerciseName(exerciseName), weight(weight), reps(reps) {}

const std::string& SetLog::getExerciseName() const { return exerciseName; }
double SetLog::getWeight() const { return weight; }
int SetLog::getReps() const { return reps; }
double SetLog::volume() const { return weight * reps; }

std::string SetLog::toString() const {
    return exerciseName + ": " + std::to_string(reps) + " reps @ " + std::to_string(weight) + "kg";
}
