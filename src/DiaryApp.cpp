#include "DiaryApp.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdlib>  // for system()
#include <algorithm> 

// Helper function to open file picker and return selected file path
std::string openFilePickerForPhoto() {
    // Uses zenity (standard on Ubuntu/Fedora)
    // Alternative: could use other tools like kdialog, nautilus, etc.
    
    system("zenity --file-selection --filename=$HOME/Pictures/ "
           "--title='Select Travel Photo' > /tmp/photo_path.txt 2>/dev/null");
    
    std::ifstream file("/tmp/photo_path.txt");
    std::string path;
    if (file.is_open()) {
        std::getline(file, path);
        file.close();
        // Remove trailing newline
        if (!path.empty() && path.back() == '\n')
            path.pop_back();
    }
    return path;
}

// ── Constructor ───────────────────────────────────────────────

DiaryApp::DiaryApp(Database& db)
    : m_window(sf::VideoMode(900, 650), "My Pakistan Travel Diary",
               sf::Style::Titlebar | sf::Style::Close),
      m_db(db),
      m_selectedIndex(0),
      m_scrollOffset(0),
      m_currentScreen(Screen::LIST),
      m_activeField(0),
      m_questionStep(0)
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
    m_exporter = new HTMLExporter(m_db);
    clearForm();
    m_editingEntryId = -1;
    m_deleteConfirmed = false;
}

void DiaryApp::clearForm() {
    m_inputPlace    = "";
    m_inputCity     = "";
    m_inputDate     = "";
    m_inputDesc     = "";
    m_inputCategory = "";
    m_inputRating   = "3";
    m_inputMood     = "Peaceful";
    m_inputStatus   = "Visited";
    m_inputPhoto    = "";
    m_activeField   = 0;
    m_questionStep  = 0;
    m_check         = PreTripCheck();
    m_editingEntryId = -1;
    m_deleteConfirmed = false;
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
                if (event.key.code == sf::Keyboard::A) {
                    clearForm();
                    m_currentScreen = Screen::ADD_ENTRY;
                }
                if (event.key.code == sf::Keyboard::E) {
                   m_exporter->exportToHTML(m_entries);
                   m_currentScreen = Screen::EXPORT_COMPLETE;
                }
            }
            else if (m_currentScreen == Screen::DETAIL) {
                if (event.key.code == sf::Keyboard::Escape ||
                   event.key.code == sf::Keyboard::BackSpace) {
                   m_currentScreen = Screen::LIST;
                }
               if (event.key.code == sf::Keyboard::E) {
                // Edit mode
                  const TravelEntry& e = m_entries[m_selectedIndex];
                  m_editingEntryId = e.getId();
                  m_inputPlace = e.getPlace();
                  m_inputCity = e.getCity();
                  m_inputDate = e.getDate();
                  m_inputDesc = e.getDescription();
                  m_inputCategory = e.getCategory();
                  m_inputRating = std::to_string((int)e.getRating());
                  m_inputMood = e.getMood();
                  m_inputStatus = e.getStatus();
                  m_inputPhoto = e.getPhotoPath();
                  m_activeField = 0;
                  m_currentScreen = Screen::EDIT_ENTRY;
                }
               if (event.key.code == sf::Keyboard::D) {
                // Delete confirmation
                  m_deleteConfirmed = false;
                  m_currentScreen = Screen::DELETE_CONFIRM;
                }
               if (event.key.code == sf::Keyboard::P && 
                  !m_entries[m_selectedIndex].getPhotoPath().empty()) {
                // View photo
                   m_currentScreen = Screen::PHOTO_VIEW;
                }
            }
            else if (m_currentScreen == Screen::ADD_ENTRY) {
                handleAddEntryInput(event);
            }
            else if (m_currentScreen == Screen::QUESTIONNAIRE) {
                handleQuestionnaireInput(event);
            }
            else if (m_currentScreen == Screen::SUPPORT_MESSAGE) {
                if (event.key.code == sf::Keyboard::Return ||
                    event.key.code == sf::Keyboard::Escape) {
                    saveFormEntry();
                    m_entries = m_db.getAllEntries();
                    m_currentScreen = Screen::LIST;
                }
            }
            else if (m_currentScreen == Screen::EXPORT_COMPLETE) {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return) {
                    m_currentScreen = Screen::LIST;
                }
            }
        
        else if (m_currentScreen == Screen::EDIT_ENTRY) {
                handleEditEntryInput(event);
        }
        else if (m_currentScreen == Screen::DELETE_CONFIRM) {
               handleDeleteConfirmInput(event);
        }
        else if (m_currentScreen == Screen::PHOTO_VIEW) {
            if (event.key.code == sf::Keyboard::Escape ||
               event.key.code == sf::Keyboard::Return) {
               m_currentScreen = Screen::DETAIL;
    }
}
       }       // Mouse click on card
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
    else if (m_currentScreen == Screen::ADD_ENTRY)
        drawAddEntryScreen();
    else if (m_currentScreen == Screen::QUESTIONNAIRE)
        drawQuestionnaireScreen();
    else if (m_currentScreen == Screen::SUPPORT_MESSAGE)
        drawSupportScreen();    
    else if (m_currentScreen == Screen::EXPORT_COMPLETE)
        drawExportCompleteScreen();
    else if (m_currentScreen == Screen::EDIT_ENTRY)
        drawEditEntryScreen();
    else if (m_currentScreen == Screen::DELETE_CONFIRM)
        drawDeleteConfirmScreen();
    else if (m_currentScreen == Screen::PHOTO_VIEW)
        drawPhotoViewScreen();
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

   // Photo display
