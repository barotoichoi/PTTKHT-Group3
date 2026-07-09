#include "Student.h"

#include <algorithm>
#include <cmath>
#include <sqlite3.h>

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

std::vector<Student> Student::loadStudentsFromDatabase(const std::string& dbPath)
{
    std::vector<Student> students;
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cout << "Khong the mo co so du lieu: " << dbPath << std::endl;
        if (db) {
            sqlite3_close(db);
        }
        return students;
    }

    const char* sql = "SELECT studentId, name, email, major, phone, username, password, gpa, tuitionOwed FROM students;";
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cout << "Loi prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return students;
    }

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string studentId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string major = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        double gpa = sqlite3_column_double(stmt, 7);
        double tuitionOwed = sqlite3_column_double(stmt, 8);

        students.emplace_back(studentId, name, email, major, phone, username, password, gpa, tuitionOwed);
    }

    if (result != SQLITE_DONE) {
        std::cout << "Loi khi doc du lieu: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return students;
}

std::vector<Student> Student::loadDefaultStudents()
{
    return loadStudentsFromDatabase("student_db.sqlite");
}

bool Student::initializeDatabase(const std::string& dbPath)
{
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return false;
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS students ("
                      "studentId TEXT PRIMARY KEY,"
                      "name TEXT NOT NULL,"
                      "email TEXT NOT NULL,"
                      "major TEXT NOT NULL,"
                      "phone TEXT NOT NULL,"
                      "username TEXT NOT NULL UNIQUE,"
                      "password TEXT NOT NULL,"
                      "gpa REAL NOT NULL DEFAULT 0.0,"
                      "tuitionOwed REAL NOT NULL DEFAULT 0.0);";

    char* errMsg = nullptr;
    bool ok = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) == SQLITE_OK;
    if (!ok) {
        std::cout << "Loi tao bang: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    return ok;
}

bool Student::saveToDatabase(const std::string& dbPath) const
{
    if (!initializeDatabase(dbPath)) {
        return false;
    }

    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return false;
    }

    const char* sql = "INSERT OR REPLACE INTO students (studentId, name, email, major, phone, username, password, gpa, tuitionOwed) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cout << "Loi prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Use bind to prevent SQL injection and handle embedded quotes safely.
    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, major.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, phone.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 8, gpa);
    sqlite3_bind_double(stmt, 9, tuitionOwed);

    result = sqlite3_step(stmt);
    bool ok = result == SQLITE_DONE;
    if (!ok) {
        std::cout << "Loi ghi du lieu: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

bool Student::deleteFromDatabase(const std::string& dbPath) const
{
    if (!initializeDatabase(dbPath)) {
        return false;
    }

    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return false;
    }

    const char* sql = "DELETE FROM students WHERE studentId = ?;";
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cout << "Loi prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, studentId.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    bool ok = result == SQLITE_DONE;
    if (!ok) {
        std::cout << "Loi xoa du lieu: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

Student Student::findByUsername(const std::string& username, const std::string& dbPath)
{
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return Student();
    }

    const char* sql = "SELECT studentId, name, email, major, phone, username, password, gpa, tuitionOwed FROM students WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cout << "Loi prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return Student();
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        if (result != SQLITE_DONE) {
            std::cout << "Loi truy van: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return Student();
    }

    std::string studentId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    std::string major = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    std::string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    std::string foundUsername = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    double gpa = sqlite3_column_double(stmt, 7);
    double tuitionOwed = sqlite3_column_double(stmt, 8);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return Student(studentId, name, email, major, phone, foundUsername, password, gpa, tuitionOwed);
}
