#include "CardioExercise.h"
#include <iostream>
using namespace std;

CardioExercise::CardioExercise(const string& name, const string& muscleGroup,
                               int durationMinutes, double distanceKm)
    : Exercise(name, muscleGroup), durationMinutes(durationMinutes), distanceKm(distanceKm) {}

void CardioExercise::perform() const {
    cout << "  [Cardio] " << getName() << " | " << durationMinutes << " min";
    if (distanceKm > 0) cout << " / " << distanceKm << " km";
    cout << "  (muscle: " << getMuscleGroup() << ")\n";
}

string CardioExercise::getType() const { return "Cardio"; }
Exercise* CardioExercise::clone() const { return new CardioExercise(*this); }
int CardioExercise::getDuration() const { return durationMinutes; }
double CardioExercise::getDistance() const { return distanceKm; }

string CardioExercise::toString() const {
    string s = "[Cardio] " + getName() + " | " + to_string(durationMinutes) + " min";
    if (distanceKm > 0) s += " / " + to_string(distanceKm) + " km";
    s += " | " + getMuscleGroup();
    return s;
}
