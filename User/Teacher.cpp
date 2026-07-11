#include "Teacher.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
uint32_t rotateRight(uint32_t value, uint32_t bits) {
    return (value >> bits) | (value << (32 - bits));
}

std::string sha256Hex(const std::string& input) {
    static const std::array<uint32_t, 64> k = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    std::array<uint32_t, 8> h = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    std::vector<uint8_t> bytes(input.begin(), input.end());
    const uint64_t bitLength = static_cast<uint64_t>(bytes.size()) * 8;
    bytes.push_back(0x80);
    while ((bytes.size() % 64) != 56) bytes.push_back(0);
    for (int shift = 56; shift >= 0; shift -= 8) {
        bytes.push_back(static_cast<uint8_t>((bitLength >> shift) & 0xff));
    }

    for (size_t offset = 0; offset < bytes.size(); offset += 64) {
        std::array<uint32_t, 64> w = {};
        for (int i = 0; i < 16; ++i) {
            const size_t j = offset + i * 4;
            w[i] = (static_cast<uint32_t>(bytes[j]) << 24) |
                   (static_cast<uint32_t>(bytes[j + 1]) << 16) |
                   (static_cast<uint32_t>(bytes[j + 2]) << 8) |
                   static_cast<uint32_t>(bytes[j + 3]);
        }
        for (int i = 16; i < 64; ++i) {
            const uint32_t s0 = rotateRight(w[i - 15], 7) ^ rotateRight(w[i - 15], 18) ^ (w[i - 15] >> 3);
            const uint32_t s1 = rotateRight(w[i - 2], 17) ^ rotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hash = h[7];

        for (int i = 0; i < 64; ++i) {
            const uint32_t s1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
            const uint32_t ch = (e & f) ^ ((~e) & g);
            const uint32_t temp1 = hash + s1 + ch + k[i] + w[i];
            const uint32_t s0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
            const uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            const uint32_t temp2 = s0 + maj;

            hash = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hash;
    }

    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (uint32_t part : h) out << std::setw(8) << part;
    return out.str();
}

std::string makeCurrentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime {};
#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif
    std::ostringstream out;
    out << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return out.str();
}
}

ScoreRecord::ScoreRecord()
    : studentId(""), assessment(""), score(0.0), updatedAt(makeCurrentTimestamp()) {}

ScoreRecord::ScoreRecord(const std::string& studentId,
                         const std::string& assessment,
                         double score,
                         const std::string& updatedAt)
    : studentId(studentId),
      assessment(assessment),
      score(score),
      updatedAt(updatedAt.empty() ? makeCurrentTimestamp() : updatedAt) {}

TeacherNotification::TeacherNotification()
    : message(""), createdAt(makeCurrentTimestamp()), isRead(false) {}

TeacherNotification::TeacherNotification(const std::string& message,
                                         const std::string& createdAt,
                                         bool isRead)
    : message(message),
      createdAt(createdAt.empty() ? makeCurrentTimestamp() : createdAt),
      isRead(isRead) {}

Teacher::Teacher()
    : teacherId(""),
      name(""),
      email(""),
      department(""),
      phone(""),
      username(""),
      passwordHash(""),
      status("Active") {}

Teacher::Teacher(const std::string& teacherId, const std::string& name,
                 const std::string& email, const std::string& department,
                 const std::string& phone, const std::string& username,
                 const std::string& password)
    : teacherId(teacherId),
      name(name),
      email(email),
      department(department),
      phone(phone),
      username(username),
      passwordHash(hashPassword(username, password)),
      status("Active") {}

std::string Teacher::currentTimestamp() {
    return makeCurrentTimestamp();
}

std::string Teacher::hashPassword(const std::string& username, const std::string& password) {
    return sha256Hex("teacher-management-system|" + username + "|" + password);
}

bool Teacher::login(const std::string& username, const std::string& password) const {
    return this->username == username && this->passwordHash == hashPassword(username, password);
}

std::string Teacher::getTeacherId() const { return teacherId; }
void Teacher::setTeacherId(const std::string& teacherId) { this->teacherId = teacherId; }

std::string Teacher::getName() const { return name; }
void Teacher::setName(const std::string& name) { this->name = name; }

std::string Teacher::getEmail() const { return email; }
void Teacher::setEmail(const std::string& email) { this->email = email; }

std::string Teacher::getDepartment() const { return department; }
void Teacher::setDepartment(const std::string& department) { this->department = department; }

std::string Teacher::getPhone() const { return phone; }
void Teacher::setPhone(const std::string& phone) { this->phone = phone; }

std::string Teacher::getUsername() const { return username; }
void Teacher::setUsername(const std::string& username) { this->username = username; }

std::string Teacher::getPassword() const { return passwordHash; }
void Teacher::setPassword(const std::string& password) { this->passwordHash = hashPassword(username, password); }

