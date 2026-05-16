#include "Exercise.h"

Exercise::Exercise(const std::string& name, const std::string& muscleGroup)
    : name(name), muscleGroup(muscleGroup) {}


const std::string& Exercise::getName() const { return name; }
const std::string& Exercise::getMuscleGroup() const { return muscleGroup; }

std::string Exercise::toString() const {
    return name + " [" + muscleGroup + "]";
}
