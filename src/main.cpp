#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "User.h"
#include "StrengthExercise.h"
#include "CardioExercise.h"
#include "ProgressAnalyzer.h"
#include "PlateauDetector.h"
#include "RecommendationEngine.h"
#include "LoadTracker.h"
using namespace std;

static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string getDate() {
    string d;
    cout << "Enter date (YYYY-MM-DD): ";
    cin >> d;
    return d;
}

static void menuCreatePlan(User& user) {
    clearInput();
    string planName;
    cout << "Plan name: ";
    getline(cin, planName);
    user.createPlan(planName);
    cout << "Plan '" << planName << "' created.\n";
}

static void menuAddExercise(User& user) {
    auto& plans = user.getPlans();
    if (plans.empty()) { cout << "No plans. Create one first.\n"; return; }

    cout << "Select plan:\n";
    for (int i = 0; i < (int)plans.size(); ++i)
        cout << "  " << i + 1 << ". " << plans[i].getName() << "\n";
    int idx; cin >> idx; idx--;
    if (idx < 0 || idx >= (int)plans.size()) { cout << "Invalid.\n"; return; }

    clearInput();
    cout << "Exercise type: 1) Strength  2) Cardio\nChoice: ";
    int type; cin >> type;
    clearInput();

    string eName, muscle;
    cout << "Exercise name: "; getline(cin, eName);
    cout << "Muscle group: ";  getline(cin, muscle);

    if (type == 1) {
        double w; int s, r;
        cout << "Weight (kg): "; cin >> w;
        cout << "Sets: ";        cin >> s;
        cout << "Reps: ";        cin >> r;
        plans[idx].addExercise(new StrengthExercise(eName, muscle, w, s, r));
    } else {
        int dur; double dist;
        cout << "Duration (min): ";           cin >> dur;
        cout << "Distance (km, 0 if N/A): ";  cin >> dist;
        plans[idx].addExercise(new CardioExercise(eName, muscle, dur, dist));
    }
    cout << "Exercise added.\n";
}

static void menuStartSession(User& user) {
    auto& plans = user.getPlans();
    if (plans.empty()) { cout << "No plans available.\n"; return; }

    cout << "Select plan for session:\n";
    for (int i = 0; i < (int)plans.size(); ++i)
        cout << "  " << i + 1 << ". " << plans[i].getName() << "\n";
    int idx; cin >> idx; idx--;
    if (idx < 0 || idx >= (int)plans.size()) { cout << "Invalid.\n"; return; }

    string date = getDate();
    user.startSession(plans[idx].getName(), date);

    cout << "Session started. Plan: " << plans[idx].getName() << "\n";
    cout << "Exercises in this plan:\n";
    plans[idx].display();

    char more = 'y';
    while (more == 'y' || more == 'Y') {
        clearInput();
        string exName;
        double weight; int reps;
        cout << "Exercise name: "; getline(cin, exName);
        cout << "Weight (kg): ";   cin >> weight;
        cout << "Reps: ";          cin >> reps;
        try {
            user.addSetToLastSession(exName, weight, reps);
            cout << "Set logged.\n";
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << "\n";
        }
        cout << "Log another set? (y/n): "; cin >> more;
    }
    cout << "Session saved.\n";
}

static void menuAnalyzeProgress(const User& user) {
    if (user.getHistory().empty()) { cout << "No sessions recorded.\n"; return; }
    clearInput();
    string exName;
    cout << "Exercise name to analyze: ";
    getline(cin, exName);
    ProgressAnalyzer analyzer;
    analyzer.analyzeProgress(user.getHistory(), exName);
}

static void menuDetectPlateau(const User& user) {
    if (user.getHistory().empty()) { cout << "No sessions recorded.\n"; return; }
    clearInput();
    string exName;
    cout << "Exercise name to check: ";
    getline(cin, exName);
    PlateauDetector detector;
    detector.detectPlateau(user.getHistory(), exName);
}

static void menuAddGoal(User& user) {
    clearInput();
    string desc, exName;
    double target;
    cout << "Goal description: ";  getline(cin, desc);
    cout << "Exercise name: ";     getline(cin, exName);
    cout << "Target volume (weight x reps, e.g. 100): "; cin >> target;
    user.addGoal(desc, exName, target);
    cout << "Goal added.\n";
}

static void menuRecommend(const User& user) {
    if (user.getHistory().empty()) { cout << "No sessions recorded.\n"; return; }
    clearInput();
    string exName;
    cout << "Exercise name: ";
    getline(cin, exName);
    RecommendationEngine engine;
    engine.generateRecommendation(user.getHistory(), exName);
}

static void menuLoadTracker(const User& user) {
    if (user.getHistory().empty()) { cout << "No sessions recorded.\n"; return; }
    LoadTracker tracker;
    tracker.analyzeLoad(user.getHistory());
}

static void printMenu() {
    cout << "\n====== FITNESS TRACKER ======\n"
         << " 1. Create workout plan\n"
         << " 2. Add exercise to plan\n"
         << " 3. View all plans\n"
         << " 4. Start training session\n"
         << " 5. View session history\n"
         << " 6. View personal records\n"
         << " 7. Analyze exercise progress\n"
         << " 8. Check for plateau\n"
         << " 9. Get recommendations\n"
         << "10. Add goal\n"
         << "11. View goals\n"
         << "12. Analyze training load\n"
         << " 0. Exit\n"
         << "Choice: ";
}

static const string DATA_FILE = "data.txt";

int main() {
    string userName;
    cout << "Enter your name: ";
    getline(cin, userName);
    User user(userName);
    user.loadFromFile(DATA_FILE);
    cout << "Welcome, " << user.getName() << "!\n";

    int choice = -1;
    while (choice != 0) {
        printMenu();
        cin >> choice;
        try {
            switch (choice) {
                case 1:  menuCreatePlan(user);          break;
                case 2:  menuAddExercise(user);         break;
                case 3:  user.displayPlans();           break;
                case 4:  menuStartSession(user);        break;
                case 5:  user.displayHistory();         break;
                case 6:  user.displayPersonalRecords(); break;
                case 7:  menuAnalyzeProgress(user);     break;
                case 8:  menuDetectPlateau(user);       break;
                case 9:  menuRecommend(user);           break;
                case 10: menuAddGoal(user);             break;
                case 11: user.displayGoals();           break;
                case 12: menuLoadTracker(user);         break;
                case 0:
                    user.saveToFile(DATA_FILE);
                    cout << "Goodbye!\n";
                    break;
                default: cout << "Invalid option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
