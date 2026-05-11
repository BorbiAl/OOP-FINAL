#pragma once
#include <string>

class Exercise {
protected:
    std::string name;
    std::string muscleGroup;
public:
    Exercise(const std::string& name, const std::string& muscleGroup);
    virtual ~Exercise() = default;
    virtual void perform() const = 0;
    virtual std::string getType() const = 0;
    const std::string& getName() const;
    const std::string& getMuscleGroup() const;
    virtual std::string toString() const;
};
