#include <iostream>
#include <vector>
#include "TravelEntry.h"

int main() {

    std::cout << "======================================\n";
    std::cout << "  My Pakistan — Personal Travel Diary \n";
    std::cout << "  Muhammad Maaz Gul  |  FAST-NUCES Isb\n";
    std::cout << "======================================\n\n";

    std::vector<TravelEntry> diary = {

        { 1, "Shah Allah Ditta Caves", "Islamabad", "2026-06-06",
          "Ancient Buddhist cave hermitages in the Margalla Hills. Peaceful walk through history.",
          "Historical", 5.0f, "Peaceful", "Visited" },

        { 2, "Lal Masjid", "Islamabad", "2026-06-05",
          "The Red Mosque — iconic twin minarets. Visited the library and courtyard.",
          "Religious", 4.0f, "Reflective", "Visited" },

        { 3, "Taxila Buddhist Site (Julian)", "Rawalpindi", "2026-01-15",
          "UNESCO World Heritage Site. Julian monastery ruins — 2000 years of history under open sky.",
          "Historical", 5.0f, "Nostalgic", "Visited" },

        { 4, "Daewoo Terminal", "Rawalpindi", "2026-02-10",
          "Travel hub — the gateway to every adventure. Captured the energy of people in transit.",
          "Recreation", 3.0f, "Energized", "Visited" },

        { 5, "Army Museum", "Rawalpindi", "2026-03-20",
          "Fascinating military history of Pakistan. Tanks, aircraft and medals on display.",
          "Historical", 4.0f, "Nostalgic", "Visited" },

        { 6, "Faisal Mosque & Pakistan Monument", "Islamabad", "2025-08-14",
          "Visited on Independence Day eve. Faisal Mosque glowing at dusk, Monument lit in green and white.",
          "Religious", 5.0f, "Peaceful", "Visited" },

        { 7, "Murree — Mall Road & Chair Lift", "Murree", "2025-07-10",
          "Summer escape to the hills. Mall Road buzzing with tourists, chair lift gave a stunning valley view.",
          "Nature", 4.0f, "Joyful", "Visited" },

        { 8, "Lahore — Shahi Qila, Minar-e-Pakistan, Army Museum, Joyland",
          "Lahore", "2024-12-25",
          "Epic Lahore trip! Sheesh Mahal was the highlight. Joyland Top Spin ride was a rush.",
          "Historical", 5.0f, "Joyful", "Visited" },
    };

    // ── Print all entries ─────────────────────────────────────
    std::cout << "Loaded " << diary.size() << " travel entries:\n";
    std::cout << "────────────────────────────────────────────────\n\n";

    for (const auto& entry : diary) {
        std::cout << entry.toString() << "\n";
        std::cout << "  " << entry.getDescription() << "\n\n";
    }

    // ── Validation ────────────────────────────────────────────
    std::cout << "────────────────────────────────────────────────\n";
    std::cout << "Validation:\n";
    for (const auto& entry : diary) {
        std::cout << "  " << entry.getPlace()
                  << " → " << (entry.isValid() ? "✓ OK" : "✗ INVALID") << "\n";
    }

    // ── Mood summary ──────────────────────────────────────────
    std::cout << "\nMood summary:\n";
    std::vector<std::string> moods = {"Peaceful","Joyful","Reflective",
                                      "Nostalgic","Energized","Anxious"};
    for (const auto& mood : moods) {
        int count = 0;
        for (const auto& entry : diary)
            if (entry.getMood() == mood) count++;
        if (count > 0)
            std::cout << "  " << mood << ": " << count << " trip(s)\n";
    }

    std::cout << "\nDay 1 complete. Ready for Day 2 (SQLite)!\n";
    return 0;
}