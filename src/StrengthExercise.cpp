#include "StrengthExercise.h"
#include <iostream>

StrengthExercise::StrengthExercise(const std::string& name, const std::string& muscleGroup,
                                   double weight, int sets, int reps)
    : Exercise(name, muscleGroup), weight(weight), sets(sets), reps(reps) {}

void StrengthExercise::perform() const {
    std::cout << "  [Strength] " << getName() << " | " << sets << "x" << reps
              << " @ " << weight << "kg  (muscle: " << getMuscleGroup() << ")\n";
}

std::string StrengthExercise::getType() const { return "Strength"; }
Exercise* StrengthExercise::clone() const { return new StrengthExercise(*this); }
double StrengthExercise::getWeight() const { return weight; }
int StrengthExercise::getSets() const { return sets; }
int StrengthExercise::getReps() const { return reps; }

std::string StrengthExercise::toString() const {
    return "[Strength] " + getName() + " | " + std::to_string(sets) + "x" +
           std::to_string(reps) + " @ " + std::to_string(weight) + "kg | " + getMuscleGroup();
}
