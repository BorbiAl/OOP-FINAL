#include "WorkoutPlan.h"
#include <iostream>

WorkoutPlan::WorkoutPlan(const std::string& name) : name(name) {}

WorkoutPlan::~WorkoutPlan() {
    for (auto* e : exercises) delete e;
}

WorkoutPlan::WorkoutPlan(WorkoutPlan&& other) noexcept
    : name(std::move(other.name)), exercises(std::move(other.exercises)) {
    other.exercises.clear();
}

void WorkoutPlan::addExercise(Exercise* exercise) {
    exercises.push_back(exercise);
}

const std::string& WorkoutPlan::getName() const { return name; }
const std::vector<Exercise*>& WorkoutPlan::getExercises() const { return exercises; }

void WorkoutPlan::display() const {
    std::cout << "Plan: " << name << "\n";
    if (exercises.empty()) {
        std::cout << "  (no exercises)\n";
        return;
    }
    for (const auto* e : exercises) {
        std::cout << "  " << e->toString() << "\n";
    }
}
