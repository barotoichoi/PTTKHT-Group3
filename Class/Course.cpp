#include "Course.h"

// Constructor implementation
Course::Course(std::string id, std::string courseName, int courseCredits) {
    courseId = id;
    name = courseName;
    credits = courseCredits;
}

// Getter implementations
std::string Course::getCourseId() const {
    return courseId;
}

std::string Course::getName() const {
    return name;
}

int Course::getCredits() const {
    return credits;
}

// Setter implementations
void Course::setName(std::string courseName) {
    name = courseName;
}

void Course::setCredits(int courseCredits) {
    credits = courseCredits;
}