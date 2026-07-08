#include "Student.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

Student::Student()
    : studentId(""), name(""), email(""), major(""), phone(""), username(""), password(""),
      gpa(0.0), tuitionOwed(0.0)
{
}

Student::Student(const std::string& studentId, const std::string& name,
                 const std::string& email, const std::string& major,
                 const std::string& phone, const std::string& username,
                 const std::string& password, double gpa, double tuitionOwed)
    : studentId(studentId), name(name), email(email), major(major), phone(phone),
      username(username), password(password), gpa(gpa), tuitionOwed(tuitionOwed)
{
}

bool Student::login(const std::string& username, const std::string& password) const
{
    return this->username == username && this->password == password;
}

std::string Student::getStudentId() const
{
    return studentId;
}

void Student::setStudentId(const std::string& studentId)
{
    this->studentId = studentId;
}

std::string Student::getName() const
{
    return name;
}

void Student::setName(const std::string& name)
{
    this->name = name;
}

std::string Student::getEmail() const
{
    return email;
}

void Student::setEmail(const std::string& email)
{
    this->email = email;
}

std::string Student::getMajor() const
{
    return major;
}

void Student::setMajor(const std::string& major)
{
    this->major = major;
}

std::string Student::getPhone() const
{
    return phone;
}

void Student::setPhone(const std::string& phone)
{
    this->phone = phone;
}

std::string Student::getUsername() const
{
    return username;
}

void Student::setUsername(const std::string& username)
{
    this->username = username;
}

std::string Student::getPassword() const
{
    return password;
}

void Student::setPassword(const std::string& password)
{
    this->password = password;
}

double Student::getGpa() const
{
    return gpa;
}

void Student::setGpa(double gpa)
{
    this->gpa = gpa;
}

double Student::getTuitionOwed() const
{
    return tuitionOwed;
}

void Student::setTuitionOwed(double tuitionOwed)
{
    this->tuitionOwed = tuitionOwed;
}

void Student::printPersonalInfo() const
{
    std::cout << "Thong tin ca nhan:" << std::endl;
    std::cout << "  Ma sinh vien: " << studentId << std::endl;
    std::cout << "  Ho ten: " << name << std::endl;
    std::cout << "  Email: " << email << std::endl;
    std::cout << "  Nganh hoc: " << major << std::endl;
    std::cout << "  So dien thoai: " << phone << std::endl;
}

void Student::updatePersonalInfo(const std::string& name, const std::string& email,
                                 const std::string& major, const std::string& phone)
{
    this->name = name;
    this->email = email;
    this->major = major;
    this->phone = phone;
    std::cout << "Thong tin ca nhan da duoc cap nhat." << std::endl;
}

void Student::printStudentCard() const
{
    std::cout << "The sinh vien" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << "Ma sinh vien: " << studentId << std::endl;
    std::cout << "Ho ten: " << name << std::endl;
    std::cout << "Nganh: " << major << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "So dien thoai: " << phone << std::endl;
    std::cout << "GPA: " << gpa << std::endl;
}

void Student::printGPA() const
{
    double computedGpa = gpa;
    if (std::fabs(computedGpa) < 1e-9 && !gradebook.empty()) {
        double total = 0.0;
        int count = 0;
        for (const auto& entry : gradebook) {
            for (const auto& record : entry.second) {
                total += record.score;
                ++count;
            }
        }
        if (count > 0) {
            computedGpa = total / count;
        }
    }

    std::cout << "GPA hien tai: " << computedGpa << std::endl;
}

void Student::printGradebook() const
{
    std::cout << "Bang diem cua sinh vien:" << std::endl;
    if (gradebook.empty()) {
        std::cout << "  Chua co diem nao." << std::endl;
        return;
    }

    for (const auto& courseEntry : gradebook) {
        std::cout << "Mon: " << courseEntry.first << std::endl;
        if (courseEntry.second.empty()) {
            std::cout << "  Chua co diem." << std::endl;
            continue;
        }
        for (const auto& record : courseEntry.second) {
            std::cout << "  Hinh thuc: " << record.assessment
                      << " | Diem: " << record.score << std::endl;
        }
    }
}

void Student::addEnrolledCourse(const CourseInfo& course)
{
    enrolledCourses.push_back(course);
}

std::vector<CourseInfo> Student::getEnrolledCourses() const
{
    return enrolledCourses;
}

void Student::printEnrolledCourses() const
{
    std::cout << "Danh sach mon dang hoc:" << std::endl;
    if (enrolledCourses.empty()) {
        std::cout << "  Chua dang ky mon nao." << std::endl;
        return;
    }

    for (const auto& course : enrolledCourses) {
        std::cout << "  Ma mon: " << course.courseId
                  << " | Ten mon: " << course.courseName
                  << " | Hoc ky: " << course.semester
                  << " | Lop: " << course.classId << std::endl;
    }
}

void Student::enrollCourse(const CourseInfo& course)
{
    for (const auto& enrolledCourse : enrolledCourses) {
        if (enrolledCourse.courseId == course.courseId) {
            std::cout << "Ban da dang ky mon nay truoc do." << std::endl;
            return;
        }
    }

    if (!checkPrerequisites(course.courseId)) {
        std::cout << "Khong du dieu kien tien quyet de dang ky mon " << course.courseId << "." << std::endl;
        return;
    }

    enrolledCourses.push_back(course);
    gradebook[course.courseId];
    tuitionOwed += 1500000.0;
    notifications.push_back("Dang ky mon " + course.courseId + " thanh cong.");
    std::cout << "Dang ky mon " << course.courseId << " thanh cong." << std::endl;
}

