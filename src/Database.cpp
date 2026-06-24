#include "Database.h"
#include <iostream>

// ── Constructor / Destructor ──────────────────────────────────

Database::Database(const std::string& dbPath)
    : m_db(nullptr), m_dbPath(dbPath) {}

Database::~Database() {
    close();
}

// ── Connection ────────────────────────────────────────────────

bool Database::open() {
    int result = sqlite3_open(m_dbPath.c_str(), &m_db);
    if (result != SQLITE_OK) {
        std::cerr << "Cannot open database: "
                  << sqlite3_errmsg(m_db) << "\n";
        return false;
    }
    std::cout << "Database opened: " << m_dbPath << "\n";
    return true;
}

void Database::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool Database::isOpen() const {
    return m_db != nullptr;
}

// ── Internal helper ───────────────────────────────────────────

bool Database::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int result = sqlite3_exec(m_db, sql.c_str(),
                              nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// ── Schema ────────────────────────────────────────────────────

bool Database::createTable() {
    // Existing travel_entries table
    std::string sql1 = R"(
        CREATE TABLE IF NOT EXISTS travel_entries (
            id              INTEGER PRIMARY KEY AUTOINCREMENT,
            place           TEXT    NOT NULL,
            city            TEXT    NOT NULL,
            date            TEXT    NOT NULL,
            description     TEXT,
            category        TEXT,
            rating          REAL    DEFAULT 3.0,
            mood            TEXT,
            status          TEXT    DEFAULT 'Visited',
            photo_path      TEXT    DEFAULT '',
            is_anonymous    INTEGER DEFAULT 0,
            discomfort_flag INTEGER DEFAULT 0
        );
    )";
    
    // New cities table
    std::string sql2 = R"(
        CREATE TABLE IF NOT EXISTS cities (
            id              INTEGER PRIMARY KEY AUTOINCREMENT,
            name            TEXT    UNIQUE NOT NULL,
            description     TEXT,
            best_time       TEXT,
            safety_info     TEXT,
            travel_tips     TEXT,
            latitude        REAL,
            longitude       REAL
        );
    )";
    
    if (!executeSQL(sql1)) return false;
    if (!executeSQL(sql2)) return false;
    
    std::cout << "Tables ready.\n";
    return true;
}

// ── INSERT ────────────────────────────────────────────────────

bool Database::insertEntry(const TravelEntry& entry) {
    std::string sql = R"(
        INSERT INTO travel_entries
        (place, city, date, description, category,
         rating, mood, status, photo_path,
         is_anonymous, discomfort_flag)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1,  entry.getPlace().c_str(),       -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2,  entry.getCity().c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3,  entry.getDate().c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4,  entry.getDescription().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5,  entry.getCategory().c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, entry.getRating());
    sqlite3_bind_text(stmt, 7,  entry.getMood().c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8,  entry.getStatus().c_str(),      -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9,  entry.getPhotoPath().c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt,  10, entry.getIsAnonymous()    ? 1 : 0);
    sqlite3_bind_int(stmt,  11, entry.getDiscomfortFlag() ? 1 : 0);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

// ── UPDATE ────────────────────────────────────────────────────

bool Database::updateEntry(const TravelEntry& entry) {
    std::string sql = R"(
        UPDATE travel_entries SET
            place           = ?,
            city            = ?,
            date            = ?,
            description     = ?,
            category        = ?,
            rating          = ?,
            mood            = ?,
            status          = ?,
            photo_path      = ?,
            is_anonymous    = ?,
            discomfort_flag = ?
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1,  entry.getPlace().c_str(),       -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2,  entry.getCity().c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3,  entry.getDate().c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4,  entry.getDescription().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5,  entry.getCategory().c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, entry.getRating());
    sqlite3_bind_text(stmt, 7,  entry.getMood().c_str(),        -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8,  entry.getStatus().c_str(),      -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9,  entry.getPhotoPath().c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt,  10, entry.getIsAnonymous()    ? 1 : 0);
    sqlite3_bind_int(stmt,  11, entry.getDiscomfortFlag() ? 1 : 0);
    sqlite3_bind_int(stmt,  12, entry.getId());

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

// ── DELETE ────────────────────────────────────────────────────

bool Database::deleteEntry(int id) {
    std::string sql = "DELETE FROM travel_entries WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return result == SQLITE_DONE;
}

// ── Helper: row → TravelEntry ─────────────────────────────────

