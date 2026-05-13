cmake_minimum_required(VERSION 3.10)
project(FitnessTracker)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(fitness_tracker
    src/main.cpp
    src/Exercise.cpp
    src/StrengthExercise.cpp
    src/CardioExercise.cpp
    src/SetLog.cpp
    src/WorkoutPlan.cpp
    src/WorkoutSession.cpp
    src/User.cpp
)
