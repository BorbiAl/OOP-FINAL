#include "PlateauDetector.h"
#include <iostream>
#include <iomanip>
using namespace std;

bool PlateauDetector::detectPlateau(const vector<WorkoutSession>& history,
                                     const string& exerciseName,
                                     int windowSize) const {
    vector<double> volumes;
    for (const auto& session : history) {
        double vol = 0;
        for (const auto& log : session.getLogs()) {
            if (log.getExerciseName() == exerciseName)
                vol += log.volume();
        }
        if (vol > 0)
            volumes.push_back(vol);
    }

    if ((int)volumes.size() < windowSize) {
        cout << "Not enough sessions to detect plateau for '" << exerciseName
             << "' (need " << windowSize << ", have " << volumes.size() << ").\n";
        return false;
    }

    int start = (int)volumes.size() - windowSize;
    bool improving = false;
    for (int i = start + 1; i < (int)volumes.size(); ++i) {
        if (volumes[i] > volumes[i - 1]) { improving = true; break; }
    }

    if (!improving) {
        cout << "** PLATEAU DETECTED for '" << exerciseName << "' **\n"
             << "   No improvement in the last " << windowSize << " sessions.\n"
             << "   Consider changing weight, reps, or the exercise itself.\n";
        return true;
    }

    cout << "No plateau detected for '" << exerciseName << "' in the last "
         << windowSize << " sessions.\n";
    return false;
}
