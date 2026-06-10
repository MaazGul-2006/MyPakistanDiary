#pragma once
#include <string>
#include <vector>
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
};