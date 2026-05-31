#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "User.h"
#include "StrengthExercise.h"
#include "CardioExercise.h"

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string getDate() {
    std::string d;
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::cin >> d;
    return d;
}

static void menuCreatePlan(User& user) {
    clearInput();
    std::string planName;
    std::cout << "Plan name: ";
    std::getline(std::cin, planName);
    user.createPlan(planName);
    std::cout << "Plan '" << planName << "' created.\n";
}


static void menuAddExercise(User& user) {
    auto& plans = user.getPlans();
    if (plans.empty()) { std::cout << "No plans. Create one first.\n"; return; }

    std::cout << "Select plan:\n";
    for (int i = 0; i < (int)plans.size(); ++i)
        std::cout << "  " << i + 1 << ". " << plans[i].getName() << "\n";
    int idx; std::cin >> idx; idx--;
    if (idx < 0 || idx >= (int)plans.size()) { std::cout << "Invalid.\n"; return; }

    clearInput();
    std::cout << "Exercise type: 1) Strength  2) Cardio\nChoice: ";
    int type; std::cin >> type;
    clearInput();

    std::string eName, muscle;
    std::cout << "Exercise name: "; std::getline(std::cin, eName);
    std::cout << "Muscle group: "; std::getline(std::cin, muscle);

    if (type == 1) {
        double w; int s, r;
        std::cout << "Weight (kg): "; std::cin >> w;
        std::cout << "Sets: ";        std::cin >> s;
        std::cout << "Reps: ";        std::cin >> r;
        plans[idx].addExercise(new StrengthExercise(eName, muscle, w, s, r));
    } else {
        int dur; double dist;
        std::cout << "Duration (min): ";  std::cin >> dur;
        std::cout << "Distance (km, 0 if N/A): "; std::cin >> dist;
        plans[idx].addExercise(new CardioExercise(eName, muscle, dur, dist));
    }
    std::cout << "Exercise added.\n";
}

static void menuStartSession(User& user) {
    auto& plans = user.getPlans();
    if (plans.empty()) { std::cout << "No plans available.\n"; return; }

    std::cout << "Select plan for session:\n";
    for (int i = 0; i < (int)plans.size(); ++i)
        std::cout << "  " << i + 1 << ". " << plans[i].getName() << "\n";
    int idx; std::cin >> idx; idx--;
    if (idx < 0 || idx >= (int)plans.size()) { std::cout << "Invalid.\n"; return; }

    std::string date = getDate();
    user.startSession(plans[idx].getName(), date);

    std::cout << "Session started. Plan: " << plans[idx].getName() << "\n";
    std::cout << "Exercises in this plan:\n";
    plans[idx].display();

    char more = 'y';
    while (more == 'y' || more == 'Y') {
        clearInput();
        std::string exName;
        double weight; int reps;
        std::cout << "Exercise name: "; std::getline(std::cin, exName);
        std::cout << "Weight (kg): ";   std::cin >> weight;
        std::cout << "Reps: ";          std::cin >> reps;
        try {
            user.addSetToLastSession(exName, weight, reps);
            std::cout << "Set logged.\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
        std::cout << "Log another set? (y/n): "; std::cin >> more;
    }
    std::cout << "Session saved.\n";
}

static void printMenu() {
    std::cout << "\n====== FITNESS TRACKER ======\n"
              << " 1. Create workout plan\n"
              << " 2. Add exercise to plan\n"
              << " 3. View all plans\n"
              << " 4. Start training session\n"
              << " 5. View session history\n"
              << " 6. View personal records\n"
              << " 0. Exit\n"
              << "Choice: ";
}

int main() {
    std::string userName;
    std::cout << "Enter your name: ";
    std::getline(std::cin, userName);
    User user(userName);
    std::cout << "Welcome, " << user.getName() << "!\n";

    int choice = -1;
    while (choice != 0) {
        printMenu();
        std::cin >> choice;
        try {
            switch (choice) {
                case 1: menuCreatePlan(user);   break;
                case 2: menuAddExercise(user);  break;
                case 3: user.displayPlans();    break;
                case 4: menuStartSession(user); break;
                case 5: user.displayHistory();         break;
                case 6: user.displayPersonalRecords(); break;
                case 0: std::cout << "Goodbye!\n"; break;
                default: std::cout << "Invalid option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
