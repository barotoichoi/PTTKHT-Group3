#include "Student.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <sqlext.h>

using std::cout;
using std::ostream;
using std::string;
using std::vector;

namespace {
bool checkSqlSuccess(SQLRETURN result) {
    return result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO;
}

string getDiagnosticMessage(SQLSMALLINT handleType, SQLHANDLE handle) {
    std::ostringstream details;
    SQLSMALLINT record = 1;

    while (true) {
        SQLCHAR state[6] = {};
        SQLCHAR message[SQL_MAX_MESSAGE_LENGTH] = {};
        SQLINTEGER nativeError = 0;
        SQLSMALLINT messageLength = 0;

        SQLRETURN result = SQLGetDiagRecA(handleType, handle, record, state, &nativeError,
                                          message, sizeof(message), &messageLength);
        if (result == SQL_NO_DATA) break;
        if (!checkSqlSuccess(result)) break;

        if (details.tellp() > 0) details << " | ";
        details << "[" << state << "] " << message;
        ++record;
    }

    const string text = details.str();
    return text.empty() ? "Unknown SQL Server error" : text;
}

string nullableDateSql(const string& value) {
    if (value.empty()) return "NULL";
    return "'" + value + "'";
}

string readColumnData(SQLHSTMT statement, SQLUSMALLINT column) {
    string value;
    SQLLEN indicator = 0;

    while (true) {
        char buffer[1024] = {};
        SQLRETURN result = SQLGetData(statement, column, SQL_C_CHAR, buffer, sizeof(buffer), &indicator);
        if (result == SQL_NO_DATA) break;
        if (!checkSqlSuccess(result)) break;
        if (indicator == SQL_NULL_DATA) return "";

        value += buffer;
        if (result == SQL_SUCCESS) break;
    }

    return value;
}

uint32_t rotateRight(uint32_t value, uint32_t bits) {
    return (value >> bits) | (value << (32 - bits));
}

string sha256Hex(const string& input) {
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

    vector<uint8_t> bytes(input.begin(), input.end());
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
}

Student::Student()
    : studentId(""), name(""), gender(""), dob(""), classId(""), email(""),
      major(""), phone(""), username(""), password(""), gpa(0.0),
      tuitionOwed(0.0), status("Active") {}

Student::Student(const string& studentId,
                 const string& name,
                 const string& gender,
                 const string& dob,
                 const string& classId,
                 const string& email,
                 const string& major,
                 const string& phone,
                 const string& username,
                 const string& password,
                 double gpa,
                 double tuitionOwed,
                 const string& status)
    : studentId(studentId), name(name), gender(gender), dob(dob), classId(classId),
      email(email), major(major), phone(phone), username(username),
      password(password), gpa(gpa), tuitionOwed(tuitionOwed), status(status) {}

bool Student::login(const string& username, const string& password) const {
    return this->username == username && this->password == password;
}

void Student::displayInfo() const {
    cout << "Student ID : " << studentId << '\n';
    cout << "Name       : " << name << '\n';
    cout << "Gender     : " << gender << '\n';
    cout << "DOB        : " << dob << '\n';
    cout << "Class ID   : " << classId << '\n';
    cout << "Email      : " << email << '\n';
    cout << "Major      : " << major << '\n';
    cout << "Phone      : " << phone << '\n';
    cout << "Username   : " << username << '\n';
    cout << "GPA        : " << gpa << '\n';
    cout << "Tuition    : " << tuitionOwed << '\n';
    cout << "Status     : " << status << '\n';
}

StudentRepository::StudentRepository(const string& connectionString)
    : environmentHandle(nullptr), connectionHandle(nullptr),
      connectionString(connectionString) {}

StudentRepository::~StudentRepository() {
    disconnect();
}

string StudentRepository::defaultConnectionString() {
    return "Driver={ODBC Driver 17 for SQL Server};"
           "Server=localhost;"
           "Trusted_Connection=yes;"
           "Encrypt=no;"
           "TrustServerCertificate=yes;";
}

bool StudentRepository::connect() {
    if (connectionHandle != nullptr) return true;

    SQLHENV env = nullptr;
    SQLHDBC dbc = nullptr;

    SQLRETURN result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    if (!checkSqlSuccess(result)) return false;

    result = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 0);
    if (!checkSqlSuccess(result)) {
        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return false;
    }