std::string Teacher::getPasswordHash() const { return passwordHash; }
void Teacher::setPasswordHash(const std::string& passwordHash) { this->passwordHash = passwordHash; }

std::string Teacher::getStatus() const { return status; }
void Teacher::setStatus(const std::string& status) { this->status = status; }

void Teacher::printPersonalInfo() const {
    std::cout << "Teacher ID: " << teacherId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Department: " << department << std::endl;
    std::cout << "Phone: " << phone << std::endl;
    std::cout << "Status: " << status << std::endl;
}

void Teacher::updatePersonalInfo(const std::string& name, const std::string& email,
                                 const std::string& department, const std::string& phone) {
    this->name = name;
    this->email = email;
    this->department = department;
    this->phone = phone;
    std::cout << "Thong tin ca nhan da duoc cap nhat." << std::endl;
}

void Teacher::printTeacherCard() const {
    std::cout << "===== TEACHER CARD =====" << std::endl;
    std::cout << "ID: " << teacherId << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Department: " << department << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Phone: " << phone << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Status: " << status << std::endl;
}

void Teacher::addAssignedCourse(const CourseInfo& course) {
    assignedCourses.push_back(course);
}

std::vector<CourseInfo> Teacher::getAssignedCourses() const {
    return assignedCourses;
}

void Teacher::printAssignedCourses() const {
    std::cout << "Danh sach khoa hoc giao day:" << std::endl;
    for (const auto& course : assignedCourses) {
        std::cout << "  Ma mon: " << course.courseId
                  << " | Ten mon: " << course.courseName
                  << " | Hoc ky: " << course.semester
                  << " | Lop: " << course.classId << std::endl;
    }
}

void Teacher::printTeachingClasses() const {
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

void Teacher::addStudentToClass(const std::string& classId, const StudentInfo& student) {
    classStudents[classId].push_back(student);
}

std::vector<StudentInfo> Teacher::getStudentsInClass(const std::string& classId) const {
    auto it = classStudents.find(classId);
    if (it == classStudents.end()) return {};
    return it->second;
}

void Teacher::printStudentsInClass(const std::string& classId) const {
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

void Teacher::addScheduleEntry(const ScheduleEntry& entry) {
    schedule.push_back(entry);
}

std::vector<ScheduleEntry> Teacher::getSchedule() const {
    return schedule;
}

void Teacher::printSchedule() const {
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
                       const std::string& assessment, double score) {
    gradebook[courseId].emplace_back(studentId, assessment, score);
}

bool Teacher::editScore(const std::string& courseId, const std::string& studentId,
                        const std::string& assessment, double newScore) {
    auto it = gradebook.find(courseId);
    if (it == gradebook.end()) return false;
    for (auto& record : it->second) {
        if (record.studentId == studentId && record.assessment == assessment) {
            record.score = newScore;
            record.updatedAt = currentTimestamp();
            return true;
        }
    }
    return false;
}

bool Teacher::deleteScore(const std::string& courseId, const std::string& studentId,
                          const std::string& assessment) {
    auto it = gradebook.find(courseId);
    if (it == gradebook.end()) return false;
    auto& records = it->second;
    for (auto recordIt = records.begin(); recordIt != records.end(); ++recordIt) {
        if (recordIt->studentId == studentId && recordIt->assessment == assessment) {
            records.erase(recordIt);
            return true;
        }
    }
    return false;
}

void Teacher::printGradebook(const std::string& courseId) const {
    std::cout << "Bang diem mon " << courseId << ":" << std::endl;
    auto it = gradebook.find(courseId);
    if (it == gradebook.end() || it->second.empty()) {
        std::cout << "  Khong co diem." << std::endl;
        return;
    }
    for (const auto& record : it->second) {
        std::cout << "  Ma SV: " << record.studentId
                  << " | Hinh thuc: " << record.assessment
                  << " | Diem: " << record.score
                  << " | Cap nhat: " << record.updatedAt << std::endl;
    }
}

void Teacher::addCourseContent(const std::string& courseId, const std::string& content) {
    courseContent[courseId].push_back(content);
}

void Teacher::printCourseContent(const std::string& courseId) const {
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

void Teacher::addNotification(const std::string& message) {
    notifications.emplace_back(message);
}

std::vector<TeacherNotification> Teacher::getNotifications() const {
    return notifications;
}

void Teacher::printNotifications() const {
    std::cout << "Thong bao tu nha truong:" << std::endl;
    if (notifications.empty()) {
        std::cout << "  Khong co thong bao." << std::endl;
        return;
    }
    for (const auto& notification : notifications) {
        std::cout << "  - " << notification.message
                  << " | CreatedAt: " << notification.createdAt
                  << " | IsRead: " << (notification.isRead ? "Yes" : "No") << std::endl;
    }
}
