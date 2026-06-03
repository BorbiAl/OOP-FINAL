#include "Exercise.h"
using namespace std;

Exercise::Exercise(const string& name, const string& muscleGroup)
    : name(name), muscleGroup(muscleGroup) {}

const string& Exercise::getName() const { return name; }
const string& Exercise::getMuscleGroup() const { return muscleGroup; }

string Exercise::toString() const {
    return name + " [" + muscleGroup + "]";
}
