#pragma once
#include "Exercise.h"

class StrengthExercise : public Exercise {
private:
    double weight;
    int sets;
    int reps;
public:
    StrengthExercise(const std::string& name, const std::string& muscleGroup,
                     double weight, int sets, int reps);
    void perform() const override;
    std::string getType() const override;
    double getWeight() const;
    int getSets() const;
    int getReps() const;
    std::string toString() const override;
};
