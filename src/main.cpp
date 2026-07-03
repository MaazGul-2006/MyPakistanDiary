#include "Database.h"
#include "DiaryApp.h"
#include <iostream>
#include <vector>

int main() {
    Database db("database/diary.db");
    if (!db.open()) return 1;
    db.createTable();

    // ── Seed cities ──────────────────────────────────────
    std::vector<City> cities = {
        { 0, "Islamabad", 
          "Pakistan's capital nestled in the Margalla Hills. Blend of modern architecture and natural beauty.",
          "October to March", 
          "Generally very safe. Well-developed infrastructure.",
          "Visit Faisal Mosque at sunset. Bring hiking gear for Margalla trails.",
          33.7294, 73.1882 },
        
        { 0, "Rawalpindi",
          "Historic city with military heritage and modern infrastructure.",
          "September to April",
          "Safe city with good security. Avoid monsoon season.",
          "Visit Pindi Point, Ayub Park. Try local food in Adiala Road.",
          33.5731, 73.1522 },
        
        { 0, "Taxila",
          "Ancient UNESCO World Heritage Site. Home to Buddhist monasteries and Greco-Buddhist art from 5th century BC.",
          "October to April",
          "Safe archaeological site. Hire guide for better experience.",
          "Wear comfortable shoes, bring water. Visit Julian monastery, Sardar Sarai ruins.",
          33.7456, 72.8089 },
        
        { 0, "Lahore",
          "Cultural heart of Pakistan. Rich Mughal heritage and vibrant modern city.",
          "October to March",
          "Lively atmosphere, busy streets. Use registered taxis.",
          "Explore Shahi Qila (Lahore Fort), Badshahi Mosque, food street.",
          31.5497, 74.3436 },
        
        { 0, "Murree",
          "Hill station 60km north of Islamabad. Cool escape from summer heat.",
          "March to October",
          "Safe hill station, popular family destination. Roads can be steep.",
          "Bring warm clothes. Visit Mall Road, Pindi Point viewpoint.",
          34.1924, 73.4357 },
    };

    // ── Seed travel entries ───────────────────────────────
    std::vector<TravelEntry> entries = {
        { 0, "Shah Allah Ditta Caves", "Islamabad", "2026-06-06",
          "Ancient Buddhist cave hermitages in the Margalla Hills. Peaceful walk through history.",
          "Historical", 5.0f, "Peaceful", "Visited" },

        { 0, "Lal Masjid", "Islamabad", "2026-06-05",
          "The Red Mosque — iconic twin minarets. Visited the library and courtyard.",
          "Religious", 4.0f, "Reflective", "Visited" },

        { 0, "Taxila Buddhist Site (Julian)", "Taxila", "2026-01-15",
          "UNESCO World Heritage Site. Julian monastery ruins — 2000 years of history under open sky.",
          "Historical", 5.0f, "Nostalgic", "Visited" },

        { 0, "Daewoo Terminal", "Rawalpindi", "2026-02-10",
          "Travel hub — the gateway to every adventure. Captured the energy of people in transit.",
          "Recreation", 3.0f, "Energized", "Visited" },

        { 0, "Army Museum", "Rawalpindi", "2026-03-20",
          "Fascinating military history of Pakistan. Tanks, aircraft and medals on display.",
          "Historical", 4.0f, "Nostalgic", "Visited" },

        { 0, "Faisal Mosque & Pakistan Monument", "Islamabad", "2025-08-14",
          "Visited on Independence Day eve. Faisal Mosque glowing at dusk, Monument lit in green and white.",
          "Religious", 5.0f, "Peaceful", "Visited" },

        { 0, "Murree — Mall Road & Chair Lift", "Murree", "2025-07-10",
          "Summer escape to the hills. Mall Road buzzing with tourists, chair lift gave a stunning valley view.",
          "Nature", 4.0f, "Joyful", "Visited" },

        { 0, "Lahore — Shahi Qila, Minar-e-Pakistan, Joyland",
          "Lahore", "2024-12-25",
          "Epic Lahore trip! Sheesh Mahal was the highlight. Joyland Top Spin ride was a rush.",
          "Historical", 5.0f, "Joyful", "Visited" },
    };

    // ── Insert cities ────────────────────────────────────
    auto existingCities = db.getAllCities();
    if (existingCities.empty()) {
        std::cout << "Seeding city guides...\n";
        for (auto& c : cities) {
            db.insertCity(c);
        }
    }

    // ── Insert entries ───────────────────────────────────
    auto existingEntries = db.getAllEntries();
    if (existingEntries.empty()) {
        std::cout << "Seeding travel entries...\n";
        for (auto& e : entries) {
            db.insertEntry(e);
        }
        std::cout << "Inserted " << entries.size() << " entries and " 
                  << cities.size() << " city guides.\n";
    }

    // ── Seed Videos ────────────────────────────────────────
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> videos = {
        {"Islamabad", {"https://www.youtube.com/embed/dQw4w9WgXcQ", "Faisal Mosque - Pakistan's Most Beautiful Mosque"}},
        {"Islamabad", {"https://www.youtube.com/embed/jNQXAC9IVRw", "Margalla Hills Hiking Adventure - Nature's Paradise"}},
        {"Taxila", {"https://www.youtube.com/embed/9bZkp7q19f0", "Ancient Buddhist Ruins - UNESCO World Heritage Site"}},
        {"Taxila", {"https://www.youtube.com/embed/wIZpJw0W8gE", "2000 Years of History in Taxila"}},
        {"Lahore", {"https://www.youtube.com/embed/AJ8xQaMFpA0", "Lahore Fort - Mughal Architecture Masterpiece"}},
        {"Lahore", {"https://www.youtube.com/embed/e4Jx5OhLKAc", "Food Street Lahore - Street Food Paradise"}},
        {"Murree", {"https://www.youtube.com/embed/L3ww94j1hHg", "Murree Hill Station - Summer Escape"}},
        {"Rawalpindi", {"https://www.youtube.com/embed/TzRGdwTDdJ8", "Rawalpindi Military History Tour"}},
    };

    // Insert videos if table is empty
    auto videosCount = db.getVideosByCity("Islamabad").size();
    if (videosCount == 0) {
        std::cout << "Seeding videos...\n";
        for (const auto& v : videos) {
            db.insertVideo(v.first, v.second.first, v.second.second);
        }
    }

    // ── Seed Media Coverage ────────────────────────────────
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string>> mediaItems = {
        // Islamabad Media
        {"Islamabad", "Faisal Mosque: The World's Most Beautiful Mosque", 
         "https://www.dawn.com/news/islamabad-faisal-mosque", "News", "Dawn News"},
        {"Islamabad", "Pakistan's Hidden Gems - Travel Magazine Feature", 
         "https://www.lonelyplanet.com/pakistan/islamabad", "Magazine", "Lonely Planet"},
        {"Islamabad", "Vlog: 48 Hours in Islamabad - Must Visit Places", 
         "https://www.youtube.com/results?search_query=islamabad+travel+vlog", "Vlogger", "Travel Vloggers"},
        
        // Taxila Media
        {"Taxila", "Ancient Silk Road: Taxila's Buddhist Heritage", 
         "https://www.archaeology.org/taxila-discoveries", "Magazine", "Archaeology Magazine"},
        {"Taxila", "UNESCO World Heritage: Taxila Buddhist Complex", 
         "https://www.bbc.com/travel/article/taxila-pakistan", "News", "BBC Travel"},
        {"Taxila", "2000 Years of History in One Place - Taxila Vlog", 
         "https://www.youtube.com/results?search_query=taxila+buddhist+ruins", "Vlogger", "History Explorers"},
        
        // Lahore Media
        {"Lahore", "Lahore Fort: Pakistan's Mughal Masterpiece", 
         "https://www.nationalgeographic.com/travel/article/lahore-fort", "Magazine", "National Geographic"},
        {"Lahore", "Lahore Food Street Named Best in Asia", 
         "https://www.timeoutlahore.com/food/lahore-food-street", "News", "Timeout Lahore"},
        {"Lahore", "Complete Lahore Guide: Forts, Food, Culture", 
         "https://www.youtube.com/results?search_query=lahore+complete+guide+vlog", "Vlogger", "Travel Guides"},
        
        // Murree Media
        {"Murree", "Hill Stations of Pakistan: Travel Guide", 
         "https://www.travelguide.com/murree-hill-station", "Magazine", "Travel Guide"},
        {"Murree", "Summer in Murree: Perfect Hill Station Escape", 
         "https://www.express.com.pk/epaper/murree-tourism", "News", "Express Tribune"},
        {"Murree", "Murree Vlog: Chair Lift, Mall Road & Nature", 
         "https://www.youtube.com/results?search_query=murree+hill+station+vlog", "Vlogger", "Adventure Vlogs"},
        
        // Rawalpindi Media
        {"Rawalpindi", "Rawalpindi: The City of Soldiers - Military History", 
         "https://www.historicpakistan.com/rawalpindi", "Magazine", "Historic Pakistan"},
        {"Rawalpindi", "Rawalpindi Army Museum: Pakistan's Military Heritage", 
         "https://www.dawn.com/news/rawalpindi-army-museum", "News", "Dawn News"},
        {"Rawalpindi", "Rawalpindi Military History & Heritage Sites Tour", 
         "https://www.youtube.com/results?search_query=rawalpindi+military+tour", "Vlogger", "History Tours"},
    };

    // Insert media if table is empty
    auto mediaCount = db.getMediaByCity("Islamabad").size();
    if (mediaCount == 0) {
        std::cout << "Seeding media coverage...\n";
        for (const auto& m : mediaItems) {
            db.insertMediaCoverage(std::get<0>(m), std::get<1>(m), std::get<2>(m), 
                                   std::get<3>(m), std::get<4>(m));
        }
    }

    std::cout << "\n✅ V3 Seeding Complete!\n\n";

    DiaryApp app(db);
    app.run();

    db.close();
    return 0;
}