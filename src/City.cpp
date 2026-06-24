#include "City.h"

City::City()
    : m_id(0), m_name(""), m_description(""),
      m_bestTime(""), m_safetyInfo(""), m_travelTips(""),
      m_latitude(0.0f), m_longitude(0.0f) {}

City::City(int id, const std::string& name, const std::string& desc,
           const std::string& bestTime, const std::string& safety,
           const std::string& tips, float lat, float lng)
    : m_id(id), m_name(name), m_description(desc),
      m_bestTime(bestTime), m_safetyInfo(safety), m_travelTips(tips),
      m_latitude(lat), m_longitude(lng) {}

int                City::getId()           const { return m_id; }
const std::string& City::getName()         const { return m_name; }
const std::string& City::getDescription()  const { return m_description; }
const std::string& City::getBestTime()     const { return m_bestTime; }
const std::string& City::getSafetyInfo()   const { return m_safetyInfo; }
const std::string& City::getTravelTips()   const { return m_travelTips; }
float              City::getLatitude()      const { return m_latitude; }
float              City::getLongitude()     const { return m_longitude; }

void City::setName(const std::string& name)                { m_name = name; }
void City::setDescription(const std::string& desc)         { m_description = desc; }
void City::setBestTime(const std::string& time)            { m_bestTime = time; }
void City::setSafetyInfo(const std::string& info)          { m_safetyInfo = info; }
void City::setTravelTips(const std::string& tips)          { m_travelTips = tips; }
void City::setCoordinates(float lat, float lng)            { m_latitude = lat; m_longitude = lng; }