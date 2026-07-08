#ifndef SCORE_H
#define SCORE_H

#include <string>

class Score {
private:
    std::string scoreId;
    std::string enrollmentId; // Linked to the Enrollment table
    float score;

public:
    // Constructor
    Score(std::string sId, std::string eId, float initialScore);

    // Getters
    std::string getScoreId() const;
    std::string getEnrollmentId() const;
    float getScore() const;

    // Setter
    void setScore(float newScore);
};

#endif // SCORE_H