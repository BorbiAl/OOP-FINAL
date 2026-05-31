#include "LoadTracker.h"
#include <iostream>
#include <iomanip>
#include <numeric>

void LoadTracker::analyzeLoad(const std::vector<WorkoutSession>& history, int windowSize) const {
    if (history.empty()) { std::cout << "No sessions to analyze.\n"; return; }

    if ((int)history.size() < 2) {
        std::cout << "=== Load Analysis ===\n"
                  << "  Only one session recorded. Need more data.\n";
        return;
    }

    double lastVol = history.back().totalVolume();

    // Average of up to the previous `windowSize` sessions (excluding last)
    int n = (int)history.size() - 1;
    int start = std::max(0, n - windowSize);
    double sum = 0;
    int count = 0;
    for (int i = start; i < n; ++i) {
        sum += history[i].totalVolume();
        ++count;
    }
    double avg = sum / count;

    std::cout << "=== Load Analysis ===\n"
              << "  Last session volume : " << std::fixed << std::setprecision(1) << lastVol << " kg\n"
              << "  Rolling avg (prev " << count << " sessions): " << avg << " kg\n";

    if (avg == 0) { std::cout << "  Not enough volume data to compare.\n"; return; }

    double pct = ((lastVol - avg) / avg) * 100.0;

    if (pct > 50.0) {
        std::cout << "  ** OVERTRAINING WARNING: volume spiked +" << std::fixed << std::setprecision(1)
                  << pct << "% above average. **\n"
                  << "  -> Consider a lighter session or rest day next.\n";
    } else if (pct < -50.0) {
        std::cout << "  ** LOW LOAD: volume dropped " << std::fixed << std::setprecision(1)
                  << -pct << "% below average. **\n"
                  << "  -> Increase intensity gradually in the next session.\n";
    } else {
        std::cout << "  Load is within normal range (" << std::showpos << std::fixed
                  << std::setprecision(1) << pct << "% vs avg). Good balance.\n" << std::noshowpos;
    }
}
