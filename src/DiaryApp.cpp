#include "DiaryApp.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ── Constructor ───────────────────────────────────────────────

DiaryApp::DiaryApp(Database& db)
    : m_window(sf::VideoMode(900, 650), "My Pakistan Travel Diary",
               sf::Style::Titlebar | sf::Style::Close),
      m_db(db),
      m_selectedIndex(0),
      m_scrollOffset(0),
      m_currentScreen(Screen::LIST)
{
    m_window.setFramerateLimit(60);

    // Load font
    if (!m_font.loadFromFile("assets/fonts/DejaVuSans.ttf")) {
        std::cerr << "Font not found!\n";
    }

    // Colour palette — dark elegant theme
    m_bgColor       = sf::Color(18,  18,  24);   // near black
    m_cardColor     = sf::Color(30,  30,  42);   // dark card
    m_cardHover     = sf::Color(42,  42,  60);   // hovered card
    m_accentColor   = sf::Color(0,   168, 107);  // Pakistan green
    m_textPrimary   = sf::Color(240, 240, 240);
    m_textSecondary = sf::Color(160, 160, 180);

    // Load entries from DB
    m_entries = m_db.getAllEntries();
}

// ── Main loop ─────────────────────────────────────────────────

void DiaryApp::run() {
    while (m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// ── Events ────────────────────────────────────────────────────

void DiaryApp::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            m_window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (m_currentScreen == Screen::LIST) {
                if (event.key.code == sf::Keyboard::Down) {
                    if (m_selectedIndex < (int)m_entries.size() - 1)
                        m_selectedIndex++;
                    if (m_selectedIndex - m_scrollOffset > 4)
                        m_scrollOffset++;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    if (m_selectedIndex > 0)
                        m_selectedIndex--;
                    if (m_selectedIndex < m_scrollOffset)
                        m_scrollOffset--;
                }
                if (event.key.code == sf::Keyboard::Return)
                    m_currentScreen = Screen::DETAIL;
                if (event.key.code == sf::Keyboard::Escape)
                    m_window.close();
            }
            else if (m_currentScreen == Screen::DETAIL) {
                if (event.key.code == sf::Keyboard::Escape ||
                    event.key.code == sf::Keyboard::BackSpace)
                    m_currentScreen = Screen::LIST;
            }
        }

        // Mouse click on card
        if (event.type == sf::Event::MouseButtonPressed &&
            m_currentScreen == Screen::LIST) {
            float mouseY = (float)event.mouseButton.y;
            float cardStart = 120.f;
            float cardH     = 90.f;
            float gap       = 10.f;
            for (int i = 0; i < (int)m_entries.size(); i++) {
                int visIdx = i - m_scrollOffset;
                if (visIdx < 0 || visIdx > 4) continue;
                float y = cardStart + visIdx * (cardH + gap);
                if (mouseY >= y && mouseY <= y + cardH) {
                    m_selectedIndex = i;
                    m_currentScreen = Screen::DETAIL;
                    break;
                }
            }
        }
    }
}

void DiaryApp::update() {}

// ── Render ────────────────────────────────────────────────────

void DiaryApp::render() {
    m_window.clear(m_bgColor);

    drawHeader();

    if (m_currentScreen == Screen::LIST)
        drawListScreen();
    else
        drawDetailScreen();

    m_window.display();
}

// ── Header ────────────────────────────────────────────────────

void DiaryApp::drawHeader() {
    // Green top bar
    sf::RectangleShape bar(sf::Vector2f(900.f, 60.f));
    bar.setFillColor(sf::Color(0, 120, 80));
    m_window.draw(bar);

    // Crescent symbol
    sf::Text crescent;
    crescent.setFont(m_font);
    crescent.setString("** My Pakistan Travel Diary");
    crescent.setCharacterSize(22);
    crescent.setFillColor(sf::Color::White);
    crescent.setPosition(24.f, 16.f);
    m_window.draw(crescent);

    // Entry count
    sf::Text count;
    count.setFont(m_font);
    count.setString(std::to_string(m_entries.size()) + " memories");
    count.setCharacterSize(14);
    count.setFillColor(sf::Color(180, 255, 200));
    count.setPosition(720.f, 22.f);
    m_window.draw(count);
}

// ── List screen ───────────────────────────────────────────────

void DiaryApp::drawListScreen() {
    // Column labels
    sf::Text label;
    label.setFont(m_font);
    label.setCharacterSize(12);
    label.setFillColor(m_textSecondary);
    label.setString("PLACE                    CITY          DATE"
                    "         MOOD         RATING");
    label.setPosition(24.f, 72.f);
    m_window.draw(label);

    // Divider
    sf::RectangleShape div(sf::Vector2f(860.f, 1.f));
    div.setFillColor(sf::Color(60, 60, 80));
    div.setPosition(20.f, 90.f);
    m_window.draw(div);

    // Entry cards
    float cardW = 860.f;
    float cardH = 90.f;
    float gap   = 10.f;
    float startY = 100.f;

    for (int i = 0; i < (int)m_entries.size(); i++) {
        int visIdx = i - m_scrollOffset;
        if (visIdx < 0 || visIdx > 4) continue;
        float y = startY + visIdx * (cardH + gap);
        drawEntryCard(m_entries[i], 20.f, y, cardW,
                      i == m_selectedIndex);
    }

    // Scroll hint
    if ((int)m_entries.size() > 5) {
        sf::Text hint;
        hint.setFont(m_font);
        hint.setCharacterSize(12);
        hint.setFillColor(m_textSecondary);
        hint.setString("[Up][Down] to scroll   Enter / click to view   Esc to quit");
        hint.setPosition(24.f, 618.f);
        m_window.draw(hint);
    }

    drawCrowdWisdom();
}

