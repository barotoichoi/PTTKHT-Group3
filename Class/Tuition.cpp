#include "Tuition.h"
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

    bool createTuitionTable(sqlite3* db)
    {
        std::string query = "CREATE TABLE IF NOT EXISTS Tuition ("
                            "studentId TEXT, "
                            "semester TEXT, "
                            "totalCredits INTEGER, "
                            "tuitionFee REAL, "
                            "paymentStatus TEXT, "
                            "unitPrice REAL, "
                            "PRIMARY KEY(studentId, semester));";

        char* errorMessage = nullptr;
        int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);
        if (result != SQLITE_OK)
        {
            std::cerr << "Failed to create Tuition table: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            return false;
        }

        return true;
    }

    bool executeTuitionSql(const std::string& query)
    {
        sqlite3* db = nullptr;
        int result = sqlite3_open("school.db", &db);
        if (result != SQLITE_OK)
        {
            std::cerr << "Failed to open database." << std::endl;
            sqlite3_close(db);
            return false;
        }

        if (!createTuitionTable(db))
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

    struct TuitionQueryContext
    {
        bool found = false;
        Tuition* record = nullptr;
    };

    int tuitionFindCallback(void* data, int argc, char** argv, char** colNames)
    {
        (void)colNames;
        auto* context = static_cast<TuitionQueryContext*>(data);
        if (!context || !context->record)
        {
            return 0;
        }

        context->found = true;
        context->record->setStudentId(argv[0] ? argv[0] : "");
        context->record->setSemester(argv[1] ? argv[1] : "");
        context->record->setTotalCredits(argv[2] ? std::stoi(argv[2]) : 0);
        context->record->setTuitionFee(argv[3] ? std::stod(argv[3]) : 0.0);
        context->record->setPaymentStatus(argv[4] ? argv[4] : "");
        context->record->setUnitPrice(argv[5] ? std::stod(argv[5]) : 0.0);
        return 0;
    }
}

Tuition::Tuition()
    : studentId(""), semester(""), totalCredits(0), tuitionFee(0.0), paymentStatus("Chưa đóng"), unitPrice(0.0)
{
}

Tuition::Tuition(const std::string& studentId, const std::string& semester, int totalCredits,
                 double unitPrice, const std::string& paymentStatus)
{
    this->studentId = studentId;
    this->semester = semester;
    this->totalCredits = totalCredits;
    this->unitPrice = unitPrice;
    this->paymentStatus = paymentStatus;
    this->tuitionFee = 0.0;
}

Tuition::~Tuition()
{
}

std::string Tuition::getStudentId() const
{
    return studentId;
}

void Tuition::setStudentId(const std::string& studentId)
{
    this->studentId = studentId;
}

std::string Tuition::getSemester() const
{
    return semester;
}

void Tuition::setSemester(const std::string& semester)
{
    this->semester = semester;
}

int Tuition::getTotalCredits() const
{
    return totalCredits;
}

void Tuition::setTotalCredits(int totalCredits)
{
    this->totalCredits = totalCredits;
}

double Tuition::getTuitionFee() const
{
    return tuitionFee;
}

void Tuition::setTuitionFee(double tuitionFee)
{
    this->tuitionFee = tuitionFee;
}

double Tuition::getUnitPrice() const
{
    return unitPrice;
}

void Tuition::setUnitPrice(double unitPrice)
{
    this->unitPrice = unitPrice;
}

std::string Tuition::getPaymentStatus() const
{
    return paymentStatus;
}

void Tuition::setPaymentStatus(const std::string& paymentStatus)
{
    this->paymentStatus = paymentStatus;
}

double Tuition::calculateTuitionFee()
{
    tuitionFee = static_cast<double>(totalCredits) * unitPrice;
    return tuitionFee;
}

void Tuition::updatePaymentStatus(const std::string& paymentStatus)
{
    this->paymentStatus = paymentStatus;
    save();
    std::cout << "Trang thai thanh toan da duoc cap nhat." << std::endl;
}

bool Tuition::save()
{
    calculateTuitionFee();
    std::string query;
    if (find(studentId, semester))
    {
        query = "UPDATE Tuition SET totalCredits = " + std::to_string(totalCredits) + ", tuitionFee = " + std::to_string(tuitionFee) + ", paymentStatus = '" + escapeSql(paymentStatus) + "', unitPrice = " + std::to_string(unitPrice) + " WHERE studentId = '" + escapeSql(studentId) + "' AND semester = '" + escapeSql(semester) + "'";
    }
    else
    {
        query = "INSERT INTO Tuition (studentId, semester, totalCredits, tuitionFee, paymentStatus, unitPrice) VALUES ('" + escapeSql(studentId) + "', '" + escapeSql(semester) + "', " + std::to_string(totalCredits) + ", " + std::to_string(tuitionFee) + ", '" + escapeSql(paymentStatus) + "', " + std::to_string(unitPrice) + ")";
    }

    return executeTuitionSql(query);
}

bool Tuition::deleteRecord()
{
    std::string query = "DELETE FROM Tuition WHERE studentId = '" + escapeSql(studentId) + "' AND semester = '" + escapeSql(semester) + "'";
    return executeTuitionSql(query);
}

bool Tuition::find(const std::string& studentId, const std::string& semester)
{
    std::string query = "SELECT * FROM Tuition WHERE studentId = '" + escapeSql(studentId) + "' AND semester = '" + escapeSql(semester) + "'";

    sqlite3* db = nullptr;
    int result = sqlite3_open("school.db", &db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to open database." << std::endl;
        sqlite3_close(db);
        return false;
    }

    if (!createTuitionTable(db))
    {
        sqlite3_close(db);
        return false;
    }

    TuitionQueryContext context;
    context.record = this;
    char* errorMessage = nullptr;
    result = sqlite3_exec(db, query.c_str(), tuitionFindCallback, &context, &errorMessage);
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