    result = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    if (!checkSqlSuccess(result)) {
        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return false;
    }

    SQLCHAR outConnectionString[1024] = {};
    SQLSMALLINT outLength = 0;
    result = SQLDriverConnectA(
        dbc,
        nullptr,
        reinterpret_cast<SQLCHAR*>(connectionString.data()),
        SQL_NTS,
        outConnectionString,
        sizeof(outConnectionString),
        &outLength,
        SQL_DRIVER_NOPROMPT
    );

    if (!checkSqlSuccess(result)) {
        cout << "SQL Server connect failed: " << getDiagnosticMessage(SQL_HANDLE_DBC, dbc) << '\n';
        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return false;
    }

    environmentHandle = env;
    connectionHandle = dbc;
    return true;
}

void StudentRepository::disconnect() {
    if (connectionHandle != nullptr) {
        SQLDisconnect(static_cast<SQLHDBC>(connectionHandle));
        SQLFreeHandle(SQL_HANDLE_DBC, static_cast<SQLHDBC>(connectionHandle));
        connectionHandle = nullptr;
    }

    if (environmentHandle != nullptr) {
        SQLFreeHandle(SQL_HANDLE_ENV, static_cast<SQLHENV>(environmentHandle));
        environmentHandle = nullptr;
    }
}