void Student::dropCourse(const std::string& courseId)
{
    auto courseIt = std::find_if(enrolledCourses.begin(), enrolledCourses.end(),
                                 [&](const CourseInfo& course) {
                                     return course.courseId == courseId;
                                 });

    if (courseIt == enrolledCourses.end()) {
        std::cout << "Khong tim thay mon hoc de huy." << std::endl;
        return;
    }

    enrolledCourses.erase(courseIt);
    gradebook.erase(courseId);
    tuitionOwed = std::max(0.0, tuitionOwed - 1500000.0);
    notifications.push_back("Da huy mon " + courseId + ".");
    std::cout << "Da huy mon " << courseId << " thanh cong." << std::endl;
}

bool Student::checkPrerequisites(const std::string& courseId) const
{
    auto prereqIt = prerequisiteMap.find(courseId);
    if (prereqIt == prerequisiteMap.end() || prereqIt->second.empty()) {
        return true;
    }

    for (const auto& prereq : prereqIt->second) {
        auto gradeIt = gradebook.find(prereq);
        bool completed = false;
        if (gradeIt != gradebook.end()) {
            for (const auto& record : gradeIt->second) {
                if (record.score >= 5.0) {
                    completed = true;
                    break;
                }
            }
        }

        if (!completed) {
            return false;
        }
    }

    return true;
}

void Student::addScheduleEntry(const ScheduleEntry& entry)
{
    schedule.push_back(entry);
}

std::vector<ScheduleEntry> Student::getSchedule() const
{
    return schedule;
}

void Student::printSchedule() const
{
    std::cout << "Thoi khoa bieu:" << std::endl;
    if (schedule.empty()) {
        std::cout << "  Chua co lich hoc." << std::endl;
        return;
    }

    for (const auto& entry : schedule) {
        std::cout << "  Ma mon: " << entry.courseId
                  << " | Lop: " << entry.classId
                  << " | Thu: " << entry.dayOfWeek
                  << " | Gio: " << entry.timeRange
                  << " | Phong: " << entry.room << std::endl;
    }
}

void Student::addGrade(const std::string& courseId, const ScoreRecord& record)
{
    gradebook[courseId].push_back(record);
}

std::map<std::string, std::vector<ScoreRecord>> Student::getGradebook() const
{
    return gradebook;
}

void Student::addNotification(const std::string& message)
{
    notifications.push_back(message);
}

std::vector<std::string> Student::getNotifications() const
{
    return notifications;
}

void Student::printNotifications() const
{
    std::cout << "Thong bao:" << std::endl;
    if (notifications.empty()) {
        std::cout << "  Khong co thong bao." << std::endl;
        return;
    }

    for (const auto& message : notifications) {
        std::cout << "  - " << message << std::endl;
    }
}

void Student::printTuitionStatus() const
{
    std::cout << "Trang thai hoc phi:" << std::endl;
    if (tuitionOwed <= 0.0) {
        std::cout << "  Da nop het hoc phi." << std::endl;
        return;
    }

    std::cout << "  So tien con no: " << tuitionOwed << " dong" << std::endl;
}

void Student::payTuition(double amount)
{
    if (amount <= 0.0) {
        std::cout << "So tien nop khong hop le." << std::endl;
        return;
    }

    tuitionOwed = std::max(0.0, tuitionOwed - amount);
    notifications.push_back("Da nop hoc phi: " + std::to_string(amount) + " dong.");
    std::cout << "Da nop hoc phi thanh cong." << std::endl;
}

void Student::addPrerequisite(const std::string& courseId, const std::vector<std::string>& requiredCourses)
{
    prerequisiteMap[courseId] = requiredCourses;
}

std::vector<Student> Student::loadStudentsFromFile(const std::string& filepath)
{
    std::vector<Student> students;
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        std::cout << "Khong the mo file: " << filepath << std::endl;
        return students;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF && static_cast<unsigned char>(line[1]) == 0xBB && static_cast<unsigned char>(line[2]) == 0xBF) {
            line = line.substr(3);
        }

        std::istringstream ss(line);
        std::vector<std::string> cols;
        std::string token;
        while (std::getline(ss, token, ',')) {
            cols.push_back(token);
        }

        if (cols.size() < 7) {
            continue;
        }

        double parsedGpa = 0.0;
        double parsedTuition = 0.0;
        try {
            if (cols.size() > 7 && !cols[7].empty()) parsedGpa = std::stod(cols[7]);
            if (cols.size() > 8 && !cols[8].empty()) parsedTuition = std::stod(cols[8]);
        } catch (...) {
        }

        Student s(cols[0], cols[1], cols[2], cols[3], cols[4], cols[5], cols[6], parsedGpa, parsedTuition);
        students.push_back(s);
    }

    return students;
}

std::vector<Student> Student::loadDefaultStudents()
{
    return loadStudentsFromFile("data/students.csv");
}