if (!e.getPhotoPath().empty()) {
    // Try to load and display the photo
    if (m_photoTexture.loadFromFile(e.getPhotoPath())) {
        m_photoLoaded = true;
        
        // Photo box background
        sf::RectangleShape photoBox(sf::Vector2f(400.f, 250.f));
        photoBox.setPosition(tx, ty);
        photoBox.setFillColor(sf::Color(20, 20, 30));
        photoBox.setOutlineThickness(1.f);
        photoBox.setOutlineColor(sf::Color(255, 200, 100));
        m_window.draw(photoBox);

        // Scale photo to fit box
        m_photoSprite.setTexture(m_photoTexture, true);
        float photoWidth = m_photoTexture.getSize().x;
        float photoHeight = m_photoTexture.getSize().y;
        float scaleX = 390.f / photoWidth;
        float scaleY = 240.f / photoHeight;
        float scale = std::min(scaleX, scaleY);
        
        m_photoSprite.setScale(scale, scale);
        m_photoSprite.setPosition(
            tx + (400.f - photoWidth * scale) / 2.f,
            ty + (250.f - photoHeight * scale) / 2.f
        );
        m_window.draw(m_photoSprite);

        ty += 260.f;

        ty += 20.f;
    } else {
        // Photo file not found
        sf::RectangleShape photoBox(sf::Vector2f(400.f, 80.f));
        photoBox.setPosition(tx, ty);
        photoBox.setFillColor(sf::Color(40, 20, 20));
        photoBox.setOutlineThickness(1.f);
        photoBox.setOutlineColor(sf::Color(220, 100, 100));
        m_window.draw(photoBox);

        sf::Text photoError;
        photoError.setFont(m_font);
        photoError.setCharacterSize(13);
        photoError.setFillColor(sf::Color(220, 100, 100));
        photoError.setString("Photo not found:\n" + e.getPhotoPath());
        photoError.setPosition(tx + 10.f, ty + 10.f);
        m_window.draw(photoError);

        ty += 90.f;
    }
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
    back.setString("Esc/Backspace = back  |  E = edit  |  D = delete  |  P = photo");
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

// ── Input handler for Add Entry form ─────────────────────────

void DiaryApp::handleAddEntryInput(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    std::string* fields[] = {
        &m_inputPlace, &m_inputCity, &m_inputDate,
        &m_inputDesc,  &m_inputCategory, &m_inputRating,
        &m_inputMood,  &m_inputStatus,   &m_inputPhoto
    };
    int fieldCount = 9;

    // F to open file browser for photo
    if (event.key.code == sf::Keyboard::F) {
       if (m_activeField == 8) {  // Photo field is index 8
          std::cout << "Opening file picker...\n";
          std::string selectedPath = openFilePickerForPhoto();
          if (!selectedPath.empty()) {
              m_inputPhoto = selectedPath;
            std::cout << "Photo selected: " << selectedPath << "\n";
        }
    }
    return;
}

    // Tab to next field
    if (event.key.code == sf::Keyboard::Tab) {
        m_activeField = (m_activeField + 1) % fieldCount;
        return;
    }

    // Escape to cancel
    if (event.key.code == sf::Keyboard::Escape) {
        m_currentScreen = Screen::LIST;
        clearForm();
        return;
    }

    // Backspace to delete
    if (event.key.code == sf::Keyboard::BackSpace) {
        if (!fields[m_activeField]->empty())
            fields[m_activeField]->pop_back();
        return;
    }

    // Enter to save
    if (event.key.code == sf::Keyboard::Return) {
        if (m_inputPlace.empty() || m_inputCity.empty() || m_inputDate.empty()) {
            std::cout << "Error: Place, City, and Date required.\n";
            return;
        }

        if (m_inputStatus == "Planned") {
            m_check = PreTripCheck();
            m_questionStep = 0;
            m_currentScreen = Screen::QUESTIONNAIRE;
        } else {
            bool success = saveFormEntry();
            std::cout << (success ? "Saved OK\n" : "Save failed\n");
            m_entries = m_db.getAllEntries();
            clearForm();
            m_currentScreen = Screen::LIST;
        }
        return;
    }

    // Map keys to characters
    char c = '\0';
    if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
        c = event.key.shift ? 'A' + (event.key.code - sf::Keyboard::A)
                            : 'a' + (event.key.code - sf::Keyboard::A);
    }
    else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
        c = '0' + (event.key.code - sf::Keyboard::Num0);
    }
    else if (event.key.code == sf::Keyboard::Space) c = ' ';
    else if (event.key.code == sf::Keyboard::Period) c = '.';
    else if (event.key.code == sf::Keyboard::Comma) c = ',';
    else if (event.key.code == sf::Keyboard::Slash) c = '/';
    else if (event.key.code == sf::Keyboard::Hyphen) c = '-';
    else if (event.key.code == sf::Keyboard::Equal) c = '=';

    if (c != '\0') {
        *fields[m_activeField] += c;
    }
}
// ── Input handler for questionnaire ──────────────────────────