bool StudentRepository::initializeSchema() {
    if (!executeNonQuery("IF DB_ID('StudentManagement') IS NULL CREATE DATABASE StudentManagement;")) return false;

    const char* statements[] = {
        "IF OBJECT_ID('StudentManagement.dbo.Students', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Students ("
        "StudentID NVARCHAR(20) NOT NULL PRIMARY KEY,"
        "Name NVARCHAR(100) NOT NULL,"
        "Gender NVARCHAR(20) NULL,"
        "DOB DATE NULL,"
        "ClassID NVARCHAR(20) NULL,"
        "Email NVARCHAR(100) NULL,"
        "Major NVARCHAR(100) NULL,"
        "Phone NVARCHAR(20) NULL,"
        "Username NVARCHAR(50) NOT NULL UNIQUE,"
        "PasswordHash NVARCHAR(64) NOT NULL,"
        "GPA FLOAT NOT NULL DEFAULT 0,"
        "TuitionOwed FLOAT NOT NULL DEFAULT 0,"
        "Status NVARCHAR(30) NULL"
        ");",

        "IF COL_LENGTH('StudentManagement.dbo.Students','Major') IS NULL "
        "ALTER TABLE StudentManagement.dbo.Students ADD Major NVARCHAR(100) NULL;",

        "IF COL_LENGTH('StudentManagement.dbo.Students','Username') IS NULL "
        "ALTER TABLE StudentManagement.dbo.Students ADD Username NVARCHAR(50) NULL;",

        "IF COL_LENGTH('StudentManagement.dbo.Students','PasswordHash') IS NULL "
        "ALTER TABLE StudentManagement.dbo.Students ADD PasswordHash NVARCHAR(64) NULL;",

        "IF COL_LENGTH('StudentManagement.dbo.Students','Password') IS NOT NULL "
        "EXEC('UPDATE StudentManagement.dbo.Students SET [Password]=NULL;');",

        "IF COL_LENGTH('StudentManagement.dbo.Students','GPA') IS NULL "
        "ALTER TABLE StudentManagement.dbo.Students ADD GPA FLOAT NOT NULL DEFAULT 0;",

        "IF COL_LENGTH('StudentManagement.dbo.Students','TuitionOwed') IS NULL "
        "ALTER TABLE StudentManagement.dbo.Students ADD TuitionOwed FLOAT NOT NULL DEFAULT 0;",

        "IF OBJECT_ID('StudentManagement.dbo.Courses', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Courses ("
        "CourseID NVARCHAR(20) NOT NULL PRIMARY KEY,"
        "CourseName NVARCHAR(120) NOT NULL,"
        "Credits INT NOT NULL,"
        "TeacherName NVARCHAR(100) NULL,"
        "Room NVARCHAR(30) NULL,"
        "Semester NVARCHAR(30) NOT NULL,"
        "Status NVARCHAR(30) NOT NULL DEFAULT 'Open'"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.Enrollments', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Enrollments ("
        "EnrollmentID INT IDENTITY(1,1) PRIMARY KEY,"
        "StudentID NVARCHAR(20) NOT NULL,"
        "CourseID NVARCHAR(20) NOT NULL,"
        "Semester NVARCHAR(30) NOT NULL,"
        "Status NVARCHAR(30) NOT NULL DEFAULT 'Enrolled',"
        "CreatedAt DATETIME NOT NULL DEFAULT GETDATE(),"
        "CONSTRAINT UQ_StudentCourseSemester UNIQUE(StudentID, CourseID, Semester)"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.Schedules', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Schedules ("
        "ScheduleID NVARCHAR(20) NOT NULL PRIMARY KEY,"
        "CourseID NVARCHAR(20) NOT NULL,"
        "DayOfWeek NVARCHAR(20) NOT NULL,"
        "StartTime NVARCHAR(10) NOT NULL,"
        "EndTime NVARCHAR(10) NOT NULL,"
        "Room NVARCHAR(30) NULL,"
        "Semester NVARCHAR(30) NOT NULL"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.Scores', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Scores ("
        "ScoreID INT IDENTITY(1,1) PRIMARY KEY,"
        "StudentID NVARCHAR(20) NOT NULL,"
        "CourseID NVARCHAR(20) NOT NULL,"
        "Semester NVARCHAR(30) NOT NULL,"
        "Assignment FLOAT NOT NULL DEFAULT 0,"
        "Midterm FLOAT NOT NULL DEFAULT 0,"
        "Final FLOAT NOT NULL DEFAULT 0,"
        "Average FLOAT NOT NULL DEFAULT 0,"
        "Grade NVARCHAR(5) NULL,"
        "CONSTRAINT UQ_StudentScore UNIQUE(StudentID, CourseID, Semester)"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.CourseMaterials', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.CourseMaterials ("
        "MaterialID NVARCHAR(20) NOT NULL PRIMARY KEY,"
        "CourseID NVARCHAR(20) NOT NULL,"
        "Title NVARCHAR(150) NOT NULL,"
        "Url NVARCHAR(255) NULL,"
        "CreatedAt DATETIME NOT NULL DEFAULT GETDATE()"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.Notifications', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Notifications ("
        "NotificationID NVARCHAR(20) NOT NULL PRIMARY KEY,"
        "StudentID NVARCHAR(20) NULL,"
        "Title NVARCHAR(120) NOT NULL,"
        "Content NVARCHAR(500) NOT NULL,"
        "CreatedAt DATETIME NOT NULL DEFAULT GETDATE(),"
        "IsRead BIT NOT NULL DEFAULT 0,"
        "Type NVARCHAR(30) NOT NULL DEFAULT 'General'"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.Tuitions', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Tuitions ("
        "TuitionID NVARCHAR(20) NOT NULL PRIMARY KEY,"
        "StudentID NVARCHAR(20) NOT NULL,"
        "Semester NVARCHAR(30) NOT NULL,"
        "TotalCredits INT NOT NULL,"
        "Amount FLOAT NOT NULL,"
        "PaidAmount FLOAT NOT NULL DEFAULT 0,"
        "DueDate DATE NULL,"
        "Status NVARCHAR(30) NOT NULL DEFAULT 'Unpaid'"
        ");",

        "IF OBJECT_ID('StudentManagement.dbo.Payments', 'U') IS NULL "
        "CREATE TABLE StudentManagement.dbo.Payments ("
        "PaymentID INT IDENTITY(1,1) PRIMARY KEY,"
        "TuitionID NVARCHAR(20) NOT NULL,"
        "Amount FLOAT NOT NULL,"
        "PaidAt DATETIME NOT NULL DEFAULT GETDATE(),"
        "Method NVARCHAR(30) NOT NULL DEFAULT 'Cash'"
        ");"
    };

    for (const char* statement : statements) {
        if (!executeNonQuery(statement)) return false;
    }

    if (!addForeignKeyIfMissing("StudentManagement.dbo.Enrollments", "FK_Enrollments_Students",
                                "FOREIGN KEY(StudentID) REFERENCES StudentManagement.dbo.Students(StudentID)")) return false;
    if (!addForeignKeyIfMissing("StudentManagement.dbo.Enrollments", "FK_Enrollments_Courses",
                                "FOREIGN KEY(CourseID) REFERENCES StudentManagement.dbo.Courses(CourseID)")) return false;
    if (!addForeignKeyIfMissing("StudentManagement.dbo.Scores", "FK_Scores_Students",
                                "FOREIGN KEY(StudentID) REFERENCES StudentManagement.dbo.Students(StudentID)")) return false;
    if (!addForeignKeyIfMissing("StudentManagement.dbo.Scores", "FK_Scores_Courses",
                                "FOREIGN KEY(CourseID) REFERENCES StudentManagement.dbo.Courses(CourseID)")) return false;
    if (!addForeignKeyIfMissing("StudentManagement.dbo.Schedules", "FK_Schedules_Courses",
                                "FOREIGN KEY(CourseID) REFERENCES StudentManagement.dbo.Courses(CourseID)")) return false;
    if (!addForeignKeyIfMissing("StudentManagement.dbo.Tuitions", "FK_Tuitions_Students",
                                "FOREIGN KEY(StudentID) REFERENCES StudentManagement.dbo.Students(StudentID)")) return false;
    if (!addForeignKeyIfMissing("StudentManagement.dbo.Payments", "FK_Payments_Tuitions",
                                "FOREIGN KEY(TuitionID) REFERENCES StudentManagement.dbo.Tuitions(TuitionID)")) return false;

    return true;
}

