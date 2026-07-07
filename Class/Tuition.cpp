#include "Tuition.h"
#include <iostream>

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
    std::cout << "Trang thai thanh toan da duoc cap nhat." << std::endl;
}
