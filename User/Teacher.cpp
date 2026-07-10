
#include "Teacher.h"

class Teacher;

Teacher::Teacher()
    : teacherId(""), name(""), email(""), department(""), phone(""), username(""), password("")
{
}

Teacher::Teacher(const std::string& teacherId, const std::string& name,
                 const std::string& email, const std::string& department,
                 const std::string& phone, const std::string& username,
                 const std::string& password)
    : teacherId(teacherId), name(name), email(email), department(department), phone(phone), username(username), password(password)
{
}

bool Teacher::login(const std::string& username, const std::string& password) const
{
    return this->username == username && this->password == password;
}

std::string Teacher::getTeacherId() const
{
    return teacherId;
}

std::string Teacher::getName() const
{
    return name;
}

std::string Teacher::getEmail() const
{
    return email;
}

std::string Teacher::getDepartment() const
{
    return department;
}

std::string Teacher::getPhone() const
{
    return phone;
}

void Teacher::printPersonalInfo() const
{
    std::cout << "Teacher ID: " << teacherId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Department: " << department << std::endl;
    std::cout << "Phone: " << phone << std::endl;
}

void Teacher::updatePersonalInfo(const std::string& name, const std::string& email,
                                 const std::string& department, const std::string& phone)
{
    this->name = name;
    this->email = email;
    this->department = department;
    this->phone = phone;
    std::cout << "Thong tin ca nhan da duoc cap nhat." << std::endl;
}

void Teacher::addAssignedCourse(const CourseInfo& course)
{
    assignedCourses.push_back(course);
}

std::vector<CourseInfo> Teacher::getAssignedCourses() const
{
    return assignedCourses;
}

void Teacher::printAssignedCourses() const
{
    std::cout << "Danh sach khoa hoc giao day:" << std::endl;
    for (const auto& course : assignedCourses) {
        std::cout << "  Ma mon: " << course.courseId
                  << " | Ten mon: " << course.courseName
                  << " | Hoc ky: " << course.semester
                  << " | Lop: " << course.classId << std::endl;
    }
}

void Teacher::printTeachingClasses() const
{
    std::cout << "Danh sach lop giao day:" << std::endl;
    std::map<std::string, bool> printed;
    for (const auto& course : assignedCourses) {
        if (!printed[course.classId]) {
            std::cout << "  Lop: " << course.classId
                      << " | Mon hoc: " << course.courseName
                      << " | Hoc ky: " << course.semester << std::endl;
            printed[course.classId] = true;
        }
    }
}

void Teacher::addStudentToClass(const std::string& classId, const StudentInfo& student)
{
    classStudents[classId].push_back(student);
}

std::vector<StudentInfo> Teacher::getStudentsInClass(const std::string& classId) const
{
    auto it = classStudents.find(classId);
    if (it == classStudents.end()) {
        return {};
    }
    return it->second;
}

void Teacher::printStudentsInClass(const std::string& classId) const
{
    std::cout << "Danh sach sinh vien lop " << classId << ":" << std::endl;
    auto it = classStudents.find(classId);
    if (it == classStudents.end()) {
        std::cout << "  Khong co sinh vien." << std::endl;
        return;
    }
    for (const auto& student : it->second) {
        std::cout << "  Ma SV: " << student.studentId
                  << " | Ten: " << student.name
                  << " | Nganh: " << student.major
                  << " | Email: " << student.email << std::endl;
    }
}

void Teacher::addScheduleEntry(const ScheduleEntry& entry)
{
    schedule.push_back(entry);
}

void Teacher::printSchedule() const
{
    std::cout << "Thoi khoa bieu lich day:" << std::endl;
    for (const auto& entry : schedule) {
        std::cout << "  Ma mon: " << entry.courseId
                  << " | Lop: " << entry.classId
                  << " | Thu: " << entry.dayOfWeek
                  << " | Gio: " << entry.timeRange
                  << " | Phong: " << entry.room << std::endl;
    }
}

void Teacher::addScore(const std::string& courseId, const std::string& studentId,
                       const std::string& assessment, double score)
{
    gradebook[courseId].emplace_back(studentId, assessment, score);
}

bool Teacher::editScore(const std::string& courseId, const std::string& studentId,
                        const std::string& assessment, double newScore)
{
    auto it = gradebook.find(courseId);
    if (it == gradebook.end()) {
        return false;
    }
    for (auto& record : it->second) {
        if (record.studentId == studentId && record.assessment == assessment) {
            record.score = newScore;
            return true;
        }
    }
    return false;
}

bool Teacher::deleteScore(const std::string& courseId, const std::string& studentId,
                          const std::string& assessment)
{
    auto it = gradebook.find(courseId);
    if (it == gradebook.end()) {
        return false;
    }
    auto& records = it->second;
    for (auto recordIt = records.begin(); recordIt != records.end(); ++recordIt) {
        if (recordIt->studentId == studentId && recordIt->assessment == assessment) {
            records.erase(recordIt);
            return true;
        }
    }
    return false;
}

void Teacher::printGradebook(const std::string& courseId) const
{
    std::cout << "Bang diem mon " << courseId << ":" << std::endl;
    auto it = gradebook.find(courseId);
    if (it == gradebook.end() || it->second.empty()) {
        std::cout << "  Khong co diem." << std::endl;
        return;
    }
    for (const auto& record : it->second) {
        std::cout << "  Ma SV: " << record.studentId
                  << " | Hinh thuc: " << record.assessment
                  << " | Diem: " << record.score << std::endl;
    }
}

void Teacher::addCourseContent(const std::string& courseId, const std::string& content)
{
    courseContent[courseId].push_back(content);
}

void Teacher::printCourseContent(const std::string& courseId) const
{
    std::cout << "Noi dung hoc tap mon " << courseId << ":" << std::endl;
    auto it = courseContent.find(courseId);
    if (it == courseContent.end() || it->second.empty()) {
        std::cout << "  Chua co noi dung." << std::endl;
        return;
    }
    for (const auto& content : it->second) {
        std::cout << "  - " << content << std::endl;
    }
}

void Teacher::addNotification(const std::string& message)
{
    notifications.push_back(message);
}

void Teacher::printNotifications() const
{
    std::cout << "Thong bao tu nha truong:" << std::endl;
    if (notifications.empty()) {
        std::cout << "  Khong co thong bao." << std::endl;
        return;
    }
    for (const auto& notification : notifications) {
        std::cout << "  - " << notification << std::endl;
    }
}