bool StudentRepository::seedSampleData() {
    cout << "Student sample data is managed by SQL scripts, not hardcoded in C++.\n";
    return true;
}

bool StudentRepository::addStudent(const Student& student) {
    std::ostringstream sql;
    sql << "INSERT INTO StudentManagement.dbo.Students "
        << "(StudentID,Name,Gender,DOB,ClassID,Email,Major,Phone,Username,PasswordHash,GPA,TuitionOwed,Status) VALUES ("
        << toSqlString(student.studentId) << ","
        << toSqlString(student.name) << ","
        << toSqlString(student.gender) << ","
        << nullableDateSql(escapeSql(student.dob)) << ","
        << toSqlString(student.classId) << ","
        << toSqlString(student.email) << ","
        << toSqlString(student.major) << ","
        << toSqlString(student.phone) << ","
        << toSqlString(student.username) << ","
        << toSqlString(hashPassword(student.username, student.password)) << ","
        << student.gpa << ","
        << student.tuitionOwed << ","
        << toSqlString(student.status) << ");";
    return executeNonQuery(sql.str());
}

bool StudentRepository::updateStudent(const Student& student) {
    std::ostringstream sql;
    sql << "UPDATE StudentManagement.dbo.Students SET "
        << "Name=" << toSqlString(student.name) << ","
        << "Gender=" << toSqlString(student.gender) << ","
        << "DOB=" << nullableDateSql(escapeSql(student.dob)) << ","
        << "ClassID=" << toSqlString(student.classId) << ","
        << "Email=" << toSqlString(student.email) << ","
        << "Major=" << toSqlString(student.major) << ","
        << "Phone=" << toSqlString(student.phone) << ","
        << "Username=" << toSqlString(student.username) << ",";
    if (!student.password.empty()) {
        sql << "PasswordHash=" << toSqlString(hashPassword(student.username, student.password)) << ",";
    }
    sql
        << "GPA=" << student.gpa << ","
        << "TuitionOwed=" << student.tuitionOwed << ","
        << "Status=" << toSqlString(student.status) << " "
        << "WHERE StudentID=" << toSqlString(student.studentId) << ";";
    return executeNonQuery(sql.str());
}

bool StudentRepository::updatePersonalInfo(const string& studentId,
                                           const string& email,
                                           const string& phone,
                                           const string& password) {
    std::ostringstream sql;
    sql << "UPDATE StudentManagement.dbo.Students SET "
        << "Email=" << toSqlString(email) << ","
        << "Phone=" << toSqlString(phone);
    if (!password.empty()) {
        vector<vector<string>> rows = executeQuery(
            "SELECT Username FROM StudentManagement.dbo.Students WHERE StudentID=" + toSqlString(studentId) + ";", 1);
        if (rows.empty()) return false;
        sql << ",PasswordHash=" << toSqlString(hashPassword(rows.front()[0], password));
    }
    sql << " WHERE StudentID=" << toSqlString(studentId) << ";";
    return executeNonQuery(sql.str());
}