void DiaryApp::handleQuestionnaireInput(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    if (event.key.code == sf::Keyboard::Escape) {
        m_currentScreen = Screen::LIST;
        return;
    }

    if (m_questionStep == 0) {
        // Q1: traveling alone? Y/N
        if (event.key.code == sf::Keyboard::Y)
            m_check.travelingAlone = true;
        else if (event.key.code == sf::Keyboard::N)
            m_check.travelingAlone = false;
        else return;
        m_questionStep++;
    }
    else if (m_questionStep == 1) {
        // Q2: feeling? 1-4
        if (event.key.code == sf::Keyboard::Num1)
            m_check.feelingScore = 1;
        else if (event.key.code == sf::Keyboard::Num2)
            m_check.feelingScore = 2;
        else if (event.key.code == sf::Keyboard::Num3)
            m_check.feelingScore = 3;
        else if (event.key.code == sf::Keyboard::Num4)
            m_check.feelingScore = 4;
        else return;
        m_questionStep++;
    }
    else if (m_questionStep == 2) {
        // Q3: shared plans? Y/N
        if (event.key.code == sf::Keyboard::Y)
            m_check.sharedWithSomeone = true;
        else if (event.key.code == sf::Keyboard::N)
            m_check.sharedWithSomeone = false;
        else return;
        m_questionStep++;
    }
    else if (m_questionStep == 3) {
        // Q4: energy? 1-3
        if (event.key.code == sf::Keyboard::Num1)
            m_check.energyLevel = 1;
        else if (event.key.code == sf::Keyboard::Num2)
            m_check.energyLevel = 2;
        else if (event.key.code == sf::Keyboard::Num3)
            m_check.energyLevel = 3;
        else return;

        // All answered — evaluate
        m_check.evaluate();

        if (m_check.flagged)
            m_currentScreen = Screen::SUPPORT_MESSAGE;
        else {
            saveFormEntry();
            m_entries = m_db.getAllEntries();
            m_currentScreen = Screen::LIST;
        }
    }
}

