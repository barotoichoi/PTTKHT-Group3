#ifndef ENROLLMENT_COURSE_H
#define ENROLLMENT_COURSE_H

#include <string>

class Enrollment_course
{
private:
    std::string studentId;
    std::string courseId;
    std::string semester;
    std::string status;

public:
    Enrollment_course();
    Enrollment_course(const std::string& studentId, const std::string& courseId,
                      const std::string& semester, const std::string& status);
    ~Enrollment_course();

    std::string getStudentId() const;
    void setStudentId(const std::string& studentId);

    std::string getCourseId() const;
    void setCourseId(const std::string& courseId);

    std::string getSemester() const;
    void setSemester(const std::string& semester);

    std::string getStatus() const;
    void setStatus(const std::string& status);

    void addCourse();
    void cancelCourse();

    bool save();
    bool deleteRecord();
    bool find(const std::string& studentId, const std::string& courseId, const std::string& semester);
};

#endif
