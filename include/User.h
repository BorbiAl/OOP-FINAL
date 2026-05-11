#pragma once
#include <string>
#include <vector>
#include "WorkoutPlan.h"
#include "WorkoutSession.h"

class User {
private:
    std::string name;
    std::vector<WorkoutPlan> workoutPlans;
    std::vector<WorkoutSession> sessionHistory;
public:
    explicit User(const std::string& name);

    WorkoutPlan& createPlan(const std::string& planName);
    void startSession(const std::string& planName, const std::string& date);
    void addSetToLastSession(const std::string& exerciseName, double weight, int reps);

    const std::string& getName() const;
    std::vector<WorkoutPlan>& getPlans();
    const std::vector<WorkoutSession>& getHistory() const;
    WorkoutSession* getLastSession();

    void displayHistory() const;
    void displayPlans() const;
};
