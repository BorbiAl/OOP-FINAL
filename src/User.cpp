#include "User.h"
#include "StrengthExercise.h"
#include "CardioExercise.h"
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

    // Each plan: name + exercise count, then one line per exercise
    // Strength line: S|name|muscle|weight|sets|reps
    // Cardio line:   C|name|muscle|duration|distance
    f << "PLANS " << workoutPlans.size() << "\n";
    for (const auto& p : workoutPlans) {
        const auto& exs = p.getExercises();
        f << p.getName() << "|" << exs.size() << "\n";
        for (const auto* e : exs) {
            if (e->getType() == "Strength") {
                const auto* s = static_cast<const StrengthExercise*>(e);
                f << "S|" << s->getName() << "|" << s->getMuscleGroup()
                  << "|" << s->getWeight() << "|" << s->getSets() << "|" << s->getReps() << "\n";
            } else {
                const auto* c = static_cast<const CardioExercise*>(e);
                f << "C|" << c->getName() << "|" << c->getMuscleGroup()
                  << "|" << c->getDuration() << "|" << c->getDistance() << "\n";
            }
        }
    }

    f << "SESSIONS " << sessionHistory.size() << "\n";
    for (const auto& s : sessionHistory) {
        const auto& logs = s.getLogs();
        f << s.getDate() << "|" << s.getPlanName() << "|" << logs.size() << "\n";
        for (const auto& log : logs)
            f << log.getExerciseName() << "|" << log.getWeight() << "|" << log.getReps() << "\n";
    }

    f << "RECORDS " << personalRecords.size() << "\n";
    for (const auto& [ex, vol] : personalRecords)
        f << ex << "|" << vol << "\n";

    f << "GOALS " << goals.size() << "\n";
    for (const auto& g : goals)
        f << g.getDescription() << "|" << g.getExerciseName() << "|"
          << g.getTargetValue() << "|" << (g.isAchieved() ? 1 : 0) << "\n";

    std::cout << "Data saved to " << filename << "\n";
}

static std::vector<std::string> splitPipe(const std::string& line) {
    std::vector<std::string> parts;
    std::istringstream ss(line);
    std::string tok;
    while (std::getline(ss, tok, '|')) parts.push_back(tok);
    return parts;
}

void User::loadFromFile(const std::string& filename) {
    std::ifstream f(filename);
    if (!f || f.peek() == std::ifstream::traits_type::eof()) return;

    std::string token, line;
    f >> token; // USER
    std::getline(f, name);
    if (!name.empty() && name[0] == ' ') name = name.substr(1);

    int planCount = 0;
    f >> token >> planCount;
    f.ignore();
    for (int i = 0; i < planCount; ++i) {
        std::getline(f, line);
        auto sep = line.rfind('|');
        std::string pname = line.substr(0, sep);
        int exCount = std::stoi(line.substr(sep + 1));
        workoutPlans.emplace_back(pname);
        for (int j = 0; j < exCount; ++j) {
            std::getline(f, line);
            auto p = splitPipe(line);
            if (p[0] == "S") {
                workoutPlans.back().addExercise(
                    new StrengthExercise(p[1], p[2], std::stod(p[3]), std::stoi(p[4]), std::stoi(p[5])));
            } else {
                workoutPlans.back().addExercise(
                    new CardioExercise(p[1], p[2], std::stoi(p[3]), std::stod(p[4])));
            }
        }
    }

    int sessionCount = 0;
    f >> token >> sessionCount;
    f.ignore();
    for (int i = 0; i < sessionCount; ++i) {
        std::getline(f, line);
        auto p = splitPipe(line);
        int logCount = std::stoi(p[2]);
        sessionHistory.emplace_back(p[0], p[1]);
        for (int j = 0; j < logCount; ++j) {
            std::getline(f, line);
            auto lp = splitPipe(line);
            sessionHistory.back().addSet(lp[0], std::stod(lp[1]), std::stoi(lp[2]));
            updatePersonalRecord(lp[0], std::stod(lp[1]), std::stoi(lp[2]));
        }
    }

    int recCount = 0;
    f >> token >> recCount;
    f.ignore();
    for (int i = 0; i < recCount; ++i) {
        std::getline(f, line);
        auto p = splitPipe(line);
        if (p.size() == 2) personalRecords[p[0]] = std::stod(p[1]);
    }

    int goalCount = 0;
    f >> token >> goalCount;
    f.ignore();
    for (int i = 0; i < goalCount; ++i) {
        std::getline(f, line);
        auto p = splitPipe(line);
        if (p.size() < 4) continue;
        Goal g(p[0], p[1], std::stod(p[2]));
        if (p[3] == "1") g.markAchieved();
        goals.push_back(g);
    }
}
