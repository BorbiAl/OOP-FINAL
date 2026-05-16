#include "CardioExercise.h"
#include <iostream>

CardioExercise::CardioExercise(const std::string& name, const std::string& muscleGroup,
                               int durationMinutes, double distanceKm)
    : Exercise(name, muscleGroup), durationMinutes(durationMinutes), distanceKm(distanceKm) {}

void CardioExercise::perform() const {
    std::cout << "  [Cardio] " << getName() << " | " << durationMinutes << " min";
    if (distanceKm > 0) std::cout << " / " << distanceKm << " km";
    std::cout << "  (muscle: " << getMuscleGroup() << ")\n";
}

std::string CardioExercise::getType() const { return "Cardio"; }
Exercise* CardioExercise::clone() const { return new CardioExercise(*this); }
int CardioExercise::getDuration() const { return durationMinutes; }
double CardioExercise::getDistance() const { return distanceKm; }

std::string CardioExercise::toString() const {
    std::string s = "[Cardio] " + getName() + " | " + std::to_string(durationMinutes) + " min";
    if (distanceKm > 0) s += " / " + std::to_string(distanceKm) + " km";
    s += " | " + getMuscleGroup();
    return s;
}
