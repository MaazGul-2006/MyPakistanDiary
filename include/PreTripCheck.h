#pragma once
#include <string>

// ============================================================
//  PreTripCheck — wellness questionnaire for planned trips
//  Triggers when user sets status = "Planned"
//  Muhammad Maaz Gul | My Pakistan Travel Diary
// ============================================================

struct PreTripCheck {
    int  entryId;
    bool travelingAlone;      // Q1
    int  feelingScore;        // Q2: 1=Excited 2=Nervous 3=NeedBreak 4=Unsure
    bool sharedWithSomeone;   // Q3
    int  energyLevel;         // Q4: 1=Low 2=Medium 3=High
    bool flagged;             // computed — true if concerning combo

    PreTripCheck();

    // Compute flag based on answers
    void evaluate();

    // Human readable feeling
    std::string getFeelingLabel() const;

    // Human readable energy
    std::string getEnergyLabel() const;
};