#pragma once
#include <string>

class Goal {
private:
    std::string description;
    std::string exerciseName;
    double targetValue; // target volume (weight * reps) per set
    bool achieved;
public:
    Goal(const std::string& description, const std::string& exerciseName, double targetValue);
    const std::string& getDescription() const;
    const std::string& getExerciseName() const;
    double getTargetValue() const;
    bool isAchieved() const;
    void markAchieved();
    std::string toString() const;
};