// ── Entry card ────────────────────────────────────────────────

void DiaryApp::drawEntryCard(const TravelEntry& entry,
                              float x, float y,
                              float width, bool selected) {
    // Card background
    sf::RectangleShape card(sf::Vector2f(width, 86.f));
    card.setPosition(x, y);
    card.setFillColor(selected ? m_cardHover : m_cardColor);
    if (selected) {
        card.setOutlineThickness(1.5f);
        card.setOutlineColor(m_accentColor);
    }
    m_window.draw(card);

    // Mood color strip on left
    sf::RectangleShape strip(sf::Vector2f(4.f, 86.f));
    strip.setPosition(x, y);
    strip.setFillColor(getMoodColor(entry.getMood()));
    m_window.draw(strip);

    // Place name
    sf::Text place;
    place.setFont(m_font);
    place.setCharacterSize(16);
    place.setFillColor(m_textPrimary);
    std::string placeStr = entry.getPlace();
    if (placeStr.size() > 28) placeStr = placeStr.substr(0, 26) + "..";
    place.setString(placeStr);
    place.setPosition(x + 16.f, y + 10.f);
    m_window.draw(place);

    // City
    sf::Text city;
    city.setFont(m_font);
    city.setCharacterSize(13);
    city.setFillColor(m_textSecondary);
    city.setString(entry.getCity());
    city.setPosition(x + 16.f, y + 34.f);
    m_window.draw(city);

    // Date
    sf::Text date;
    date.setFont(m_font);
    date.setCharacterSize(13);
    date.setFillColor(m_textSecondary);
    date.setString(entry.getDate());
    date.setPosition(x + 200.f, y + 34.f);
    m_window.draw(date);

    // Mood badge
    sf::RectangleShape badge(sf::Vector2f(90.f, 22.f));
    badge.setPosition(x + 400.f, y + 30.f);
    badge.setFillColor(getMoodColor(entry.getMood()));
    m_window.draw(badge);

    sf::Text mood;
    mood.setFont(m_font);
    mood.setCharacterSize(12);
    mood.setFillColor(sf::Color::White);
    mood.setString(entry.getMood());
    mood.setPosition(x + 406.f, y + 34.f);
    m_window.draw(mood);

    // Rating stars
    sf::Text stars;
    stars.setFont(m_font);
    stars.setCharacterSize(14);
    stars.setFillColor(sf::Color(255, 200, 50));
    stars.setString(entry.getRatingStars());
    stars.setPosition(x + 520.f, y + 32.f);
    m_window.draw(stars);

    // Category tag
    sf::Text cat;
    cat.setFont(m_font);
    cat.setCharacterSize(11);
    cat.setFillColor(sf::Color(100, 180, 255));
    cat.setString(entry.getCategory());
    cat.setPosition(x + 16.f, y + 60.f);
    m_window.draw(cat);

    // Status
    sf::Text status;
    status.setFont(m_font);
    status.setCharacterSize(11);
    status.setFillColor(entry.getStatus() == "Visited"
                        ? sf::Color(100, 220, 130)
                        : sf::Color(255, 180, 50));
    status.setString(entry.getStatus());
    status.setPosition(x + 130.f, y + 60.f);
    m_window.draw(status);
}

// ── Detail screen ─────────────────────────────────────────────

