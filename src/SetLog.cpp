#include "SetLog.h"
using namespace std;

SetLog::SetLog(const string& exerciseName, double weight, int reps)
    : exerciseName(exerciseName), weight(weight), reps(reps) {}

const string& SetLog::getExerciseName() const { return exerciseName; }
double SetLog::getWeight() const { return weight; }
int SetLog::getReps() const { return reps; }
double SetLog::volume() const { return weight * reps; }

string SetLog::toString() const {
    return exerciseName + ": " + to_string(reps) + " reps @ " + to_string(weight) + "kg";
}
