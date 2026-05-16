#pragma once
#include <string>
#include <vector>
#include "Exercise.h"

class WorkoutPlan {
private:
    std::string name;
    std::vector<Exercise*> exercises;
public:
    WorkoutPlan(const std::string& name);
    ~WorkoutPlan();
    WorkoutPlan(const WorkoutPlan& other);
    WorkoutPlan& operator=(const WorkoutPlan& other);
    WorkoutPlan(WorkoutPlan&& other) noexcept;

    void addExercise(Exercise* exercise);
    const std::string& getName() const;
    const std::vector<Exercise*>& getExercises() const;
    void display() const;
};
