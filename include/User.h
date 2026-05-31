#pragma once
#include <string>
#include <vector>
#include <map>
#include "WorkoutPlan.h"
#include "WorkoutSession.h"

class User {
private:
    std::string name;
    std::vector<WorkoutPlan> workoutPlans;
    std::vector<WorkoutSession> sessionHistory;
    std::map<std::string, double> personalRecords; // exercise -> best volume per set
public:
    explicit User(const std::string& name);

    WorkoutPlan& createPlan(const std::string& planName);
    void startSession(const std::string& planName, const std::string& date);
    void addSetToLastSession(const std::string& exerciseName, double weight, int reps);

    const std::string& getName() const;
    std::vector<WorkoutPlan>& getPlans();
    const std::vector<WorkoutSession>& getHistory() const;
    WorkoutSession* getLastSession();
    const std::map<std::string, double>& getPersonalRecords() const;

    bool updatePersonalRecord(const std::string& exerciseName, double weight, int reps);
    void displayPersonalRecords() const;
    void displayHistory() const;
    void displayPlans() const;
};
