#include "StrengthExercise.h"
#include <iostream>

StrengthExercise::StrengthExercise(const std::string& name, const std::string& muscleGroup,
                                   double weight, int sets, int reps)
    : Exercise(name, muscleGroup), weight(weight), sets(sets), reps(reps) {}

void StrengthExercise::perform() const {
    std::cout << "  [Strength] " << name << " | " << sets << "x" << reps
              << " @ " << weight << "kg  (muscle: " << muscleGroup << ")\n";
}

std::string StrengthExercise::getType() const { return "Strength"; }
double StrengthExercise::getWeight() const { return weight; }
int StrengthExercise::getSets() const { return sets; }
int StrengthExercise::getReps() const { return reps; }

std::string StrengthExercise::toString() const {
    return "[Strength] " + name + " | " + std::to_string(sets) + "x" +
           std::to_string(reps) + " @ " + std::to_string(weight) + "kg | " + muscleGroup;
}
