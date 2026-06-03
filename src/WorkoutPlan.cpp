#include "WorkoutPlan.h"
#include <iostream>
using namespace std;

WorkoutPlan::WorkoutPlan(const string& name) : name(name) {}

WorkoutPlan::~WorkoutPlan() {
    for (auto* e : exercises) delete e;
}

WorkoutPlan::WorkoutPlan(const WorkoutPlan& other) : name(other.name) {
    for (const auto* e : other.exercises)
        exercises.push_back(e->clone());
}

WorkoutPlan& WorkoutPlan::operator=(const WorkoutPlan& other) {
    if (this == &other)
        return *this;
    for (auto* e : exercises) delete e;
    exercises.clear();
    name = other.name;
    for (const auto* e : other.exercises)
        exercises.push_back(e->clone());
    return *this;
}

WorkoutPlan::WorkoutPlan(WorkoutPlan&& other) noexcept
    : name(move(other.name)), exercises(move(other.exercises)) {
    other.exercises.clear();
}

void WorkoutPlan::addExercise(Exercise* exercise) {
    exercises.push_back(exercise);
}

const string& WorkoutPlan::getName() const { return name; }
const vector<Exercise*>& WorkoutPlan::getExercises() const { return exercises; }

void WorkoutPlan::display() const {
    cout << "Plan: " << name << "\n";
    if (exercises.empty()) {
        cout << "  (no exercises)\n";
        return;
    }
    for (const auto* e : exercises)
        cout << "  " << e->toString() << "\n";
}
