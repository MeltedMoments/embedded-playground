/**
 * climate.cpp
 */
#include <Arduino.h>
#include "climate.h"

// V.Good: 100, Good: 70-99, Okay: 40-69, Bad: 10-39, V.Bad: 0-9
String comfort_text (float comfort) {
    if (comfort >= 100.0) return "V.Good";
    if (comfort >= 70.0)  return "Good  ";
    if (comfort >= 40.0)  return "Okay  ";
    if (comfort >= 10.0)  return "Bad   ";
    return "V.Bad ";
}

float comfort_score(float temperature, float humidity) {
    float score = 100.0;     // best possible
    // Penalise the score for every PENALTY away from the ideal, 
    // for both temp and humidity
    score -= abs(temperature - IDEAL_TEMPERATURE) * TEMPERATURE_PENALTY;
    score -= abs(humidity - IDEAL_HUMIDITY) * HUMIDITY_PENALTY;

    score = std::max(0.0F, score);
    score = std::min(100.0F, score);
    return score;
}
