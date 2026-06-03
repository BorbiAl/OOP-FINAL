#include "WorkoutSession.h"
#include <iostream>
using namespace std;

WorkoutSession::WorkoutSession(const string& date, const string& planName)
    : date(date), planName(planName) {}

void WorkoutSession::addSet(const string& exerciseName, double weight, int reps) {
    logs.emplace_back(exerciseName, weight, reps);
}

const string& WorkoutSession::getDate() const { return date; }
const string& WorkoutSession::getPlanName() const { return planName; }
const vector<SetLog>& WorkoutSession::getLogs() const { return logs; }

double WorkoutSession::totalVolume() const {
    double total = 0;
    for (const auto& log : logs) total += log.volume();
    return total;
}

void WorkoutSession::display() const {
    cout << "Session [" << date << "] - Plan: " << planName << "\n";
    for (const auto& log : logs)
        cout << "  " << log.toString() << "  (vol: " << log.volume() << ")\n";
    cout << "  Total volume: " << totalVolume() << " kg\n";
}
