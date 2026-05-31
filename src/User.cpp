#include "User.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <sstream>

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

void User::saveToFile(const std::string& filename) const {
    std::ofstream f(filename);
    if (!f) { std::cout << "Could not open file for saving.\n"; return; }

    f << "USER " << name << "\n";

    f << "PLANS " << workoutPlans.size() << "\n";
    for (const auto& p : workoutPlans)
        f << p.getName() << "\n";

    f << "SESSIONS " << sessionHistory.size() << "\n";
    for (const auto& s : sessionHistory) {
        const auto& logs = s.getLogs();
        f << s.getDate() << " " << s.getPlanName() << " " << logs.size() << "\n";
        for (const auto& log : logs)
            f << log.getExerciseName() << " " << log.getWeight() << " " << log.getReps() << "\n";
    }

    f << "RECORDS " << personalRecords.size() << "\n";
    for (const auto& [ex, vol] : personalRecords)
        f << ex << " " << vol << "\n";

    f << "GOALS " << goals.size() << "\n";
    for (const auto& g : goals)
        f << g.getDescription() << "|" << g.getExerciseName() << "|" << g.getTargetValue() << "|" << (g.isAchieved() ? 1 : 0) << "\n";

    std::cout << "Data saved to " << filename << "\n";
}

void User::loadFromFile(const std::string& filename) {
    std::ifstream f(filename);
    if (!f || f.peek() == std::ifstream::traits_type::eof()) return;

    std::string token;
    f >> token; // USER
    std::getline(f, name); // rest of line = name (with leading space)
    if (!name.empty() && name[0] == ' ') name = name.substr(1);

    int planCount = 0;
    f >> token >> planCount; // PLANS n
    f.ignore();
    for (int i = 0; i < planCount; ++i) {
        std::string pname;
        std::getline(f, pname);
        workoutPlans.emplace_back(pname);
    }

    int sessionCount = 0;
    f >> token >> sessionCount; // SESSIONS n
    f.ignore();
    for (int i = 0; i < sessionCount; ++i) {
        std::string date, planName;
        int logCount = 0;
        f >> date >> planName >> logCount;
        f.ignore();
        sessionHistory.emplace_back(date, planName);
        for (int j = 0; j < logCount; ++j) {
            std::string exName;
            double weight; int reps;
            f >> exName >> weight >> reps;
            f.ignore();
            sessionHistory.back().addSet(exName, weight, reps);
            updatePersonalRecord(exName, weight, reps);
        }
    }

    int recCount = 0;
    f >> token >> recCount; // RECORDS n
    f.ignore();
    for (int i = 0; i < recCount; ++i) {
        std::string line;
        std::getline(f, line);
        auto sp = line.rfind(' ');
        if (sp != std::string::npos)
            personalRecords[line.substr(0, sp)] = std::stod(line.substr(sp + 1));
    }

    int goalCount = 0;
    f >> token >> goalCount; // GOALS n
    f.ignore();
    for (int i = 0; i < goalCount; ++i) {
        std::string line;
        std::getline(f, line);
        std::istringstream ss(line);
        std::string desc, exName, targetStr, achStr;
        std::getline(ss, desc,     '|');
        std::getline(ss, exName,   '|');
        std::getline(ss, targetStr,'|');
        std::getline(ss, achStr,   '|');
        Goal g(desc, exName, std::stod(targetStr));
        if (achStr == "1") g.markAchieved();
        goals.push_back(g);
    }
}
