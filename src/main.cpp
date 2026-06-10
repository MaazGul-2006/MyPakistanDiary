#include <iostream>
#include <vector>
#include "TravelEntry.h"
#include "Database.h"

int main() {
    std::cout << "======================================\n";
    std::cout << "  My Pakistan — Personal Travel Diary \n";
    std::cout << "  Muhammad Maaz Gul  |  FAST-NUCES Isb\n";
    std::cout << "======================================\n\n";

    // ── Open database ─────────────────────────────────────────
    Database db("database/diary.db");
    if (!db.open()) return 1;
    db.createTable();

    // ── Seed your 8 real entries ──────────────────────────────
    std::vector<TravelEntry> diary = {
        { 0, "Shah Allah Ditta Caves", "Islamabad", "2026-06-06",
          "Ancient Buddhist cave hermitages in the Margalla Hills.",
          "Historical", 5.0f, "Peaceful", "Visited" },
        { 0, "Lal Masjid", "Islamabad", "2026-06-05",
          "The Red Mosque — iconic twin minarets.",
          "Religious", 4.0f, "Reflective", "Visited" },
        { 0, "Taxila Buddhist Site (Julian)", "Rawalpindi", "2026-01-15",
          "UNESCO World Heritage Site. 2000 years of history.",
          "Historical", 5.0f, "Nostalgic", "Visited" },
        { 0, "Daewoo Terminal", "Rawalpindi", "2026-02-10",
          "Gateway to every adventure. Energy of people in transit.",
          "Recreation", 3.0f, "Energized", "Visited" },
        { 0, "Army Museum", "Rawalpindi", "2026-03-20",
          "Fascinating military history of Pakistan.",
          "Historical", 4.0f, "Nostalgic", "Visited" },
        { 0, "Faisal Mosque & Pakistan Monument", "Islamabad", "2025-08-14",
          "Visited on Independence Day eve. Glowing at dusk.",
          "Religious", 5.0f, "Peaceful", "Visited" },
        { 0, "Murree — Mall Road & Chair Lift", "Murree", "2025-07-10",
          "Summer escape. Chair lift gave a stunning valley view.",
          "Nature", 4.0f, "Joyful", "Visited" },
        { 0, "Lahore — Shahi Qila, Minar-e-Pakistan, Joyland",
          "Lahore", "2024-12-25",
          "Epic Lahore trip! Sheesh Mahal was the highlight.",
          "Historical", 5.0f, "Joyful", "Visited" },
    };

    // Insert only if table is empty (avoid duplicates on re-run)
    auto existing = db.getAllEntries();
    if (existing.empty()) {
        std::cout << "Seeding database...\n";
        for (auto& e : diary)
            db.insertEntry(e);
        std::cout << "Inserted " << diary.size() << " entries.\n\n";
    } else {
        std::cout << "Database already has "
                  << existing.size() << " entries.\n\n";
    }

    // ── Read back and display ─────────────────────────────────
    auto entries = db.getAllEntries();
    std::cout << "All entries from database:\n";
    std::cout << "─────────────────────────────────────\n";
    for (const auto& e : entries)
        std::cout << e.toString() << "\n";

    // ── Crowd wisdom demo ─────────────────────────────────────
    std::cout << "\nCrowd Wisdom:\n";
    std::cout << "─────────────────────────────────────\n";
    std::cout << "Islamabad visits : " << db.getVisitCount("Islamabad") << "\n";
    std::cout << "Islamabad mood   : " << db.getMostCommonMood("Islamabad") << "\n";
    std::cout << "Lahore avg rating: " << db.getAverageRating(
        "Lahore — Shahi Qila, Minar-e-Pakistan, Joyland") << "\n";

    db.close();
    std::cout << "\nDay 2 complete. Ready for Day 3 (SFML)!\n";
    return 0;
}