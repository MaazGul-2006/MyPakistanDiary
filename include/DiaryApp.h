#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "TravelEntry.h"
#include "Database.h"

// ============================================================
//  DiaryApp — SFML window and UI controller
//  Muhammad Maaz Gul | My Pakistan Travel Diary
// ============================================================

class DiaryApp {
private:
    // ── Window ───────────────────────────────────────────────
    sf::RenderWindow m_window;
    sf::Font         m_font;

    // ── Data ─────────────────────────────────────────────────
    Database&                 m_db;
    std::vector<TravelEntry>  m_entries;
    int                       m_selectedIndex;
    int                       m_scrollOffset;

    // ── UI State ─────────────────────────────────────────────
    enum class Screen { LIST, DETAIL };
    Screen m_currentScreen;

    // ── Colors ───────────────────────────────────────────────
    sf::Color m_bgColor;
    sf::Color m_cardColor;
    sf::Color m_cardHover;
    sf::Color m_accentColor;
    sf::Color m_textPrimary;
    sf::Color m_textSecondary;

    // ── Private methods ───────────────────────────────────────
    void handleEvents();
    void update();
    void render();

    void drawListScreen();
    void drawDetailScreen();
    void drawHeader();
    void drawEntryCard(const TravelEntry& entry,
                       float x, float y,
                       float width, bool selected);
    void drawCrowdWisdom();

    sf::Color getMoodColor(const std::string& mood);
    std::string wrapText(const std::string& text, int maxChars);

public:
    DiaryApp(Database& db);
    void run();
};