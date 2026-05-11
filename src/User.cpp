#include "User.h"
#include <iostream>
#include <stdexcept>

User::User(const std::string& name) : name(name) {}

WorkoutPlan& User::createPlan(const std::string& planName) {
    workoutPlans.emplace_back(planName);
    return workoutPlans.back();
}

void User::startSession(const std::string& planName, const std::string& date) {
    sessionHistory.emplace_back(date, planName);
}

void User::addSetToLastSession(const std::string& exerciseName, double weight, int reps) {
    if (sessionHistory.empty()) throw std::runtime_error("No active session.");
    sessionHistory.back().addSet(exerciseName, weight, reps);
}

const std::string& User::getName() const { return name; }
std::vector<WorkoutPlan>& User::getPlans() { return workoutPlans; }
const std::vector<WorkoutSession>& User::getHistory() const { return sessionHistory; }

WorkoutSession* User::getLastSession() {
    if (sessionHistory.empty()) return nullptr;
    return &sessionHistory.back();
}

void User::displayHistory() const {
    if (sessionHistory.empty()) { std::cout << "No sessions recorded.\n"; return; }
    for (const auto& s : sessionHistory) s.display();
}

void User::displayPlans() const {
    if (workoutPlans.empty()) { std::cout << "No plans created.\n"; return; }
    for (const auto& p : workoutPlans) p.display();
}
