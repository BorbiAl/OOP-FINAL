#include "LoadTracker.h"
#include <iostream>
#include <iomanip>
#include <numeric>
using namespace std;

void LoadTracker::analyzeLoad(const vector<WorkoutSession>& history, int windowSize) const {
    if (history.empty()) { cout << "No sessions to analyze.\n"; return; }

    if ((int)history.size() < 2) {
        cout << "=== Load Analysis ===\n"
             << "  Only one session recorded. Need more data.\n";
        return;
    }

    double lastVol = history.back().totalVolume();

    int n = (int)history.size() - 1;
    int start = max(0, n - windowSize);
    double sum = 0;
    int count = 0;
    for (int i = start; i < n; ++i) {
        sum += history[i].totalVolume();
        ++count;
    }
    double avg = sum / count;

    cout << "=== Load Analysis ===\n"
         << "  Last session volume : " << fixed << setprecision(1) << lastVol << " kg\n"
         << "  Rolling avg (prev " << count << " sessions): " << avg << " kg\n";

    if (avg == 0) { cout << "  Not enough volume data to compare.\n"; return; }

    double pct = ((lastVol - avg) / avg) * 100.0;

    if (pct > 50.0) {
        cout << "  ** OVERTRAINING WARNING: volume spiked +" << fixed << setprecision(1)
             << pct << "% above average. **\n"
             << "  -> Consider a lighter session or rest day next.\n";
    } else if (pct < -50.0) {
        cout << "  ** LOW LOAD: volume dropped " << fixed << setprecision(1)
             << -pct << "% below average. **\n"
             << "  -> Increase intensity gradually in the next session.\n";
    } else {
        cout << "  Load is within normal range (" << showpos << fixed
             << setprecision(1) << pct << "% vs avg). Good balance.\n" << noshowpos;
    }
}
