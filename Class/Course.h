#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    // Attributes aligned with the UML diagram
    std::string courseId;
    std::string name;
    int credits;

public:
    // Constructor
    Course(std::string id, std::string courseName, int courseCredits);

    // Getters
    std::string getCourseId() const;
    std::string getName() const;
    int getCredits() const;

    // Setters
    void setName(std::string courseName);
    void setCredits(int courseCredits);
};

#endif // COURSE_H