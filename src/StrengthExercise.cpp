#include "StrengthExercise.h"
#include <iostream>
using namespace std;

StrengthExercise::StrengthExercise(const string& name, const string& muscleGroup,
                                   double weight, int sets, int reps)
    : Exercise(name, muscleGroup), weight(weight), sets(sets), reps(reps) {}

void StrengthExercise::perform() const {
    cout << "  [Strength] " << getName() << " | " << sets << "x" << reps
         << " @ " << weight << "kg  (muscle: " << getMuscleGroup() << ")\n";
}

string StrengthExercise::getType() const { return "Strength"; }
Exercise* StrengthExercise::clone() const { return new StrengthExercise(*this); }
double StrengthExercise::getWeight() const { return weight; }
int StrengthExercise::getSets() const { return sets; }
int StrengthExercise::getReps() const { return reps; }

string StrengthExercise::toString() const {
    return "[Strength] " + getName() + " | " + to_string(sets) + "x" +
           to_string(reps) + " @ " + to_string(weight) + "kg | " + getMuscleGroup();
}
