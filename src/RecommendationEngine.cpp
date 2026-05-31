#include "RecommendationEngine.h"
#include <iostream>
#include <iomanip>

void RecommendationEngine::generateRecommendation(
        const std::vector<WorkoutSession>& history,
        const std::string& exerciseName) const {

    // Collect per-session max weight and total volume
    struct Entry { double maxWeight; double volume; };
    std::vector<Entry> entries;

    for (const auto& session : history) {
        double maxW = 0, vol = 0;
        for (const auto& log : session.getLogs()) {
            if (log.getExerciseName() == exerciseName) {
                vol += log.volume();
                if (log.getWeight() > maxW) maxW = log.getWeight();
            }
        }
        if (vol > 0) entries.push_back({maxW, vol});
    }

    std::cout << "=== Recommendations: " << exerciseName << " ===\n";

    if (entries.empty()) {
        std::cout << "  No data found. Start logging this exercise first.\n";
        return;
    }

    if (entries.size() == 1) {
        std::cout << "  Only one session recorded. Keep going and check back later.\n";
        return;
    }

    double lastVol  = entries.back().volume;
    double prevVol  = entries[entries.size() - 2].volume;
    double lastW    = entries.back().maxWeight;
    double change   = lastVol - prevVol;
    double changePct = prevVol > 0 ? (change / prevVol) * 100.0 : 0;

    // Check last 3 sessions for plateau
    bool plateau = entries.size() >= 3;
    if (plateau) {
        int n = (int)entries.size();
        for (int i = n - 2; i < n; ++i)
            if (entries[i].volume > entries[i - 1].volume) { plateau = false; break; }
    }

    if (plateau) {
        std::cout << "  Plateau detected over the last 3 sessions.\n";
        std::cout << "  -> Try reducing weight by 10% and increasing reps.\n";
        std::cout << "  -> Or substitute this exercise with a variation.\n";
    } else if (changePct > 10.0) {
        std::cout << "  Great progress! Volume up " << std::fixed << std::setprecision(1)
                  << changePct << "% vs last session.\n";
        std::cout << "  -> Consider adding " << lastW * 0.05 << " kg next session.\n";
    } else if (change < 0) {
        std::cout << "  Volume dropped " << std::fixed << std::setprecision(1)
                  << -changePct << "% vs last session.\n";
        std::cout << "  -> Ensure adequate rest and nutrition before next session.\n";
        std::cout << "  -> Stick with current weight (" << lastW << " kg) until volume recovers.\n";
    } else {
        std::cout << "  Steady progress. Volume change: " << std::showpos << std::fixed
                  << std::setprecision(1) << changePct << "%\n" << std::noshowpos;
        std::cout << "  -> Maintain current weight and aim for one more rep per set.\n";
    }
}
