#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Teacher.h"

class Student {
private:
    std::string studentId;
    std::string name;
    std::string email;
    std::string major;
    std::string phone;
    std::string username;
    std::string password;
    double gpa;
    double tuitionOwed;
    std::vector<CourseInfo> enrolledCourses;
    std::vector<ScheduleEntry> schedule;
    std::map<std::string, std::vector<ScoreRecord>> gradebook;
    std::vector<std::string> notifications;
    std::map<std::string, std::vector<std::string>> prerequisiteMap;

public:
    Student();
    Student(const std::string& studentId, const std::string& name,
            const std::string& email, const std::string& major,
            const std::string& phone, const std::string& username,
            const std::string& password, double gpa = 0.0,
            double tuitionOwed = 0.0);

    bool login(const std::string& username, const std::string& password) const;

    std::string getStudentId() const;
    void setStudentId(const std::string& studentId);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getEmail() const;
    void setEmail(const std::string& email);

    std::string getMajor() const;
    void setMajor(const std::string& major);

    std::string getPhone() const;
    void setPhone(const std::string& phone);

    std::string getUsername() const;
    void setUsername(const std::string& username);

    std::string getPassword() const;
    void setPassword(const std::string& password);

    double getGpa() const;
    void setGpa(double gpa);

    double getTuitionOwed() const;
    void setTuitionOwed(double tuitionOwed);

    void printPersonalInfo() const;
    void updatePersonalInfo(const std::string& name, const std::string& email,
                            const std::string& major, const std::string& phone);

    void printStudentCard() const;
    void printGPA() const;
    void printGradebook() const;

    void addEnrolledCourse(const CourseInfo& course);
    std::vector<CourseInfo> getEnrolledCourses() const;
    void printEnrolledCourses() const;
    void enrollCourse(const CourseInfo& course);
    void dropCourse(const std::string& courseId);
    bool checkPrerequisites(const std::string& courseId) const;

    void addScheduleEntry(const ScheduleEntry& entry);
    std::vector<ScheduleEntry> getSchedule() const;
    void printSchedule() const;

    void addGrade(const std::string& courseId, const ScoreRecord& record);
    std::map<std::string, std::vector<ScoreRecord>> getGradebook() const;

    void addNotification(const std::string& message);
    std::vector<std::string> getNotifications() const;
    void printNotifications() const;

    void printTuitionStatus() const;
    void payTuition(double amount);

    void addPrerequisite(const std::string& courseId, const std::vector<std::string>& requiredCourses);
};

#endif
