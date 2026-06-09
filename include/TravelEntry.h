#pragma once
#include <string>

// ============================================================
//  TravelEntry — represents one travel memory in the diary
//  Muhammad Maaz Gul | My Pakistan Travel Diary
// ============================================================

class TravelEntry {
private:
    int         m_id;
    std::string m_place;
    std::string m_city;
    std::string m_date;
    std::string m_description;
    std::string m_category;
    float       m_rating;
    std::string m_mood;           // "Peaceful"|"Joyful"|"Reflective"|"Nostalgic"|"Energized"|"Anxious"
    std::string m_status;         // "Visited" | "Planned"
    std::string m_photoPath;      // local path to JPG/PNG, empty if none
    bool        m_isAnonymous;    // share without name
    bool        m_discomfortFlag; // reported discomfort

public:
    // ── Constructors ─────────────────────────────────────────
    TravelEntry();
    TravelEntry(int id,
                const std::string& place,
                const std::string& city,
                const std::string& date,
                const std::string& description,
                const std::string& category,
                float rating,
                const std::string& mood,
                const std::string& status,
                const std::string& photoPath   = "",
                bool isAnonymous               = false,
                bool discomfortFlag            = false);

    // ── Getters ───────────────────────────────────────────────
    int                getId()             const;
    const std::string& getPlace()          const;
    const std::string& getCity()           const;
    const std::string& getDate()           const;
    const std::string& getDescription()    const;
    const std::string& getCategory()       const;
    float              getRating()         const;
    const std::string& getMood()           const;
    const std::string& getStatus()         const;
    const std::string& getPhotoPath()      const;
    bool               getIsAnonymous()    const;
    bool               getDiscomfortFlag() const;

    // ── Setters ───────────────────────────────────────────────
    void setId(int id);
    void setPlace(const std::string& place);
    void setCity(const std::string& city);
    void setDate(const std::string& date);
    void setDescription(const std::string& description);
    void setCategory(const std::string& category);
    void setRating(float rating);
    void setMood(const std::string& mood);
    void setStatus(const std::string& status);
    void setPhotoPath(const std::string& path);
    void setIsAnonymous(bool val);
    void setDiscomfortFlag(bool val);

    // ── Utility ───────────────────────────────────────────────
    std::string toString()       const;
    std::string toHTMLRow()      const;
    bool        isValid()        const;
    std::string getRatingStars() const;
};