bool StudentRepository::deleteStudent(const string& studentId) {
    return executeNonQuery("DELETE FROM StudentManagement.dbo.Students WHERE StudentID=" + toSqlString(studentId) + ";");
}

bool StudentRepository::findStudentById(const string& studentId, Student& student) {
    vector<vector<string>> rows = executeQuery(
        "SELECT StudentID,Name,Gender,CONVERT(VARCHAR(10),DOB,23),ClassID,Email,Major,Phone,Username,GPA,TuitionOwed,Status "
        "FROM StudentManagement.dbo.Students WHERE StudentID=" + toSqlString(studentId) + ";", 12);
    if (rows.empty()) return false;

    const vector<string>& r = rows.front();
    student = Student(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], "",
                      toDouble(r[9]), toDouble(r[10]), r[11]);
    return true;
}

bool StudentRepository::login(const string& username, const string& password, Student& student) {
    vector<vector<string>> rows = executeQuery(
        "SELECT StudentID,PasswordHash FROM StudentManagement.dbo.Students WHERE Username=" + toSqlString(username) + ";", 2);
    if (rows.empty()) return false;
    if (rows.front()[1] != hashPassword(username, password)) return false;
    return findStudentById(rows.front()[0], student);
}

vector<Student> StudentRepository::getAllStudents() {
    vector<Student> students;
    vector<vector<string>> rows = executeQuery(
        "SELECT StudentID,Name,Gender,CONVERT(VARCHAR(10),DOB,23),ClassID,Email,Major,Phone,Username,GPA,TuitionOwed,Status "
        "FROM StudentManagement.dbo.Students ORDER BY StudentID;", 12);
    for (const vector<string>& r : rows) {
        students.emplace_back(r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8], "",
                              toDouble(r[9]), toDouble(r[10]), r[11]);
    }
    return students;
}

vector<StudentCourse> StudentRepository::getAvailableCourses(const string& semester) {
    vector<StudentCourse> courses;
    vector<vector<string>> rows = executeQuery(
        "SELECT CourseID,CourseName,Credits,TeacherName,Room,Semester,Status "
        "FROM StudentManagement.dbo.Courses WHERE Semester=" + toSqlString(semester) +
        " AND Status='Open' ORDER BY CourseID;", 7);
    for (const vector<string>& r : rows) {
        courses.push_back({r[0], r[1], toInt(r[2]), r[3], r[4], r[5], r[6]});
    }
    return courses;
}

vector<StudentCourse> StudentRepository::getCurrentCourses(const string& studentId, const string& semester) {
    vector<StudentCourse> courses;
    vector<vector<string>> rows = executeQuery(
        "SELECT c.CourseID,c.CourseName,c.Credits,c.TeacherName,c.Room,e.Semester,e.Status "
        "FROM StudentManagement.dbo.Enrollments e "
        "JOIN StudentManagement.dbo.Courses c ON c.CourseID=e.CourseID "
        "WHERE e.StudentID=" + toSqlString(studentId) + " AND e.Semester=" + toSqlString(semester) +
        " AND e.Status='Enrolled' ORDER BY c.CourseID;", 7);
    for (const vector<string>& r : rows) {
        courses.push_back({r[0], r[1], toInt(r[2]), r[3], r[4], r[5], r[6]});
    }
    return courses;
}

vector<StudentSchedule> StudentRepository::getSchedule(const string& studentId, const string& semester) {
    vector<StudentSchedule> schedules;
    vector<vector<string>> rows = executeQuery(
        "SELECT s.ScheduleID,s.CourseID,c.CourseName,s.DayOfWeek,s.StartTime,s.EndTime,s.Room "
        "FROM StudentManagement.dbo.Schedules s "
        "JOIN StudentManagement.dbo.Courses c ON c.CourseID=s.CourseID "
        "JOIN StudentManagement.dbo.Enrollments e ON e.CourseID=s.CourseID AND e.Semester=s.Semester "
        "WHERE e.StudentID=" + toSqlString(studentId) + " AND e.Semester=" + toSqlString(semester) +
        " AND e.Status='Enrolled' ORDER BY s.DayOfWeek,s.StartTime;", 7);
    for (const vector<string>& r : rows) {
        schedules.push_back({r[0], r[1], r[2], r[3], r[4], r[5], r[6]});
    }
    return schedules;
}

