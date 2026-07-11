#ifndef TEACHER_H
#define TEACHER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

struct CourseInfo {
    std::string courseId;
    std::string courseName;
    std::string semester;
    std::string classId;

    CourseInfo() = default;
    CourseInfo(const std::string& courseId, const std::string& courseName,
               const std::string& semester, const std::string& classId)
        : courseId(courseId), courseName(courseName), semester(semester), classId(classId) {}
};

struct StudentInfo {
    std::string studentId;
    std::string name;
    std::string major;
    std::string email;

    StudentInfo() = default;
    StudentInfo(const std::string& studentId, const std::string& name,
                const std::string& major, const std::string& email)
        : studentId(studentId), name(name), major(major), email(email) {}
};

struct ScoreRecord {
    std::string studentId;
    std::string assessment;
    double score;
    std::string updatedAt;

    ScoreRecord();
    ScoreRecord(const std::string& studentId,
                const std::string& assessment,
                double score,
                const std::string& updatedAt = "");
};

struct ScheduleEntry {
    std::string courseId;
    std::string classId;
    std::string dayOfWeek;
    std::string timeRange;
    std::string room;

    ScheduleEntry() = default;
    ScheduleEntry(const std::string& courseId, const std::string& classId,
                  const std::string& dayOfWeek, const std::string& timeRange,
                  const std::string& room)
        : courseId(courseId), classId(classId), dayOfWeek(dayOfWeek), timeRange(timeRange), room(room) {}
};

struct TeacherNotification {
    std::string message;
    std::string createdAt;
    bool isRead;

    TeacherNotification();
    TeacherNotification(const std::string& message,
                        const std::string& createdAt = "",
                        bool isRead = false);
};

class Teacher {
private:
    std::string teacherId;
    std::string name;
    std::string email;
    std::string department;
    std::string phone;
    std::string username;
    std::string passwordHash;
    std::string status;

    std::vector<CourseInfo> assignedCourses;
    std::map<std::string, std::vector<StudentInfo>> classStudents;
    std::map<std::string, std::vector<ScoreRecord>> gradebook;
    std::map<std::string, std::vector<std::string>> courseContent;
    std::vector<ScheduleEntry> schedule;
    std::vector<TeacherNotification> notifications;

    static std::string currentTimestamp();
    static std::string hashPassword(const std::string& username, const std::string& password);

public:
    Teacher();
    Teacher(const std::string& teacherId, const std::string& name,
            const std::string& email, const std::string& department,
            const std::string& phone, const std::string& username,
            const std::string& password);

    bool login(const std::string& username, const std::string& password) const;

    std::string getTeacherId() const;
    void setTeacherId(const std::string& teacherId);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getEmail() const;
    void setEmail(const std::string& email);

    std::string getDepartment() const;
    void setDepartment(const std::string& department);

    std::string getPhone() const;
    void setPhone(const std::string& phone);

    std::string getUsername() const;
    void setUsername(const std::string& username);

    std::string getPassword() const;
    void setPassword(const std::string& password);

    std::string getPasswordHash() const;
    void setPasswordHash(const std::string& passwordHash);

    std::string getStatus() const;
    void setStatus(const std::string& status);

    void printPersonalInfo() const;
    void updatePersonalInfo(const std::string& name, const std::string& email,
                            const std::string& department, const std::string& phone);
    void printTeacherCard() const;

    void addAssignedCourse(const CourseInfo& course);
    std::vector<CourseInfo> getAssignedCourses() const;
    void printAssignedCourses() const;
    void printTeachingClasses() const;

    void addStudentToClass(const std::string& classId, const StudentInfo& student);
    std::vector<StudentInfo> getStudentsInClass(const std::string& classId) const;
    void printStudentsInClass(const std::string& classId) const;

    void addScheduleEntry(const ScheduleEntry& entry);
    std::vector<ScheduleEntry> getSchedule() const;
    void printSchedule() const;

    void addScore(const std::string& courseId, const std::string& studentId,
                  const std::string& assessment, double score);
    bool editScore(const std::string& courseId, const std::string& studentId,
                   const std::string& assessment, double newScore);
    bool deleteScore(const std::string& courseId, const std::string& studentId,
                     const std::string& assessment);
    void printGradebook(const std::string& courseId) const;

    void addCourseContent(const std::string& courseId, const std::string& content);
    void printCourseContent(const std::string& courseId) const;

    void addNotification(const std::string& message);
    std::vector<TeacherNotification> getNotifications() const;
    void printNotifications() const;
};

#endif // TEACHER_H
