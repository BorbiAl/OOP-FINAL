#pragma once
#include <string>

class SetLog {
private:
    std::string exerciseName;
    double weight;
    int reps;
public:
    SetLog(const std::string& exerciseName, double weight, int reps);
    const std::string& getExerciseName() const;
    double getWeight() const;
    int getReps() const;
    double volume() const;
    std::string toString() const;
};