vector<StudentScore> StudentRepository::getScores(const string& studentId, const string& semester) {
    vector<StudentScore> scores;
    vector<vector<string>> rows = executeQuery(
        "SELECT sc.CourseID,c.CourseName,sc.Assignment,sc.Midterm,sc.Final,sc.Average,sc.Grade "
        "FROM StudentManagement.dbo.Scores sc "
        "JOIN StudentManagement.dbo.Courses c ON c.CourseID=sc.CourseID "
        "WHERE sc.StudentID=" + toSqlString(studentId) + " AND sc.Semester=" + toSqlString(semester) +
        " ORDER BY sc.CourseID;", 7);
    for (const vector<string>& r : rows) {
        scores.push_back({r[0], r[1], toDouble(r[2]), toDouble(r[3]), toDouble(r[4]), toDouble(r[5]), r[6]});
    }
    return scores;
}

vector<CourseMaterial> StudentRepository::getNewCourseMaterials(const string& studentId) {
    vector<CourseMaterial> materials;
    vector<vector<string>> rows = executeQuery(
        "SELECT m.MaterialID,m.CourseID,c.CourseName,m.Title,m.Url,CONVERT(VARCHAR(19),m.CreatedAt,120) "
        "FROM StudentManagement.dbo.CourseMaterials m "
        "JOIN StudentManagement.dbo.Courses c ON c.CourseID=m.CourseID "
        "JOIN StudentManagement.dbo.Enrollments e ON e.CourseID=m.CourseID "
        "WHERE e.StudentID=" + toSqlString(studentId) + " AND e.Status='Enrolled' "
        "ORDER BY m.CreatedAt DESC;", 6);
    for (const vector<string>& r : rows) {
        materials.push_back({r[0], r[1], r[2], r[3], r[4], r[5]});
    }
    return materials;
}

vector<StudentNotification> StudentRepository::getTuitionNotifications(const string& studentId) {
    vector<StudentNotification> notifications;
    vector<vector<string>> rows = executeQuery(
        "SELECT NotificationID,ISNULL(StudentID,''),Title,Content,CONVERT(VARCHAR(19),CreatedAt,120),IsRead "
        "FROM StudentManagement.dbo.Notifications "
        "WHERE Type='Tuition' AND (StudentID IS NULL OR StudentID=" + toSqlString(studentId) + ") "
        "ORDER BY CreatedAt DESC;", 6);
    for (const vector<string>& r : rows) {
        notifications.push_back({r[0], r[1], r[2], r[3], r[4], r[5] == "1"});
    }
    return notifications;
}

bool StudentRepository::enrollCourse(const string& studentId, const string& courseId, const string& semester) {
    if (!exists("SELECT 1 FROM StudentManagement.dbo.Students WHERE StudentID=" + toSqlString(studentId) + ";")) {
        cout << "Enroll failed: student does not exist.\n";
        return false;
    }
    if (!exists("SELECT 1 FROM StudentManagement.dbo.Courses WHERE CourseID=" + toSqlString(courseId) + ";")) {
        cout << "Enroll failed: course does not exist.\n";
        return false;
    }

    std::ostringstream sql;
    sql << "MERGE StudentManagement.dbo.Enrollments WITH (HOLDLOCK) AS target "
        << "USING (SELECT " << toSqlString(studentId) << " AS StudentID, "
        << toSqlString(courseId) << " AS CourseID, " << toSqlString(semester) << " AS Semester) AS source "
        << "ON target.StudentID=source.StudentID AND target.CourseID=source.CourseID AND target.Semester=source.Semester "
        << "WHEN MATCHED THEN UPDATE SET Status='Enrolled' "
        << "WHEN NOT MATCHED THEN INSERT (StudentID,CourseID,Semester,Status) "
        << "VALUES (source.StudentID,source.CourseID,source.Semester,'Enrolled');";
    return executeInTransaction(sql.str());
}

