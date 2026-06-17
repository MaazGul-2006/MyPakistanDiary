#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "TravelEntry.h"
#include "Database.h"
#include "PreTripCheck.h"
#include "HTMLExporter.h"

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
  enum class Screen {
    LIST,
    DETAIL,
    ADD_ENTRY,
    QUESTIONNAIRE,
    SUPPORT_MESSAGE,
    EXPORT_COMPLETE
};
    Screen m_currentScreen;

    // ── Add Entry form state ──────────────────────────────────
int         m_activeField;    // which field is focused
std::string m_inputPlace;
std::string m_inputCity;
std::string m_inputDate;
std::string m_inputDesc;
std::string m_inputCategory;
std::string m_inputRating;
std::string m_inputMood;
std::string m_inputStatus;
std::string m_inputPhoto;

// ── Pre-trip check state ──────────────────────────────────
PreTripCheck m_check;
int          m_questionStep;  // 0-3, which question we're on

    // ── Colors ───────────────────────────────────────────────
    sf::Color m_bgColor;
    sf::Color m_cardColor;
    sf::Color m_cardHover;
    sf::Color m_accentColor;
    sf::Color m_textPrimary;
    sf::Color m_textSecondary;
    // ── Export ──────────────────────────────────── 
    HTMLExporter* m_exporter = nullptr;

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
    void drawAddEntryScreen();
    void drawExportCompleteScreen();
void drawQuestionnaireScreen();
void drawSupportScreen();
void handleAddEntryInput(sf::Event& event);
void handleQuestionnaireInput(sf::Event& event);
void clearForm();
bool saveFormEntry();

public:
    DiaryApp(Database& db);
    ~DiaryApp() ;
    void run();
};