static TravelEntry rowToEntry(sqlite3_stmt* stmt) {
    auto getText = [&](int col) -> std::string {
        const char* val = reinterpret_cast<const char*>(
                          sqlite3_column_text(stmt, col));
        return val ? val : "";
    };
    return TravelEntry(
        sqlite3_column_int(stmt,    0),   // id
        getText(1),                        // place
        getText(2),                        // city
        getText(3),                        // date
        getText(4),                        // description
        getText(5),                        // category
        (float)sqlite3_column_double(stmt, 6), // rating
        getText(7),                        // mood
        getText(8),                        // status
        getText(9),                        // photoPath
        sqlite3_column_int(stmt,   10) == 1,   // isAnonymous
        sqlite3_column_int(stmt,   11) == 1    // discomfortFlag
    );
}

// ── GET ALL ───────────────────────────────────────────────────

std::vector<TravelEntry> Database::getAllEntries() {
    std::vector<TravelEntry> entries;
    std::string sql = "SELECT * FROM travel_entries ORDER BY date DESC;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW)
        entries.push_back(rowToEntry(stmt));
    sqlite3_finalize(stmt);
    return entries;
}

// ── GET BY ID ─────────────────────────────────────────────────

TravelEntry Database::getEntryById(int id) {
    std::string sql = "SELECT * FROM travel_entries WHERE id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    TravelEntry entry;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        entry = rowToEntry(stmt);
    sqlite3_finalize(stmt);
    return entry;
}

// ── FILTER QUERIES ────────────────────────────────────────────

std::vector<TravelEntry> Database::getEntriesByMood(const std::string& mood) {
    std::vector<TravelEntry> entries;
    std::string sql = "SELECT * FROM travel_entries WHERE mood = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, mood.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW)
        entries.push_back(rowToEntry(stmt));
    sqlite3_finalize(stmt);
    return entries;
}

std::vector<TravelEntry> Database::getEntriesByCity(const std::string& city) {
    std::vector<TravelEntry> entries;
    std::string sql = "SELECT * FROM travel_entries WHERE city = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, city.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW)
        entries.push_back(rowToEntry(stmt));
    sqlite3_finalize(stmt);
    return entries;
}

std::vector<TravelEntry> Database::getEntriesByStatus(const std::string& status) {
    std::vector<TravelEntry> entries;
    std::string sql = "SELECT * FROM travel_entries WHERE status = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW)
        entries.push_back(rowToEntry(stmt));
    sqlite3_finalize(stmt);
    return entries;
}

// ── CROWD WISDOM ──────────────────────────────────────────────

float Database::getAverageRating(const std::string& place) {
    std::string sql = "SELECT AVG(rating) FROM travel_entries WHERE place = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, place.c_str(), -1, SQLITE_STATIC);
    float avg = 0.0f;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        avg = (float)sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return avg;
}

int Database::getVisitCount(const std::string& city) {
    std::string sql = "SELECT COUNT(*) FROM travel_entries WHERE city = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, city.c_str(), -1, SQLITE_STATIC);
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count;
}

std::string Database::getMostCommonMood(const std::string& city) {
    std::string sql = R"(
        SELECT mood, COUNT(*) as cnt
        FROM travel_entries
        WHERE city = ?
        GROUP BY mood
        ORDER BY cnt DESC
        LIMIT 1;
    )";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, city.c_str(), -1, SQLITE_STATIC);
    std::string mood = "Unknown";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* val = reinterpret_cast<const char*>(
                          sqlite3_column_text(stmt, 0));
        if (val) mood = val;
    }
    sqlite3_finalize(stmt);
    return mood;
}

bool Database::insertCity(const City& city) {
    std::string sql = R"(
        INSERT OR REPLACE INTO cities (name, description, best_time, safety_info, travel_tips, latitude, longitude)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )";
    
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, city.getName().c_str(),         -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, city.getDescription().c_str(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, city.getBestTime().c_str(),     -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, city.getSafetyInfo().c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, city.getTravelTips().c_str(),   -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, city.getLatitude());
    sqlite3_bind_double(stmt, 7, city.getLongitude());
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}

City Database::getCityByName(const std::string& name) {
    std::string sql = "SELECT * FROM cities WHERE name = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    
    City city;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        city = City(
            sqlite3_column_int(stmt,    0),  // id
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),  // name
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))),  // description
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),  // bestTime
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))),  // safetyInfo
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))),  // travelTips
            (float)sqlite3_column_double(stmt, 6),  // latitude
            (float)sqlite3_column_double(stmt, 7)   // longitude
        );
    }
    sqlite3_finalize(stmt);
    return city;
}

std::vector<City> Database::getAllCities() {
    std::vector<City> cities;
    std::string sql = "SELECT * FROM cities ORDER BY name;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        City city(
            sqlite3_column_int(stmt,    0),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))),
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))),
            (float)sqlite3_column_double(stmt, 6),
            (float)sqlite3_column_double(stmt, 7)
        );
        cities.push_back(city);
    }
    sqlite3_finalize(stmt);
    return cities;
}