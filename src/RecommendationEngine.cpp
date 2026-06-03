#include "RecommendationEngine.h"
#include <iostream>
#include <iomanip>
using namespace std;

void RecommendationEngine::generateRecommendation(
        const vector<WorkoutSession>& history,
        const string& exerciseName) const {

    struct Entry { double maxWeight; double volume; };
    vector<Entry> entries;

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

    cout << "=== Recommendations: " << exerciseName << " ===\n";

    if (entries.empty()) {
        cout << "  No data found. Start logging this exercise first.\n";
        return;
    }

    if (entries.size() == 1) {
        cout << "  Only one session recorded. Keep going and check back later.\n";
        return;
    }

    double lastVol   = entries.back().volume;
    double prevVol   = entries[entries.size() - 2].volume;
    double lastW     = entries.back().maxWeight;
    double change    = lastVol - prevVol;
    double changePct = prevVol > 0 ? (change / prevVol) * 100.0 : 0;

    bool plateau = entries.size() >= 3;
    if (plateau) {
        int n = (int)entries.size();
        for (int i = n - 2; i < n; ++i)
            if (entries[i].volume > entries[i - 1].volume) { plateau = false; break; }
    }

    if (plateau) {
        cout << "  Plateau detected over the last 3 sessions.\n";
        cout << "  -> Try reducing weight by 10% and increasing reps.\n";
        cout << "  -> Or substitute this exercise with a variation.\n";
    } else if (changePct > 10.0) {
        cout << "  Great progress! Volume up " << fixed << setprecision(1)
             << changePct << "% vs last session.\n";
        cout << "  -> Consider adding " << lastW * 0.05 << " kg next session.\n";
    } else if (change < 0) {
        cout << "  Volume dropped " << fixed << setprecision(1)
             << -changePct << "% vs last session.\n";
        cout << "  -> Ensure adequate rest and nutrition before next session.\n";
        cout << "  -> Stick with current weight (" << lastW << " kg) until volume recovers.\n";
    } else {
        cout << "  Steady progress. Volume change: " << showpos << fixed
             << setprecision(1) << changePct << "%\n" << noshowpos;
        cout << "  -> Maintain current weight and aim for one more rep per set.\n";
    }
}
