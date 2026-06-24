#include <iostream>
#include "Database.h"
#include "DiaryApp.h"

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

    DiaryApp app(db);
    app.run();

    db.close();
    return 0;
}