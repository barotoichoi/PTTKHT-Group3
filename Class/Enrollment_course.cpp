#include "Enrollment_course.h"
#include <iostream>
#include <sqlite3.h>
#include <string>

namespace
{
    std::string escapeSql(const std::string& value)
    {
        std::string result = value;
        std::size_t pos = 0;
        while ((pos = result.find("'", pos)) != std::string::npos)
        {
            result.insert(pos, 1, '\'');
            pos += 2;
        }
        return result;
    }

    bool createEnrollmentTable(sqlite3* db)
    {
        std::string query = "CREATE TABLE IF NOT EXISTS Enrollment_course ("
                            "studentId TEXT, "
                            "courseId TEXT, "
                            "semester TEXT, "
                            "status TEXT, "
                            "PRIMARY KEY(studentId, courseId, semester));";

        char* errorMessage = nullptr;
        int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
        if (result != SQLITE_OK)
        {
            std::cerr << "Failed to create Enrollment_course table: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            return false;
        }

        return true;
    }

    bool executeEnrollmentSql(const std::string& query)
    {
        sqlite3* db = nullptr;
        int result = sqlite3_open("school.db", &db);
        if (result != SQLITE_OK)
        {
            std::cerr << "Failed to open database." << std::endl;
            sqlite3_close(db);
            return false;
        }

        if (!createEnrollmentTable(db))
        {
            sqlite3_close(db);
            return false;
        }

        char* errorMessage = nullptr;
        result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
        if (result != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            sqlite3_close(db);
            return false;
        }

        sqlite3_close(db);
        return true;
    }

    struct EnrollmentQueryContext
    {
        bool found = false;
        Enrollment_course* record = nullptr;
    };

    int enrollmentFindCallback(void* data, int argc, char** argv, char** colNames)
    {
        (void)colNames;
        auto* context = static_cast<EnrollmentQueryContext*>(data);
        if (!context || !context->record)
        {
            return 0;
        }

        context->found = true;
        context->record->setStudentId(argv[0] ? argv[0] : "");
        context->record->setCourseId(argv[1] ? argv[1] : "");
        context->record->setSemester(argv[2] ? argv[2] : "");
        context->record->setStatus(argv[3] ? argv[3] : "");
        return 0;
    }
}

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
    save();
    std::cout << "Dang ky mon hoc thanh cong." << std::endl;
}

void Enrollment_course::cancelCourse()
{
    status = "Hủy";
    save();
    std::cout << "Da huy mon hoc." << std::endl;
}

bool Enrollment_course::save()
{
    std::string query;
    if (find(studentId, courseId, semester))
    {
        query = "UPDATE Enrollment_course SET status = '" + escapeSql(status) + "' WHERE studentId = '" + escapeSql(studentId) + "' AND courseId = '" + escapeSql(courseId) + "' AND semester = '" + escapeSql(semester) + "'";
    }
    else
    {
        query = "INSERT INTO Enrollment_course (studentId, courseId, semester, status) VALUES ('" + escapeSql(studentId) + "', '" + escapeSql(courseId) + "', '" + escapeSql(semester) + "', '" + escapeSql(status) + "')";
    }

    return executeEnrollmentSql(query);
}

bool Enrollment_course::deleteRecord()
{
    std::string query = "DELETE FROM Enrollment_course WHERE studentId = '" + escapeSql(studentId) + "' AND courseId = '" + escapeSql(courseId) + "' AND semester = '" + escapeSql(semester) + "'";
    return executeEnrollmentSql(query);
}

bool Enrollment_course::find(const std::string& studentId, const std::string& courseId, const std::string& semester)
{
    std::string query = "SELECT * FROM Enrollment_course WHERE studentId = '" + escapeSql(studentId) + "' AND courseId = '" + escapeSql(courseId) + "' AND semester = '" + escapeSql(semester) + "'";

    sqlite3* db = nullptr;
    int result = sqlite3_open("school.db", &db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to open database." << std::endl;
        sqlite3_close(db);
        return false;
    }

    if (!createEnrollmentTable(db))
    {
        sqlite3_close(db);
        return false;
    }

    EnrollmentQueryContext context;
    context.record = this;
    char* errorMessage = nullptr;
    result = sqlite3_exec(db, query.c_str(), enrollmentFindCallback, &context, &errorMessage);
    if (result != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return context.found;
}