bool StudentRepository::cancelEnrollment(const string& studentId, const string& courseId, const string& semester) {
    return executeNonQuery(
        "UPDATE StudentManagement.dbo.Enrollments SET Status='Cancelled' WHERE StudentID=" + toSqlString(studentId) +
        " AND CourseID=" + toSqlString(courseId) + " AND Semester=" + toSqlString(semester) + ";");
}

bool StudentRepository::printTuitionStatement(const string& studentId, const string& semester, ostream& output) {
    vector<vector<string>> rows = executeQuery(
        "SELECT TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,CONVERT(VARCHAR(10),DueDate,23),Status "
        "FROM StudentManagement.dbo.Tuitions WHERE StudentID=" + toSqlString(studentId) +
        " AND Semester=" + toSqlString(semester) + ";", 8);
    if (rows.empty()) return false;

    const vector<string>& r = rows.front();
    output << "===== TUITION STATEMENT =====\n";
    output << "Tuition ID : " << r[0] << '\n';
    output << "Student ID : " << r[1] << '\n';
    output << "Semester   : " << r[2] << '\n';
    output << "Credits    : " << r[3] << '\n';
    output << "Amount     : " << r[4] << '\n';
    output << "Paid       : " << r[5] << '\n';
    output << "Due date   : " << r[6] << '\n';
    output << "Status     : " << r[7] << '\n';
    return true;
}

vector<StudentTuition> StudentRepository::getTuitionRecords(const string& studentId) {
    vector<StudentTuition> tuitions;
    vector<vector<string>> rows = executeQuery(
        "SELECT TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,CONVERT(VARCHAR(10),DueDate,23),Status "
        "FROM StudentManagement.dbo.Tuitions WHERE StudentID=" + toSqlString(studentId) +
        " ORDER BY Semester DESC;", 8);
    for (const vector<string>& r : rows) {
        tuitions.push_back({r[0], r[1], r[2], toInt(r[3]), toDouble(r[4]), toDouble(r[5]), r[6], r[7]});
    }
    return tuitions;
}

vector<StudentTuition> StudentRepository::getUnpaidTuitionRecords(const string& studentId) {
    vector<StudentTuition> tuitions;
    vector<vector<string>> rows = executeQuery(
        "SELECT TuitionID,StudentID,Semester,TotalCredits,Amount,PaidAmount,CONVERT(VARCHAR(10),DueDate,23),Status "
        "FROM StudentManagement.dbo.Tuitions WHERE StudentID=" + toSqlString(studentId) +
        " AND Status <> 'Paid' ORDER BY DueDate;", 8);
    for (const vector<string>& r : rows) {
        tuitions.push_back({r[0], r[1], r[2], toInt(r[3]), toDouble(r[4]), toDouble(r[5]), r[6], r[7]});
    }
    return tuitions;
}

bool StudentRepository::isTuitionOwnedByStudent(const string& studentId, const string& tuitionId) {
    return exists("SELECT 1 FROM StudentManagement.dbo.Tuitions WHERE StudentID=" +
                  toSqlString(studentId) + " AND TuitionID=" + toSqlString(tuitionId) + ";");
}

bool StudentRepository::payTuition(const string& tuitionId, double amount) {
    if (amount <= 0) {
        cout << "Payment failed: amount must be greater than zero.\n";
        return false;
    }
    if (!exists("SELECT 1 FROM StudentManagement.dbo.Tuitions WHERE TuitionID=" + toSqlString(tuitionId) + ";")) {
        cout << "Payment failed: tuition record does not exist.\n";
        return false;
    }

    std::ostringstream sql;
    sql << "INSERT INTO StudentManagement.dbo.Payments (TuitionID,Amount,Method) VALUES ("
        << toSqlString(tuitionId) << "," << amount << ",'Cash'); "
        << "UPDATE StudentManagement.dbo.Tuitions SET PaidAmount=PaidAmount+" << amount
        << ", Status=CASE WHEN PaidAmount+" << amount << " >= Amount THEN 'Paid' ELSE 'Partial' END "
        << "WHERE TuitionID=" << toSqlString(tuitionId) << ";";
    return executeInTransaction(sql.str());
}

