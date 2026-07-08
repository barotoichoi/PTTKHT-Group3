#include "Enrollment_course.h"
#include <iostream>

Enrollment_course::Enrollment_course()
    : studentId(""), courseId(""), semester(""), status("Chờ duyệt")
{
}

Enrollment_course::Enrollment_course(const std::string& studentId, const std::string& courseId,
                                     const std::string& semester, const std::string& status)
{
    this->studentId = studentId;
    this->courseId = courseId;
    this->semester = semester;
    this->status = status;
}

Enrollment_course::~Enrollment_course()
{
}

std::string Enrollment_course::getStudentId() const
{
    return studentId;
}

void Enrollment_course::setStudentId(const std::string& studentId)
{
    this->studentId = studentId;
}

std::string Enrollment_course::getCourseId() const
{
    return courseId;
}

void Enrollment_course::setCourseId(const std::string& courseId)
{
    this->courseId = courseId;
}

std::string Enrollment_course::getSemester() const
{
    return semester;
}

void Enrollment_course::setSemester(const std::string& semester)
{
    this->semester = semester;
}

std::string Enrollment_course::getStatus() const
{
    return status;
}

void Enrollment_course::setStatus(const std::string& status)
{
    this->status = status;
}

void Enrollment_course::addCourse()
{
    status = "Thành công";
    std::cout << "Dang ky mon hoc thanh cong." << std::endl;
}

void Enrollment_course::cancelCourse()
{
    status = "Hủy";
    std::cout << "Da huy mon hoc." << std::endl;
}
