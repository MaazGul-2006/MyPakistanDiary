#pragma once
#include <string>
#include <vector>

class City {
private:
    int         m_id;
    std::string m_name;
    std::string m_description;
    std::string m_bestTime;      // "October to March"
    std::string m_safetyInfo;    // "Generally safe, avoid protests"
    std::string m_travelTips;    // "Bring sunscreen, local transport..."
    float       m_latitude;
    float       m_longitude;

public:
    City();
    City(int id, const std::string& name, const std::string& desc,
         const std::string& bestTime, const std::string& safety,
         const std::string& tips, float lat, float lng);

    // Getters
    int                getId()           const;
    const std::string& getName()         const;
    const std::string& getDescription()  const;
    const std::string& getBestTime()     const;
    const std::string& getSafetyInfo()   const;
    const std::string& getTravelTips()   const;
    float              getLatitude()      const;
    float              getLongitude()     const;

    // Setters
    void setName(const std::string& name);
    void setDescription(const std::string& desc);
    void setBestTime(const std::string& time);
    void setSafetyInfo(const std::string& info);
    void setTravelTips(const std::string& tips);
    void setCoordinates(float lat, float lng);
};