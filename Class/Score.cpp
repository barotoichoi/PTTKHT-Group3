#include "Score.h"

// Constructor implementation
Score::Score(std::string sId, std::string eId, float initialScore) {
    scoreId = sId;
    enrollmentId = eId;
    score = initialScore;
}

// Getter implementations
std::string Score::getScoreId() const {
    return scoreId;
}

std::string Score::getEnrollmentId() const {
    return enrollmentId;
}

float Score::getScore() const {
    return score;
}

// Setter implementation
void Score::setScore(float newScore) {
    // Only allow valid scores from 0 to 10
    if (newScore >= 0.0f && newScore <= 10.0f) {
        score = newScore;
    }
}