void DiaryApp::drawDetailScreen() {
    if (m_entries.empty()) return;
    const TravelEntry& e = m_entries[m_selectedIndex];

    // Detail card background
    sf::RectangleShape card(sf::Vector2f(860.f, 520.f));
    card.setPosition(20.f, 75.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.f);
    card.setOutlineColor(getMoodColor(e.getMood()));
    m_window.draw(card);

    // Mood color bar at top of card
    sf::RectangleShape moodBar(sf::Vector2f(860.f, 6.f));
    moodBar.setPosition(20.f, 75.f);
    moodBar.setFillColor(getMoodColor(e.getMood()));
    m_window.draw(moodBar);

    float tx = 44.f;  // text x
    float ty = 92.f;  // text y start

    auto drawLine = [&](const std::string& label,
                        const std::string& value,
                        sf::Color valColor = sf::Color(240,240,240)) {
        sf::Text lbl;
        lbl.setFont(m_font);
        lbl.setCharacterSize(13);
        lbl.setFillColor(m_textSecondary);
        lbl.setString(label);
        lbl.setPosition(tx, ty);
        m_window.draw(lbl);

        sf::Text val;
        val.setFont(m_font);
        val.setCharacterSize(13);
        val.setFillColor(valColor);
        val.setString(value);
        val.setPosition(tx + 160.f, ty);
        m_window.draw(val);

        ty += 28.f;
    };

    // Place name big
    sf::Text placeName;
    placeName.setFont(m_font);
    placeName.setCharacterSize(24);
    placeName.setFillColor(m_textPrimary);
    placeName.setString(e.getPlace());
    placeName.setPosition(tx, ty);
    m_window.draw(placeName);
    ty += 44.f;

    drawLine("City:",     e.getCity());
    drawLine("Date:",     e.getDate());
    drawLine("Category:", e.getCategory(), sf::Color(100, 180, 255));
    drawLine("Status:",   e.getStatus(),
             e.getStatus() == "Visited"
             ? sf::Color(100, 220, 130)
             : sf::Color(255, 180, 50));
    drawLine("Mood:",     e.getMood(), getMoodColor(e.getMood()));
    drawLine("Rating:",   e.getRatingStars());

    // Divider
    sf::RectangleShape div(sf::Vector2f(800.f, 1.f));
    div.setFillColor(sf::Color(60, 60, 80));
    div.setPosition(tx, ty);
    m_window.draw(div);
    ty += 16.f;

    // Description label
    sf::Text descLabel;
    descLabel.setFont(m_font);
    descLabel.setCharacterSize(13);
    descLabel.setFillColor(m_textSecondary);
    descLabel.setString("My notes:");
    descLabel.setPosition(tx, ty);
    m_window.draw(descLabel);
    ty += 26.f;

    // Description text (wrapped)
    std::string desc = e.getDescription();
    int lineLen = 80;
    for (int i = 0; i < (int)desc.size(); i += lineLen) {
        sf::Text line;
        line.setFont(m_font);
        line.setCharacterSize(14);
        line.setFillColor(m_textPrimary);
        line.setString(desc.substr(i, lineLen));
        line.setPosition(tx, ty);
        m_window.draw(line);
        ty += 22.f;
    }

    ty += 10.f;

    // Photo path if exists
    if (!e.getPhotoPath().empty()) {
        drawLine("Photo:", e.getPhotoPath(),
                 sf::Color(255, 200, 100));
    }

    // Discomfort flag
    if (e.getDiscomfortFlag()) {
        sf::Text flag;
        flag.setFont(m_font);
        flag.setCharacterSize(13);
        flag.setFillColor(sf::Color(255, 100, 100));
        flag.setString("⚠  Discomfort reported for this location");
        flag.setPosition(tx, ty);
        m_window.draw(flag);
        ty += 28.f;
    }

    // Back hint
    sf::Text back;
    back.setFont(m_font);
    back.setCharacterSize(13);
    back.setFillColor(m_textSecondary);
    back.setString("Backspace or Esc — return to list");
    back.setPosition(tx, 574.f);
    m_window.draw(back);
}

// ── Crowd Wisdom panel (bottom of list) ───────────────────────

void DiaryApp::drawCrowdWisdom() {
    sf::RectangleShape panel(sf::Vector2f(860.f, 46.f));
    panel.setPosition(20.f, 566.f);
    panel.setFillColor(sf::Color(20, 40, 30));
    panel.setOutlineThickness(1.f);
    panel.setOutlineColor(sf::Color(0, 100, 60));
    m_window.draw(panel);

    sf::Text wisdom;
    wisdom.setFont(m_font);
    wisdom.setCharacterSize(13);
    wisdom.setFillColor(sf::Color(100, 220, 150));

    int islamabadCount = m_db.getVisitCount("Islamabad");
    std::string islamabadMood = m_db.getMostCommonMood("Islamabad");
    int lahoreCount = m_db.getVisitCount("Lahore");

    std::ostringstream oss;
    oss << "Crowd Wisdom >>  "
        << "Islamabad: " << islamabadCount << " visits | "
        << "Top mood: "  << islamabadMood  << "  |  "
        << "Lahore: "    << lahoreCount    << " visit(s)  |  "
        << "Total memories: " << m_entries.size();

    wisdom.setString(oss.str());
    wisdom.setPosition(30.f, 578.f);
    m_window.draw(wisdom);
}

// ── Mood → color ──────────────────────────────────────────────

sf::Color DiaryApp::getMoodColor(const std::string& mood) {
    if (mood == "Peaceful")   return sf::Color(70,  130, 180);  // steel blue
    if (mood == "Joyful")     return sf::Color(255, 165,  0);   // orange
    if (mood == "Reflective") return sf::Color(138,  43, 226);  // purple
    if (mood == "Nostalgic")  return sf::Color(160,  82,  45);  // sienna
    if (mood == "Energized")  return sf::Color(50,  205,  50);  // lime green
    if (mood == "Anxious")    return sf::Color(220,  20,  60);  // crimson
    return sf::Color(100, 100, 120);                             // default gray
}

std::string DiaryApp::wrapText(const std::string& text, int maxChars) {
    if ((int)text.size() <= maxChars) return text;
    return text.substr(0, maxChars - 2) + "..";
}