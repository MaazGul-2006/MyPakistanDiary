#pragma once
#include <string>
#include <vector>
#include "TravelEntry.h"
#include "Database.h"

// ============================================================
//  HTMLExporter — generate styled HTML travel report
//  Includes personal diary + crowd wisdom insights
//  Muhammad Maaz Gul | My Pakistan Travel Diary
// ============================================================

class HTMLExporter {
private:
    std::string generateCitySection(const City& city, 
                                   const std::vector<TravelEntry>& entries) const;

public:
    bool exportToHTML(const std::vector<TravelEntry>& entries,
                     const std::vector<City>& cities);
private:
    Database& m_db;
    std::string m_outputPath;

    // Helper methods
    std::string getColorForMood(const std::string& mood) const;
    std::string getCategoryEmoji(const std::string& category) const;
    std::string copyPhotoToExport(const std::string& sourcePath) const;

public:
    HTMLExporter(Database& db, const std::string& outputPath = "export/travel_report.html");

    // Main export function
    bool exportToHTML(const std::vector<TravelEntry>& entries);

    // Generate individual sections
    std::string generateHeader() const;
    std::string generateEntryCard(const TravelEntry& entry) const;
    std::string generateCrowdWisdom() const;
    std::string generateFooter() const;
};