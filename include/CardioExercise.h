#pragma once
#include "Exercise.h"

class CardioExercise : public Exercise {
private:
    int durationMinutes;
    double distanceKm;
public:
    CardioExercise(const std::string& name, const std::string& muscleGroup,
                   int durationMinutes, double distanceKm = 0.0);
    void perform() const override;
    std::string getType() const override;
    int getDuration() const;
    double getDistance() const;
    std::string toString() const override;
};
