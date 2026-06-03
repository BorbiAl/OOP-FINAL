#pragma once
#include <string>
#include <vector>
#include <map>
#include "WorkoutPlan.h"
#include "WorkoutSession.h"
#include "Goal.h"
#include <fstream>

class User {
private:
    std::string name;
    std::vector<WorkoutPlan> workoutPlans;
    std::vector<WorkoutSession> sessionHistory;
    std::map<std::string, double> personalRecords;
    std::vector<Goal> goals;
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

    void addGoal(const std::string& description, const std::string& exerciseName, double targetVolume);
    void checkGoals();
    void displayGoals() const;

    void displayHistory() const;
    void displayPlans() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};
