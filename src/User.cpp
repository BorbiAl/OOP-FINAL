#include "User.h"
#include "StrengthExercise.h"
#include "CardioExercise.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

User::User(const string& name) : name(name) {}

WorkoutPlan& User::createPlan(const string& planName) {
    workoutPlans.emplace_back(planName);
    return workoutPlans.back();
}

void User::startSession(const string& planName, const string& date) {
    sessionHistory.emplace_back(date, planName);
}

void User::addSetToLastSession(const string& exerciseName, double weight, int reps) {
    if (sessionHistory.empty()) throw runtime_error("No active session.");
    sessionHistory.back().addSet(exerciseName, weight, reps);
    if (updatePersonalRecord(exerciseName, weight, reps)) {
        cout << "  ** New personal record for " << exerciseName << "! **\n";
        checkGoals();
    }
}

const string& User::getName() const { return name; }
vector<WorkoutPlan>& User::getPlans() { return workoutPlans; }
const vector<WorkoutSession>& User::getHistory() const { return sessionHistory; }

WorkoutSession* User::getLastSession() {
    if (sessionHistory.empty()) return nullptr;
    return &sessionHistory.back();
}

const map<string, double>& User::getPersonalRecords() const { return personalRecords; }

bool User::updatePersonalRecord(const string& exerciseName, double weight, int reps) {
    double vol = weight * reps;
    auto it = personalRecords.find(exerciseName);
    if (it == personalRecords.end() || vol > it->second) {
        personalRecords[exerciseName] = vol;
        return true;
    }
    return false;
}

void User::displayPersonalRecords() const {
    if (personalRecords.empty()) { cout << "No personal records yet.\n"; return; }
    cout << "=== Personal Records ===\n";
    for (const auto& [ex, vol] : personalRecords)
        cout << "  " << ex << ": best volume " << fixed << setprecision(1) << vol << " kg\n";
}

void User::addGoal(const string& description, const string& exerciseName, double targetVolume) {
    goals.emplace_back(description, exerciseName, targetVolume);
}

void User::checkGoals() {
    for (auto& goal : goals) {
        if (goal.isAchieved()) continue;
        auto it = personalRecords.find(goal.getExerciseName());
        if (it != personalRecords.end() && it->second >= goal.getTargetValue()) {
            goal.markAchieved();
            cout << "  ** Goal achieved: " << goal.getDescription() << " **\n";
        }
    }
}

void User::displayGoals() const {
    if (goals.empty()) { cout << "No goals set.\n"; return; }
    cout << "=== Goals ===\n";
    for (const auto& g : goals)
        cout << "  " << g.toString() << "\n";
}

void User::displayHistory() const {
    if (sessionHistory.empty()) { cout << "No sessions recorded.\n"; return; }
    for (const auto& s : sessionHistory) s.display();
}

void User::displayPlans() const {
    if (workoutPlans.empty()) { cout << "No plans created.\n"; return; }
    for (const auto& p : workoutPlans) p.display();
}

void User::saveToFile(const string& filename) const {
    ofstream f(filename);
    if (!f) { cout << "Could not open file for saving.\n"; return; }

    f << "USER " << name << "\n";

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

    cout << "Data saved to " << filename << "\n";
}

static vector<string> splitPipe(const string& line) {
    vector<string> parts;
    istringstream ss(line);
    string tok;
    while (getline(ss, tok, '|')) parts.push_back(tok);
    return parts;
}

void User::loadFromFile(const string& filename) {
    ifstream f(filename);
    if (!f || f.peek() == ifstream::traits_type::eof()) return;

    string token, line;
    f >> token;
    getline(f, name);
    if (!name.empty() && name[0] == ' ') name = name.substr(1);

    int planCount = 0;
    f >> token >> planCount;
    f.ignore();
    for (int i = 0; i < planCount; ++i) {
        getline(f, line);
        auto sep = line.rfind('|');
        string pname = line.substr(0, sep);
        int exCount = stoi(line.substr(sep + 1));
        workoutPlans.emplace_back(pname);
        for (int j = 0; j < exCount; ++j) {
            getline(f, line);
            auto p = splitPipe(line);
            if (p[0] == "S") {
                workoutPlans.back().addExercise(
                    new StrengthExercise(p[1], p[2], stod(p[3]), stoi(p[4]), stoi(p[5])));
            } else {
                workoutPlans.back().addExercise(
                    new CardioExercise(p[1], p[2], stoi(p[3]), stod(p[4])));
            }
        }
    }

    int sessionCount = 0;
    f >> token >> sessionCount;
    f.ignore();
    for (int i = 0; i < sessionCount; ++i) {
        getline(f, line);
        auto p = splitPipe(line);
        int logCount = stoi(p[2]);
        sessionHistory.emplace_back(p[0], p[1]);
        for (int j = 0; j < logCount; ++j) {
            getline(f, line);
            auto lp = splitPipe(line);
            sessionHistory.back().addSet(lp[0], stod(lp[1]), stoi(lp[2]));
            updatePersonalRecord(lp[0], stod(lp[1]), stoi(lp[2]));
        }
    }

    int recCount = 0;
    f >> token >> recCount;
    f.ignore();
    for (int i = 0; i < recCount; ++i) {
        getline(f, line);
        auto p = splitPipe(line);
        if (p.size() == 2) personalRecords[p[0]] = stod(p[1]);
    }

    int goalCount = 0;
    f >> token >> goalCount;
    f.ignore();
    for (int i = 0; i < goalCount; ++i) {
        getline(f, line);
        auto p = splitPipe(line);
        if (p.size() < 4) continue;
        Goal g(p[0], p[1], stod(p[2]));
        if (p[3] == "1") g.markAchieved();
        goals.push_back(g);
    }
}
