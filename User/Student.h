#pragma once

#include <iosfwd>
#include <string>
#include <vector>

class Student {
public:
    std::string studentId;
    std::string name;
    std::string gender;
    std::string dob;
    std::string classId;
    std::string email;
    std::string major;
    std::string phone;
    std::string username;
    std::string password;
    double gpa;
    double tuitionOwed;
    std::string status;

    Student();
    Student(const std::string& studentId,
            const std::string& name,
            const std::string& gender,
            const std::string& dob,
            const std::string& classId,
            const std::string& email,
            const std::string& major,
            const std::string& phone,
            const std::string& username,
            const std::string& password,
            double gpa,
            double tuitionOwed,
            const std::string& status);

    bool login(const std::string& username, const std::string& password) const;
    void displayInfo() const;
};

struct StudentCourse {
    std::string courseId;
    std::string courseName;
    int credits;
    std::string teacherName;
    std::string room;
    std::string semester;
    std::string status;
};

struct StudentSchedule {
    std::string scheduleId;
    std::string courseId;
    std::string courseName;
    std::string day;
    std::string startTime;
    std::string endTime;
    std::string room;
};

struct StudentScore {
    std::string courseId;
    std::string courseName;
    double assignment;
    double midterm;
    double finalExam;
    double average;
    std::string grade;
};

struct CourseMaterial {
    std::string materialId;
    std::string courseId;
    std::string courseName;
    std::string title;
    std::string url;
    std::string createdAt;
};

struct StudentNotification {
    std::string notificationId;
    std::string studentId;
    std::string title;
    std::string content;
    std::string createdAt;
    bool isRead;
};

struct StudentTuition {
    std::string tuitionId;
    std::string studentId;
    std::string semester;
    int totalCredits;
    double amount;
    double paidAmount;
    std::string dueDate;
    std::string status;
};

class StudentRepository {
public:
    explicit StudentRepository(const std::string& connectionString = defaultConnectionString());
    ~StudentRepository();

    static std::string defaultConnectionString();

    bool connect();
    void disconnect();
    bool initializeSchema();
    bool seedSampleData();

    bool addStudent(const Student& student);
    bool updateStudent(const Student& student);
    bool updatePersonalInfo(const std::string& studentId,
                            const std::string& email,
                            const std::string& phone,
                            const std::string& password);
    bool deleteStudent(const std::string& studentId);
    bool findStudentById(const std::string& studentId, Student& student);
    bool login(const std::string& username, const std::string& password, Student& student);
    std::vector<Student> getAllStudents();

    std::vector<StudentCourse> getAvailableCourses(const std::string& semester);
    std::vector<StudentCourse> getCurrentCourses(const std::string& studentId, const std::string& semester);
    std::vector<StudentSchedule> getSchedule(const std::string& studentId, const std::string& semester);
    std::vector<StudentScore> getScores(const std::string& studentId, const std::string& semester);
    std::vector<CourseMaterial> getNewCourseMaterials(const std::string& studentId);
    std::vector<StudentNotification> getTuitionNotifications(const std::string& studentId);

    bool enrollCourse(const std::string& studentId, const std::string& courseId, const std::string& semester);
    bool cancelEnrollment(const std::string& studentId, const std::string& courseId, const std::string& semester);
    bool printTuitionStatement(const std::string& studentId, const std::string& semester, std::ostream& output);
    std::vector<StudentTuition> getTuitionRecords(const std::string& studentId);
    std::vector<StudentTuition> getUnpaidTuitionRecords(const std::string& studentId);
    bool payTuition(const std::string& tuitionId, double amount);

private:
    void* environmentHandle;
    void* connectionHandle;
    std::string connectionString;

    bool executeNonQuery(const std::string& sql);
    std::vector<std::vector<std::string>> executeQuery(const std::string& sql, int columnCount);
    static std::string escapeSql(const std::string& value);
    static std::string toSqlString(const std::string& value);
    static double toDouble(const std::string& value);
    static int toInt(const std::string& value);
};
