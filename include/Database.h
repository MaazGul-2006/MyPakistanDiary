#pragma once
#include <string>
#include <vector>
#include "City.h"
#include "TravelEntry.h"
#include <sqlite3.h>

// ============================================================
//  Database — handles all SQLite operations
//  Muhammad Maaz Gul | My Pakistan Travel Diary
// ============================================================

class Database {
private:
    sqlite3*    m_db;       // SQLite connection handle
    std::string m_dbPath;   // path to .db file

    // Internal helper — runs a no-result SQL statement
    bool executeSQL(const std::string& sql);

public:
    // ── Constructor / Destructor ──────────────────────────────
    Database(const std::string& dbPath);
    ~Database();

    // ── Connection ────────────────────────────────────────────
    bool open();
    void close();
    bool isOpen() const;

    // ── Schema ────────────────────────────────────────────────
    bool createTable();

    // ── CRUD ──────────────────────────────────────────────────
    bool                    insertEntry(const TravelEntry& entry);
    bool                    updateEntry(const TravelEntry& entry);
    bool                    deleteEntry(int id);
    std::vector<TravelEntry> getAllEntries();
    TravelEntry              getEntryById(int id);

    // ── Queries ───────────────────────────────────────────────
    std::vector<TravelEntry> getEntriesByMood(const std::string& mood);
    std::vector<TravelEntry> getEntriesByCity(const std::string& city);
    std::vector<TravelEntry> getEntriesByStatus(const std::string& status);

    // ── Crowd wisdom ──────────────────────────────────────────
    float       getAverageRating(const std::string& place);
    int         getVisitCount(const std::string& city);
    std::string getMostCommonMood(const std::string& city);

    // City operations ──────────────────────────────────────────
    bool                insertCity(const City& city);
    City                getCityByName(const std::string& name);
    std::vector<City>   getAllCities();

    public:
    // Add to public section of Database class:

// Video operations
bool insertVideo(const std::string& city, const std::string& youtubeUrl, const std::string& title);
// In Database.h
std::vector<std::pair<std::string, std::string>> getVideosByCity(const std::string& city);

// Media coverage operations
bool insertMediaCoverage(const std::string& city, const std::string& title, 
                        const std::string& url, const std::string& sourceType, 
                        const std::string& sourceName);
std::vector<std::tuple<std::string, std::string, std::string, std::string>> 
getMediaByCity(const std::string& city);
};