bool StudentRepository::executeNonQuery(const string& sql) {
    if (!connect()) return false;

    SQLHSTMT statement = nullptr;
    SQLRETURN result = SQLAllocHandle(SQL_HANDLE_STMT, static_cast<SQLHDBC>(connectionHandle), &statement);
    if (!checkSqlSuccess(result)) return false;

    result = SQLExecDirectA(statement, reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.c_str())), SQL_NTS);
    if (!checkSqlSuccess(result)) {
        cout << "SQL command failed: " << getDiagnosticMessage(SQL_HANDLE_STMT, statement) << '\n';
        cout << "Failed SQL: " << sql << '\n';
        SQLFreeHandle(SQL_HANDLE_STMT, statement);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return true;
}

bool StudentRepository::executeInTransaction(const string& sql) {
    if (!connect()) return false;

    SQLHDBC dbc = static_cast<SQLHDBC>(connectionHandle);
    SQLRETURN result = SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT,
                                         reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF), 0);
    if (!checkSqlSuccess(result)) {
        cout << "Cannot start SQL transaction: " << getDiagnosticMessage(SQL_HANDLE_DBC, dbc) << '\n';
        return false;
    }

    const bool ok = executeNonQuery(sql);
    if (ok) {
        result = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_COMMIT);
    } else {
        result = SQLEndTran(SQL_HANDLE_DBC, dbc, SQL_ROLLBACK);
    }

    SQLSetConnectAttr(dbc, SQL_ATTR_AUTOCOMMIT,
                      reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON), 0);

    if (!checkSqlSuccess(result)) {
        cout << "SQL transaction finish failed: " << getDiagnosticMessage(SQL_HANDLE_DBC, dbc) << '\n';
        return false;
    }

    return ok;
}

vector<vector<string>> StudentRepository::executeQuery(const string& sql, int columnCount) {
    vector<vector<string>> rows;
    if (!connect()) return rows;

    SQLHSTMT statement = nullptr;
    SQLRETURN result = SQLAllocHandle(SQL_HANDLE_STMT, static_cast<SQLHDBC>(connectionHandle), &statement);
    if (!checkSqlSuccess(result)) return rows;

    result = SQLExecDirectA(statement, reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql.c_str())), SQL_NTS);
    if (!checkSqlSuccess(result)) {
        cout << "SQL query failed: " << getDiagnosticMessage(SQL_HANDLE_STMT, statement) << '\n';
        cout << "Failed SQL: " << sql << '\n';
        SQLFreeHandle(SQL_HANDLE_STMT, statement);
        return rows;
    }

    while (checkSqlSuccess(SQLFetch(statement))) {
        vector<string> row;
        for (SQLUSMALLINT column = 1; column <= static_cast<SQLUSMALLINT>(columnCount); ++column) {
            row.push_back(readColumnData(statement, column));
        }
        rows.push_back(row);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, statement);
    return rows;
}

bool StudentRepository::exists(const string& sql) {
    return !executeQuery(sql, 1).empty();
}

bool StudentRepository::addForeignKeyIfMissing(const string& table,
                                               const string& constraint,
                                               const string& definition) {
    std::ostringstream sql;
    sql << "IF NOT EXISTS (SELECT 1 FROM StudentManagement.sys.foreign_keys WHERE name="
        << toSqlString(constraint) << ") "
        << "ALTER TABLE " << table << " WITH NOCHECK ADD CONSTRAINT "
        << constraint << " " << definition << ";";
    return executeNonQuery(sql.str());
}

string StudentRepository::hashPassword(const string& username, const string& password) {
    return sha256Hex("student-management-system|" + username + "|" + password);
}

string StudentRepository::escapeSql(const string& value) {
    string escaped;
    escaped.reserve(value.size());
    for (char ch : value) {
        escaped += ch;
        if (ch == '\'') escaped += '\'';
    }
    return escaped;
}

string StudentRepository::toSqlString(const string& value) {
    return "N'" + escapeSql(value) + "'";
}

double StudentRepository::toDouble(const string& value) {
    if (value.empty()) return 0.0;
    return std::stod(value);
}

int StudentRepository::toInt(const string& value) {
    if (value.empty()) return 0;
    return std::stoi(value);
}
