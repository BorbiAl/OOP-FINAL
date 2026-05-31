#include "ProgressAnalyzer.h"
#include <iostream>
#include <iomanip>

void ProgressAnalyzer::analyzeProgress(const std::vector<WorkoutSession>& history,
                                        const std::string& exerciseName) const {
    struct Entry { std::string date; double volume; };
    std::vector<Entry> entries;

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
        std::cout << "No data for exercise '" << exerciseName << "'.\n";
        return;
    }

    std::cout << "=== Progress: " << exerciseName << " ===\n";
    for (int i = 0; i < (int)entries.size(); ++i) {
        std::cout << "  [" << entries[i].date << "] volume: "
                  << std::fixed << std::setprecision(1) << entries[i].volume;
        if (i > 0) {
            double diff = entries[i].volume - entries[i - 1].volume;
            if (diff > 0)      std::cout << "  (+) improved by " << diff;
            else if (diff < 0) std::cout << "  (-) dropped by " << -diff;
            else               std::cout << "  (=) no change";
        }
        std::cout << "\n";
    }

    if (entries.size() > 1) {
        double overall = entries.back().volume - entries.front().volume;
        std::cout << "  Overall change: " << (overall >= 0 ? "+" : "") << overall << " kg\n";
    }
}