// ── Save form to database ─────────────────────────────────────

bool DiaryApp::saveFormEntry() {
    float rating = 3.0f;
    try { rating = std::stof(m_inputRating); }
    catch (...) { rating = 3.0f; }
    if (rating < 1.0f) rating = 1.0f;
    if (rating > 5.0f) rating = 5.0f;

    TravelEntry entry(
        0,
        m_inputPlace,
        m_inputCity,
        m_inputDate,
        m_inputDesc,
        m_inputCategory.empty() ? "General" : m_inputCategory,
        rating,
        m_inputMood.empty()   ? "Peaceful" : m_inputMood,
        m_inputStatus.empty() ? "Visited"  : m_inputStatus,
        m_inputPhoto,
        false,
        false
    );
    return m_db.insertEntry(entry);
}

// ── Draw Add Entry screen ─────────────────────────────────────

void DiaryApp::drawAddEntryScreen() {
    // Background card
    sf::RectangleShape card(sf::Vector2f(860.f, 530.f));
    card.setPosition(20.f, 75.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.f);
    card.setOutlineColor(m_accentColor);
    m_window.draw(card);

    // Title
    sf::Text title;
    title.setFont(m_font);
    title.setCharacterSize(20);
    title.setFillColor(m_accentColor);
    title.setString("+ Add New Travel Entry");
    title.setPosition(40.f, 86.f);
    m_window.draw(title);

    // Field definitions
    struct Field {
        std::string label;
        std::string hint;
        std::string* value;
    };

    Field fields[] = {
        { "Place",    "e.g. Shah Allah Ditta Caves", &m_inputPlace    },
        { "City",     "e.g. Islamabad",              &m_inputCity     },
        { "Date",     "YYYY-MM-DD",                  &m_inputDate     },
        { "Notes",    "Your memory...",              &m_inputDesc     },
        { "Category", "Historical/Religious/Nature", &m_inputCategory },
        { "Rating",   "1.0 to 5.0",                 &m_inputRating   },
        { "Mood",     "Peaceful/Joyful/Nostalgic...", &m_inputMood   },
        { "Status",   "Visited / Planned",           &m_inputStatus   },
        { "Photo",    "path/to/photo.jpg (optional)",&m_inputPhoto    },
    };

    float startY = 120.f;
    float rowH   = 48.f;

    for (int i = 0; i < 9; i++) {
        bool active = (i == m_activeField);
        float y = startY + i * rowH;

        // Label
        sf::Text lbl;
        lbl.setFont(m_font);
        lbl.setCharacterSize(13);
        lbl.setFillColor(active ? m_accentColor : m_textSecondary);
        lbl.setString(fields[i].label + ":");
        lbl.setPosition(40.f, y);
        m_window.draw(lbl);

        // Input box
        sf::RectangleShape box(sf::Vector2f(560.f, 26.f));
        box.setPosition(180.f, y - 4.f);
        box.setFillColor(sf::Color(40, 40, 55));
        box.setOutlineThickness(active ? 1.5f : 0.5f);
        box.setOutlineColor(active
            ? m_accentColor
            : sf::Color(60, 60, 80));
        m_window.draw(box);

        // Input text or hint
        sf::Text val;
        val.setFont(m_font);
        val.setCharacterSize(13);
        std::string display = *fields[i].value;
        if (display.empty()) {
            val.setFillColor(sf::Color(80, 80, 100));
            display = fields[i].hint;
        } else {
            val.setFillColor(m_textPrimary);
            // Cursor blink effect
            if (active) display += "|";
        }
        val.setString(display);
        val.setPosition(184.f, y);
        m_window.draw(val);
    }

    // Instructions
    sf::Text hint;
    hint.setFont(m_font);
    hint.setCharacterSize(12);
    hint.setFillColor(m_textSecondary);
    hint.setString(
    "Tab = next field   Enter = save   Esc = cancel   "
    "F (in Photo field) = browse files");
    hint.setPosition(40.f, 560.f);
    m_window.draw(hint);
}

// ── Draw Questionnaire screen ─────────────────────────────────

