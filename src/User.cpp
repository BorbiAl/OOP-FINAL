#include "User.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

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
    if (updatePersonalRecord(exerciseName, weight, reps)) {
        std::cout << "  ** New personal record for " << exerciseName << "! **\n";
        checkGoals();
    }
}

const std::string& User::getName() const { return name; }
std::vector<WorkoutPlan>& User::getPlans() { return workoutPlans; }
const std::vector<WorkoutSession>& User::getHistory() const { return sessionHistory; }

WorkoutSession* User::getLastSession() {
    if (sessionHistory.empty()) return nullptr;
    return &sessionHistory.back();
}

const std::map<std::string, double>& User::getPersonalRecords() const { return personalRecords; }

bool User::updatePersonalRecord(const std::string& exerciseName, double weight, int reps) {
    double vol = weight * reps;
    auto it = personalRecords.find(exerciseName);
    if (it == personalRecords.end() || vol > it->second) {
        personalRecords[exerciseName] = vol;
        return true;
    }
    return false;
}

void User::displayPersonalRecords() const {
    if (personalRecords.empty()) { std::cout << "No personal records yet.\n"; return; }
    std::cout << "=== Personal Records ===\n";
    for (const auto& [ex, vol] : personalRecords)
        std::cout << "  " << ex << ": best volume " << std::fixed << std::setprecision(1) << vol << " kg\n";
}

void User::addGoal(const std::string& description, const std::string& exerciseName, double targetVolume) {
    goals.emplace_back(description, exerciseName, targetVolume);
}

void User::checkGoals() {
    for (auto& goal : goals) {
        if (goal.isAchieved()) continue;
        auto it = personalRecords.find(goal.getExerciseName());
        if (it != personalRecords.end() && it->second >= goal.getTargetValue()) {
            goal.markAchieved();
            std::cout << "  ** Goal achieved: " << goal.getDescription() << " **\n";
        }
    }
}

void User::displayGoals() const {
    if (goals.empty()) { std::cout << "No goals set.\n"; return; }
    std::cout << "=== Goals ===\n";
    for (const auto& g : goals)
        std::cout << "  " << g.toString() << "\n";
}

void User::displayHistory() const {
    if (sessionHistory.empty()) { std::cout << "No sessions recorded.\n"; return; }
    for (const auto& s : sessionHistory) s.display();
}

void User::displayPlans() const {
    if (workoutPlans.empty()) { std::cout << "No plans created.\n"; return; }
    for (const auto& p : workoutPlans) p.display();
}
