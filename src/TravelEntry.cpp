#include "TravelEntry.h"
#include <sstream>

// ── Constructors ─────────────────────────────────────────────

TravelEntry::TravelEntry()
    : m_id(0), m_place(""), m_city(""), m_date(""),
      m_description(""), m_category(""), m_rating(0.0f),
      m_mood(""), m_status("Visited"),
      m_photoPath(""), m_isAnonymous(false), m_discomfortFlag(false) {}

TravelEntry::TravelEntry(int id,
                         const std::string& place,
                         const std::string& city,
                         const std::string& date,
                         const std::string& description,
                         const std::string& category,
                         float rating,
                         const std::string& mood,
                         const std::string& status,
                         const std::string& photoPath,
                         bool isAnonymous,
                         bool discomfortFlag)
    : m_id(id), m_place(place), m_city(city), m_date(date),
      m_description(description), m_category(category), m_rating(rating),
      m_mood(mood), m_status(status), m_photoPath(photoPath),
      m_isAnonymous(isAnonymous), m_discomfortFlag(discomfortFlag) {}

// ── Getters ───────────────────────────────────────────────────

int                TravelEntry::getId()             const { return m_id; }
const std::string& TravelEntry::getPlace()          const { return m_place; }
const std::string& TravelEntry::getCity()           const { return m_city; }
const std::string& TravelEntry::getDate()           const { return m_date; }
const std::string& TravelEntry::getDescription()    const { return m_description; }
const std::string& TravelEntry::getCategory()       const { return m_category; }
float              TravelEntry::getRating()         const { return m_rating; }
const std::string& TravelEntry::getMood()           const { return m_mood; }
const std::string& TravelEntry::getStatus()         const { return m_status; }
const std::string& TravelEntry::getPhotoPath()      const { return m_photoPath; }
bool               TravelEntry::getIsAnonymous()    const { return m_isAnonymous; }
bool               TravelEntry::getDiscomfortFlag() const { return m_discomfortFlag; }

// ── Setters ───────────────────────────────────────────────────

void TravelEntry::setId(int id)                        { m_id = id; }
void TravelEntry::setPlace(const std::string& place)   { m_place = place; }
void TravelEntry::setCity(const std::string& city)     { m_city = city; }
void TravelEntry::setDate(const std::string& date)     { m_date = date; }
void TravelEntry::setDescription(const std::string& d) { m_description = d; }
void TravelEntry::setCategory(const std::string& cat)  { m_category = cat; }
void TravelEntry::setRating(float rating)              { m_rating = rating; }
void TravelEntry::setMood(const std::string& mood)     { m_mood = mood; }
void TravelEntry::setStatus(const std::string& status) { m_status = status; }
void TravelEntry::setPhotoPath(const std::string& p)   { m_photoPath = p; }
void TravelEntry::setIsAnonymous(bool val)             { m_isAnonymous = val; }
void TravelEntry::setDiscomfortFlag(bool val)          { m_discomfortFlag = val; }

// ── Utility ───────────────────────────────────────────────────

std::string TravelEntry::toString() const {
    std::ostringstream oss;
    oss << "[" << m_id << "] "
        << m_place       << " | "
        << m_city        << " | "
        << m_date        << " | "
        << m_status      << " | "
        << m_mood        << " | "
        << m_category    << " | "
        << getRatingStars();
    return oss.str();
}

std::string TravelEntry::toHTMLRow() const {
    std::ostringstream oss;
    oss << "  <tr>\n"
        << "    <td>" << m_id             << "</td>\n"
        << "    <td>" << m_place          << "</td>\n"
        << "    <td>" << m_city           << "</td>\n"
        << "    <td>" << m_date           << "</td>\n"
        << "    <td>" << m_status         << "</td>\n"
        << "    <td>" << m_mood           << "</td>\n"
        << "    <td>" << m_category       << "</td>\n"
        << "    <td>" << m_description    << "</td>\n"
        << "    <td>" << getRatingStars() << "</td>\n"
        << "  </tr>\n";
    return oss.str();
}

bool TravelEntry::isValid() const {
    return !m_place.empty()  &&
           !m_city.empty()   &&
           !m_date.empty()   &&
           !m_mood.empty()   &&
           !m_status.empty() &&
           m_rating >= 1.0f  &&
           m_rating <= 5.0f;
}

std::string TravelEntry::getRatingStars() const {
    int full  = static_cast<int>(m_rating);
    int empty = 5 - full;
    std::string stars;
    for (int i = 0; i < full;  ++i) stars += "\u2605"; // ★
    for (int i = 0; i < empty; ++i) stars += "\u2606"; // ☆
    return stars;
}