void DiaryApp::drawQuestionnaireScreen() {
    sf::RectangleShape card(sf::Vector2f(700.f, 380.f));
    card.setPosition(100.f, 120.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(sf::Color(0, 168, 107));
    m_window.draw(card);

    sf::Text title;
    title.setFont(m_font);
    title.setCharacterSize(18);
    title.setFillColor(m_accentColor);
    title.setString("Before you go — quick check-in");
    title.setPosition(130.f, 136.f);
    m_window.draw(title);

    sf::Text sub;
    sub.setFont(m_font);
    sub.setCharacterSize(13);
    sub.setFillColor(m_textSecondary);
    sub.setString("Question " + std::to_string(m_questionStep + 1) + " of 4");
    sub.setPosition(130.f, 164.f);
    m_window.draw(sub);

    // Divider
    sf::RectangleShape div(sf::Vector2f(640.f, 1.f));
    div.setFillColor(sf::Color(60, 60, 80));
    div.setPosition(130.f, 184.f);
    m_window.draw(div);

    float qy = 204.f;
    sf::Text question;
    question.setFont(m_font);
    question.setCharacterSize(16);
    question.setFillColor(m_textPrimary);
    question.setPosition(130.f, qy);

    sf::Text options;
    options.setFont(m_font);
    options.setCharacterSize(14);
    options.setFillColor(sf::Color(100, 220, 150));
    options.setPosition(130.f, qy + 46.f);

    if (m_questionStep == 0) {
        question.setString("Are you traveling alone?");
        options.setString("Y = Yes      N = No");
    }
    else if (m_questionStep == 1) {
        question.setString("How are you feeling about this trip?");
        options.setString(
            "1 = Excited    2 = Nervous\n"
            "3 = Need a break    4 = Unsure");
    }
    else if (m_questionStep == 2) {
        question.setString("Does anyone know your travel plans?");
        options.setString("Y = Yes      N = No");
    }
    else {
        question.setString("How is your energy level today?");
        options.setString("1 = Low      2 = Medium      3 = High");
    }

    m_window.draw(question);
    m_window.draw(options);

    sf::Text esc;
    esc.setFont(m_font);
    esc.setCharacterSize(12);
    esc.setFillColor(m_textSecondary);
    esc.setString("Esc = cancel and go back");
    esc.setPosition(130.f, 460.f);
    m_window.draw(esc);
}

// ── Draw Support Message screen ───────────────────────────────

void DiaryApp::drawSupportScreen() {
    // Soft warm card — not alarming
    sf::RectangleShape card(sf::Vector2f(700.f, 340.f));
    card.setPosition(100.f, 140.f);
    card.setFillColor(sf::Color(30, 28, 40));
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(sf::Color(180, 140, 255));
    m_window.draw(card);

    float ty = 162.f;
    auto drawText = [&](const std::string& s,
                        int size,
                        sf::Color col,
                        float indent = 0.f) {
        sf::Text t;
        t.setFont(m_font);
        t.setCharacterSize(size);
        t.setFillColor(col);
        t.setString(s);
        t.setPosition(130.f + indent, ty);
        m_window.draw(t);
        ty += size + 14.f;
    };

    drawText("Hey, a moment before you go.",
             18, sf::Color(200, 180, 255));

    ty += 10.f;

    drawText("It sounds like you might be carrying",
             14, m_textPrimary);
    drawText("a lot right now. That's okay.",
             14, m_textPrimary);

    ty += 10.f;

    drawText("Consider sharing your plans with",
             14, m_textSecondary);
    drawText("someone you trust before you leave.",
             14, m_textSecondary);

    ty += 14.f;

    drawText("You are not alone.",
             16, sf::Color(150, 255, 180));

    ty += 10.f;

    drawText("If you need to talk to someone:",
             13, m_textSecondary);

    // Helpline box
    sf::RectangleShape hbox(sf::Vector2f(400.f, 36.f));
    hbox.setPosition(130.f, ty);
    hbox.setFillColor(sf::Color(40, 35, 60));
    hbox.setOutlineThickness(1.f);
    hbox.setOutlineColor(sf::Color(180, 140, 255));
    m_window.draw(hbox);

    sf::Text helpline;
    helpline.setFont(m_font);
    helpline.setCharacterSize(15);
    helpline.setFillColor(sf::Color(200, 180, 255));
    helpline.setString("Umang helpline: 0317-4288665");
    helpline.setPosition(138.f, ty + 8.f);
    m_window.draw(helpline);

    ty += 56.f;

    drawText("Your entry has been saved.",
             13, sf::Color(100, 200, 130));

    ty += 6.f;

    drawText("Press Enter or Esc to continue.",
             13, m_textSecondary);
}

DiaryApp::~DiaryApp() {
    if (m_exporter != nullptr) {
        delete m_exporter;
        m_exporter = nullptr;
    }
}

void DiaryApp::drawExportCompleteScreen() {
    sf::RectangleShape card(sf::Vector2f(700.f, 300.f));
    card.setPosition(100.f, 160.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(sf::Color(100, 220, 130));
    m_window.draw(card);

    float ty = 182.f;
    auto drawText = [&](const std::string& s, int size, sf::Color col) {
        sf::Text t;
        t.setFont(m_font);
        t.setCharacterSize(size);
        t.setFillColor(col);
        t.setString(s);
        t.setPosition(130.f, ty);
        m_window.draw(t);
        ty += size + 16.f;
    };

    drawText("Export Complete!", 22, sf::Color(100, 220, 130));
    ty += 10.f;
    drawText("Your travel diary has been saved as HTML.", 14, m_textPrimary);
    drawText("Open export/travel_report.html in your browser.", 14, m_textPrimary);
    ty += 10.f;
    drawText("Share this file with friends for decentralized", 13, m_textSecondary);
    drawText("community insights.", 13, m_textSecondary);

    drawText("Press Enter to return to diary.", 13, m_textSecondary);
}

// ── Draw Edit Entry screen ────────────────────────────

void DiaryApp::drawEditEntryScreen() {
    // Reuse the same layout as ADD_ENTRY but with "Edit" title
    sf::RectangleShape card(sf::Vector2f(860.f, 530.f));
    card.setPosition(20.f, 75.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.f);
    card.setOutlineColor(sf::Color(100, 200, 255));  // Blue for edit
    m_window.draw(card);

    // Title
    sf::Text title;
    title.setFont(m_font);
    title.setCharacterSize(20);
    title.setFillColor(sf::Color(100, 200, 255));
    title.setString("✎ Edit Travel Entry");
    title.setPosition(40.f, 86.f);
    m_window.draw(title);

    // Field definitions
    struct Field {
        std::string label;
        std::string hint;
        std::string* value;
    };

    Field fields[] = {
        { "Place",    "e.g. Shah Allah Ditta Caves", &m_inputPlace    },
        { "City",     "e.g. Islamabad",              &m_inputCity     },
        { "Date",     "YYYY-MM-DD",                  &m_inputDate     },
        { "Notes",    "Your memory...",              &m_inputDesc     },
        { "Category", "Historical/Religious/Nature", &m_inputCategory },
        { "Rating",   "1.0 to 5.0",                 &m_inputRating   },
        { "Mood",     "Peaceful/Joyful/Nostalgic...", &m_inputMood   },
        { "Status",   "Visited / Planned",           &m_inputStatus   },
        { "Photo",    "path/to/photo.jpg (optional)",&m_inputPhoto    },
    };

    float startY = 120.f;
    float rowH   = 48.f;

    for (int i = 0; i < 9; i++) {
        bool active = (i == m_activeField);
        float y = startY + i * rowH;

        // Label
        sf::Text lbl;
        lbl.setFont(m_font);
        lbl.setCharacterSize(13);
        lbl.setFillColor(active ? sf::Color(100, 200, 255) : m_textSecondary);
        lbl.setString(fields[i].label + ":");
        lbl.setPosition(40.f, y);
        m_window.draw(lbl);

        // Input box
        sf::RectangleShape box(sf::Vector2f(560.f, 26.f));
        box.setPosition(180.f, y - 4.f);
        box.setFillColor(sf::Color(40, 40, 55));
        box.setOutlineThickness(active ? 1.5f : 0.5f);
        box.setOutlineColor(active
            ? sf::Color(100, 200, 255)
            : sf::Color(60, 60, 80));
        m_window.draw(box);

        // Input text or hint
        sf::Text val;
        val.setFont(m_font);
        val.setCharacterSize(13);
        std::string display = *fields[i].value;
        if (display.empty()) {
            val.setFillColor(sf::Color(80, 80, 100));
            display = fields[i].hint;
        } else {
            val.setFillColor(m_textPrimary);
            if (active) display += "|";
        }
        val.setString(display);
        val.setPosition(184.f, y);
        m_window.draw(val);
    }

   // Instructions
    sf::Text hint;
    hint.setFont(m_font);
    hint.setCharacterSize(12);
    hint.setFillColor(m_textSecondary);
    hint.setString(
    "Tab = next field   Enter = save   Esc = cancel   "
    "F (in Photo field) = browse files");
    hint.setPosition(40.f, 560.f);
    m_window.draw(hint);
}

// ── Draw Delete Confirm screen ────────────────────────

void DiaryApp::drawDeleteConfirmScreen() {
    sf::RectangleShape card(sf::Vector2f(600.f, 280.f));
    card.setPosition(150.f, 160.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(sf::Color(220, 50, 50));
    m_window.draw(card);

    float ty = 180.f;
    auto drawText = [&](const std::string& s, int size, sf::Color col) {
        sf::Text t;
        t.setFont(m_font);
        t.setCharacterSize(size);
        t.setFillColor(col);
        t.setString(s);
        t.setPosition(170.f, ty);
        m_window.draw(t);
        ty += size + 18.f;
    };

    drawText("Delete Entry?", 20, sf::Color(220, 50, 50));
    ty += 10.f;

    const TravelEntry& e = m_entries[m_selectedIndex];
    drawText(e.getPlace(), 16, m_textPrimary);

    ty += 20.f;

    drawText("This cannot be undone.", 13, m_textSecondary);

    ty += 30.f;

    drawText("Y = Delete     N = Cancel", 14, sf::Color(255, 200, 100));
}

// ── Draw Photo View screen ─────────────────────────────

void DiaryApp::drawPhotoViewScreen() {
    const TravelEntry& e = m_entries[m_selectedIndex];
    std::string photoPath = e.getPhotoPath();

    sf::RectangleShape card(sf::Vector2f(700.f, 480.f));
    card.setPosition(100.f, 75.f);
    card.setFillColor(m_cardColor);
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(sf::Color(255, 200, 100));
    m_window.draw(card);

    float ty = 95.f;

    // Title
    sf::Text title;
    title.setFont(m_font);
    title.setCharacterSize(18);
    title.setFillColor(sf::Color(255, 200, 100));
    title.setString("Photo: " + e.getPlace());
    title.setPosition(130.f, ty);
    m_window.draw(title);

    ty += 40.f;

    // Photo path info
    sf::Text pathLabel;
    pathLabel.setFont(m_font);
    pathLabel.setCharacterSize(12);
    pathLabel.setFillColor(m_textSecondary);
    pathLabel.setString("File path:");
    pathLabel.setPosition(130.f, ty);
    m_window.draw(pathLabel);

    ty += 20.f;

    sf::Text pathValue;
    pathValue.setFont(m_font);
    pathValue.setCharacterSize(13);
    pathValue.setFillColor(sf::Color(100, 200, 255));
    pathValue.setString(photoPath);
    pathValue.setPosition(130.f, ty);
    m_window.draw(pathValue);

    ty += 40.f;

    // Status message
    sf::Text info;
    info.setFont(m_font);
    info.setCharacterSize(13);
    info.setFillColor(m_textSecondary);
    info.setString(
        "To view the photo, open the file at the path above\n"
        "in your image viewer or file manager.\n\n"
        "In a future update, photos will preview here.");
    info.setPosition(130.f, ty);
    m_window.draw(info);

    ty += 120.f;

    // Instructions
    sf::Text hint;
    hint.setFont(m_font);
    hint.setCharacterSize(13);
    hint.setFillColor(m_textSecondary);
    hint.setString("Press Esc or Enter to go back");
    hint.setPosition(130.f, ty);
    m_window.draw(hint);
}

// ── Input handler for Edit Entry form ─────────────────

void DiaryApp::handleEditEntryInput(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    // DEBUG
    std::cout << "Edit form key: " << event.key.code << "\n";

    std::string* fields[] = {
        &m_inputPlace, &m_inputCity, &m_inputDate,
        &m_inputDesc,  &m_inputCategory, &m_inputRating,
        &m_inputMood,  &m_inputStatus,   &m_inputPhoto
    };
    int fieldCount = 9;

    // F to open file browser for photo
if (event.key.code == sf::Keyboard::F) {
    if (m_activeField == 8) {  // Photo field is index 8
        std::cout << "Opening file picker...\n";
        std::string selectedPath = openFilePickerForPhoto();
        if (!selectedPath.empty()) {
            m_inputPhoto = selectedPath;
            std::cout << "Photo selected: " << selectedPath << "\n";
        }
    }
    return;
}

    // Tab to next field
    if (event.key.code == sf::Keyboard::Tab) {
        std::cout << "TAB pressed\n";
        m_activeField = (m_activeField + 1) % fieldCount;
        return;
    }

    // Escape to cancel
    if (event.key.code == sf::Keyboard::Escape) {
        
        m_currentScreen = Screen::DETAIL;
        clearForm();
        return;
    }

    // Backspace to delete
    if (event.key.code == sf::Keyboard::BackSpace) {
        if (!fields[m_activeField]->empty())
            fields[m_activeField]->pop_back();
        return;
    }

    // Enter to save
    if (event.key.code == sf::Keyboard::Return) {
        if (m_inputPlace.empty() || m_inputCity.empty() || m_inputDate.empty()) {
            std::cout << "Error: Place, City, and Date required.\n";
            return;
        }

        bool success = updateFormEntry();
        std::cout << (success ? "Entry updated.\n" : "Update failed.\n");
        m_entries = m_db.getAllEntries();
        clearForm();
        m_currentScreen = Screen::DETAIL;
        return;
    }

    // Character input
    char c = '\0';
    if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
        c = event.key.shift ? 'A' + (event.key.code - sf::Keyboard::A)
                            : 'a' + (event.key.code - sf::Keyboard::A);
    }
    else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
        c = '0' + (event.key.code - sf::Keyboard::Num0);
    }
    else if (event.key.code == sf::Keyboard::Space) c = ' ';
    else if (event.key.code == sf::Keyboard::Period) c = '.';
    else if (event.key.code == sf::Keyboard::Comma) c = ',';
    else if (event.key.code == sf::Keyboard::Slash) c = '/';
    else if (event.key.code == sf::Keyboard::Hyphen) c = '-';
    else if (event.key.code == sf::Keyboard::Equal) c = '=';

    if (c != '\0') {
        *fields[m_activeField] += c;
    }
}

// ── Input handler for Delete Confirm ──────────────────

void DiaryApp::handleDeleteConfirmInput(sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    // Get the key code as lowercase letter
    sf::Keyboard::Key key = event.key.code;
    
    if (key == sf::Keyboard::Y) {
        // Delete confirmed
        int entryId = m_entries[m_selectedIndex].getId();
        bool success = m_db.deleteEntry(entryId);
        std::cout << (success ? "Entry deleted.\n" : "Delete failed.\n");
        m_entries = m_db.getAllEntries();
        
        if (!m_entries.empty()) {
            if (m_selectedIndex >= (int)m_entries.size())
                m_selectedIndex = m_entries.size() - 1;
        }
        
        clearForm();
        m_currentScreen = Screen::LIST;
    }
    else if (key == sf::Keyboard::N) {
        // Cancel
        m_currentScreen = Screen::DETAIL;
    }
}

// ── Update entry in database ──────────────────────────

bool DiaryApp::updateFormEntry() {
    float rating = 3.0f;
    try { rating = std::stof(m_inputRating); }
    catch (...) { rating = 3.0f; }
    if (rating < 1.0f) rating = 1.0f;
    if (rating > 5.0f) rating = 5.0f;

    TravelEntry entry(
        m_editingEntryId,  // Use existing ID
        m_inputPlace,
        m_inputCity,
        m_inputDate,
        m_inputDesc,
        m_inputCategory.empty() ? "General" : m_inputCategory,
        rating,
        m_inputMood.empty()   ? "Peaceful" : m_inputMood,
        m_inputStatus.empty() ? "Visited"  : m_inputStatus,
        m_inputPhoto,
        false,
        false
    );
    return m_db.updateEntry(entry);  // UPDATE, not INSERT
}