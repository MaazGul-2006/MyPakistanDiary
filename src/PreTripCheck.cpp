#include "PreTripCheck.h"

PreTripCheck::PreTripCheck()
    : entryId(0),
      travelingAlone(false),
      feelingScore(1),
      sharedWithSomeone(true),
      energyLevel(2),
      flagged(false) {}

void PreTripCheck::evaluate() {
    // Flag if: alone AND (nervous or need break) AND
    //          nobody knows AND low energy
    flagged = travelingAlone          &&
              (feelingScore == 2 ||
               feelingScore == 3)     &&
              !sharedWithSomeone      &&
              energyLevel == 1;
}

std::string PreTripCheck::getFeelingLabel() const {
    if (feelingScore == 1) return "Excited";
    if (feelingScore == 2) return "Nervous";
    if (feelingScore == 3) return "Need a break";
    return "Unsure";
}

std::string PreTripCheck::getEnergyLabel() const {
    if (energyLevel == 1) return "Low";
    if (energyLevel == 2) return "Medium";
    return "High";
}