#include "ProgressAnalyzer.h"
#include <iostream>
#include <iomanip>
using namespace std;

void ProgressAnalyzer::analyzeProgress(const vector<WorkoutSession>& history,
                                        const string& exerciseName) const {
    struct Entry { string date; double volume; };
    vector<Entry> entries;

    for (const auto& session : history) {
        double vol = 0;
        for (const auto& log : session.getLogs()) {
            if (log.getExerciseName() == exerciseName)
                vol += log.volume();
        }
        if (vol > 0)
            entries.push_back({session.getDate(), vol});
    }

    if (entries.empty()) {
        cout << "No data for exercise '" << exerciseName << "'.\n";
        return;
    }

    cout << "=== Progress: " << exerciseName << " ===\n";
    for (int i = 0; i < (int)entries.size(); ++i) {
        cout << "  [" << entries[i].date << "] volume: "
             << fixed << setprecision(1) << entries[i].volume;
        if (i > 0) {
            double diff = entries[i].volume - entries[i - 1].volume;
            if (diff > 0)      cout << "  (+) improved by " << diff;
            else if (diff < 0) cout << "  (-) dropped by " << -diff;
            else               cout << "  (=) no change";
        }
        cout << "\n";
    }

    if (entries.size() > 1) {
        double overall = entries.back().volume - entries.front().volume;
        cout << "  Overall change: " << (overall >= 0 ? "+" : "") << overall << " kg\n";
    }
}
