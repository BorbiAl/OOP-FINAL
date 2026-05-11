#pragma once
#include <string>
#include <vector>
#include "SetLog.h"

class WorkoutSession {
private:
    std::string date;
    std::string planName;
    std::vector<SetLog> logs;
public:
    WorkoutSession(const std::string& date, const std::string& planName);
    void addSet(const std::string& exerciseName, double weight, int reps);
    const std::string& getDate() const;
    const std::string& getPlanName() const;
    const std::vector<SetLog>& getLogs() const;
    double totalVolume() const;
    void display() const;
};
