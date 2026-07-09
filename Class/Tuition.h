#ifndef TUITION_H
#define TUITION_H

#include <string>

class Tuition
{
private:
    std::string studentId;
    std::string semester;
    int totalCredits;
    double tuitionFee;
    std::string paymentStatus;
    double unitPrice;

public:
    Tuition();
    Tuition(const std::string& studentId, const std::string& semester, int totalCredits,
            double unitPrice, const std::string& paymentStatus);
    ~Tuition();

    std::string getStudentId() const;
    void setStudentId(const std::string& studentId);

    std::string getSemester() const;
    void setSemester(const std::string& semester);

    int getTotalCredits() const;
    void setTotalCredits(int totalCredits);

    double getTuitionFee() const;
    void setTuitionFee(double tuitionFee);

    double getUnitPrice() const;
    void setUnitPrice(double unitPrice);

    std::string getPaymentStatus() const;
    void setPaymentStatus(const std::string& paymentStatus);

    double calculateTuitionFee();
    void updatePaymentStatus(const std::string& paymentStatus);

    bool save();
    bool deleteRecord();
    bool find(const std::string& studentId, const std::string& semester);
};

#endif
