#include "WorkoutSession.h"
#include <iostream>

WorkoutSession::WorkoutSession(const std::string& date, const std::string& planName)
    : date(date), planName(planName) {}

void WorkoutSession::addSet(const std::string& exerciseName, double weight, int reps) {
    logs.emplace_back(exerciseName, weight, reps);
}

const std::string& WorkoutSession::getDate() const { return date; }
const std::string& WorkoutSession::getPlanName() const { return planName; }
const std::vector<SetLog>& WorkoutSession::getLogs() const { return logs; }

double WorkoutSession::totalVolume() const {
    double total = 0;
    for (const auto& log : logs) total += log.volume();
    return total;
}

void WorkoutSession::display() const {
    std::cout << "Session [" << date << "] - Plan: " << planName << "\n";
    for (const auto& log : logs) {
        std::cout << "  " << log.toString() << "  (vol: " << log.volume() << ")\n";
    }
    std::cout << "  Total volume: " << totalVolume() << " kg\n";
}
