# My Pakistan — Personal Travel Diary

> A decentralized community travel diary celebrating Pakistani journeys, emotions, and mental wellness.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## 🌐 LIVE WEBSITE

**Visit the live travel website:** https://MaazGul-2006.github.io/MyPakistanDiary-Website/

The website features:
- ✅ Multi-page city guides (Islamabad, Lahore, Taxila, Murree, Rawalpindi)
- ✅ Real YouTube travel videos for each destination
- ✅ Newspaper & magazine features (The Guardian, Express Tribune, Lonely Planet)
- ✅ Interactive OpenStreetMap integration
- ✅ Light/Dark mode toggle
- ✅ Professional responsive design
- ✅ 8 real travel photos from Pakistan

---

## 🎯 Project Vision

**My Pakistan Travel Diary** is a desktop application that transforms travel documentation into a gateway for emotional connection and peer support. Built during a week-long intensive, this project demonstrates how shared experiences—especially travel—can help university students overcome isolation and stigma around mental health.

### The Problem
Many Pakistani university students struggle silently with depression and anxiety, unable to share their feelings. Social barriers, stigma, and lack of safe spaces prevent them from seeking support.

### Our Solution
We embed wellness check-ins within a travel diary—making emotional expression feel natural and safe. When peers share their journeys, the app aggregates their collective insights (Wisdom of Crowds), creating community validation without centralization.

---

## ✨ Key Features

### 📔 Personal Travel Diary
- **Add Entries**: Place, city, date, personal notes, category, mood, rating
- **Rich Metadata**: Mood tags (Peaceful, Joyful, Reflective, Nostalgic, Energized, Anxious)
- **Status Tracking**: Mark trips as Visited or Planned
- **Photo Support**: Link travel photos (JPG/PNG) with automatic copying to export
- **Rating System**: 1-5 star ratings for each destination
- **Category System**: Historical, Religious, Nature, Recreation, etc.

### 🎬 Destination Videos & Media (NEW - V3)
- **Embedded YouTube Videos**: Real travel vlogs for each destination
  - Lahore: Fort Heritage Tour, Food Street Experience
  - Taxila: Museum & Excavations, 2000 Years History
  - Islamabad: Faisal Mosque, Margalla Hills
  - Murree: Hill Station Adventure
  - Rawalpindi: Military History Tour

- **Media Coverage**: Authentic newspaper & magazine features
  - 📰 News: The Guardian, New York Times, BBC Travel
  - 👑 Magazines: Lonely Planet, Express Tribune, National Geographic
  - Direct links to real travel journalism

### 🗺️ Interactive Maps (NEW - V3)
- OpenStreetMap integration for every city
- Free, no API key required
- Shows exact coordinates
- Professional styling with markers

### 🛡️ Mental Wellness Layer
- **Pre-Trip Questionnaire**: When marking a trip as "Planned", users answer 4 wellness questions:
  - Are you traveling alone?
  - How are you feeling about this trip?
  - Does anyone know your travel plans?
  - What's your energy level today?
- **Discomfort Reporting**: Flag unsafe experiences at locations (future: crowd-alerts)
- **Support Resources**: If responses flag concern → display **Umang helpline: 0317-4288665**

### 🌍 Wisdom of Crowds
Aggregate community insights without a central server:
- **Average Ratings per Place**: See how others rated the same destination
- **Visit Frequency Heatmap**: Which Pakistani cities are most visited?
- **Mood Distribution**: "Most travelers feel Peaceful at Faisal Mosque"
- **Hidden Gem Score**: High ratings + low visit count = underrated treasure

### 📤 Decentralized Sharing
- **JSON Import/Export**: Share your diary with friends as `.json` files
- **No Server Required**: Each SQLite database is an independent node
- **HTML Export**: Beautiful styled report with city sections
- **Multi-Page Website Export**: Full tourism website generated from your data
- **Community Aggregation**: Import friends' entries locally, crowd wisdom emerges

### 🎨 User Interface
- **SFML-based GUI**: Smooth, responsive desktop app
- **Dark Theme**: Eye-friendly dark mode with Pakistan green accents (#0A6B3F)
- **Keyboard Navigation**: Arrow keys, Tab, Enter — full terminal-style control
- **Photo Display**: View travel photos directly in app
- **Entry Cards**: Color-coded by mood, with at-a-glance info

### 🌓 Light/Dark Mode Toggle
- Works on all pages
- Preference saved locally
- Professional styling for both modes

---

## 🏗️ Architecture

### Tech Stack
- **Language**: C++17 (OOP + PF concepts)
- **GUI**: SFML 2.5 (graphics, windowing)
- **Database**: SQLite3 (lightweight, local, decentralized)
- **Export**: HTML + CSS (styled reports + multi-page website)
- **Web**: OpenStreetMap + Leaflet (free maps)
- **Videos**: YouTube embedded players
- **Build**: CMake 3.16+, g++ compiler
- **OS**: Linux (Ubuntu 20.04+)

### File Structure
MyPakistanDiary/
├── include/
│   ├── TravelEntry.h        # Core data model
│   ├── Database.h           # SQLite CRUD + V3 videos/media
│   ├── City.h               # City guides with tips
│   ├── DiaryApp.h           # SFML window & UI controller
│   ├── PreTripCheck.h       # Wellness questionnaire
│   └── HTMLExporter.h       # Report generation
├── src/
│   ├── main.cpp             # Entry point + V3 seeding
│   ├── TravelEntry.cpp
│   ├── Database.cpp         # V3: videos, media methods
│   ├── City.cpp
│   ├── DiaryApp.cpp
│   ├── PreTripCheck.cpp
│   └── HTMLExporter.cpp
├── database/
│   └── diary.db             # SQLite (auto-created)
├── assets/
│   ├── fonts/               # DejaVuSans.ttf
│   └── photos/              # User travel photos
├── export/
│   ├── index.html           # Home page
│   ├── cities.html          # All cities overview
│   ├── city-*.html          # Individual city pages
│   ├── css/styles.css       # Professional styling
│   ├── photos/              # Copied travel photos
│   └── travel_report.html   # Single-page report
├── export_website.py        # Python generator for multi-page site
├── CMakeLists.txt
├── README.md
└── .gitignore
### Data Model

**TravelEntry**
```cpp
int id, float rating
string place, city, date, description, category
string mood (Peaceful|Joyful|Reflective|Nostalgic|Energized|Anxious)
string status (Visited|Planned)
string photoPath
bool isAnonymous, discomfortFlag
```

**City (V3)**
```cpp
int id
string name, description, bestTime, safetyInfo, travelTips
float latitude, longitude
```

**Videos (V3)**
```cpp
int id
string city, youtubeUrl, title
```

**MediaCoverage (V3)**
```cpp
int id
string city, title, url, sourceType (Magazine|News|Vlogger), sourceName
```

**PreTripCheck**
```cpp
bool travelingAlone
int feelingScore (1-4: Excited, Nervous, NeedBreak, Unsure)
bool sharedWithSomeone
int energyLevel (1-3: Low, Medium, High)
bool flagged (computed from above)
```

---

## 🚀 Quick Start

### Prerequisites
```bash
sudo apt update
sudo apt install libsfml-dev libsqlite3-dev cmake g++ python3
```

### Build
```bash
cd MyPakistanDiary
mkdir -p build
cd build
cmake ..
make
g++ -std=c++17 -Iinclude src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsqlite3 -o build/diary
```

### Run Desktop App
```bash
./build/diary
```

### Generate Website
```bash
python3 export_website.py
# Opens at: export/index.html
```

### Deploy to GitHub Pages
```bash
cd export
git init
git add .
git commit -m "My Pakistan Travel Diary - Live Website"
git remote add origin https://github.com/YOUR_USERNAME/MyPakistanDiary-Website.git
git push -u origin main
```

Then enable GitHub Pages in repo Settings → Pages

### Controls
| Key | Action |
|-----|--------|
| `↑` / `↓` | Scroll entries |
| `Enter` | View entry details / Confirm |
| `A` | Add new entry |
| `E` | Export to HTML |
| `Alt+P` | Browse for photo (in form) |
| `Esc` | Back / Quit |
| `Tab` | Next field (in form) |
| `Backspace` | Delete character (in form) |

---

## 📖 Usage Example

### Add a Personal Memory
1. Press `A` to open form
2. Fill fields: "Murree", "Murree", "2025-07-10", "Summer escape..."
3. Set Status to `Visited`
4. Press `Alt+P` to add photo (optional)
5. Press `Enter` → Entry saved to database

### Add a Planned Trip with Wellness Check
1. Press `A`
2. Fill: "Hunza Valley", "Gilgit-Baltistan", "2025-09-01"
3. Set Status to `Planned`
4. Press `Enter`
5. Answer 4 wellness questions (Y, 2, N, 1)
6. If flagged → See support message with helpline
7. Entry saved; return to list

### Export & Share
1. Press `E` from list
2. HTML report generated to `export/travel_report.html`
3. Open in browser → Beautiful styled report with crowd wisdom
4. Share file with friends — they import it for aggregated insights

### Generate Multi-Page Website
1. Press `E` to export from desktop app
2. Run `python3 export_website.py`
3. Website generated with:
   - City pages with travel tips
   - YouTube videos embedded
   - Newspaper & magazine links
   - Interactive maps
   - Professional styling
   - Light/Dark mode

---

## 🎓 Learning Outcomes

This project demonstrates:
- **OOP Design**: Clean class hierarchy (TravelEntry, Database, DiaryApp, City)
- **Database Design**: SQLite schema, CRUD operations, aggregation queries, V3 relations
- **GUI Programming**: SFML event handling, rendering pipeline, state machines, photo display
- **Web Development**: HTML/CSS styling, responsive design, multi-page generation, maps API
- **Python Scripting**: Data extraction, HTML templating, file operations
- **C++ Proficiency**: Smart pointers, STL containers, file I/O, string manipulation
- **Software Architecture**: Decentralized design, separation of concerns, scalability
- **User-Centric Design**: Accessibility (keyboard-first), gentle UX for sensitive topic

---

## 🌱 Mental Health & Wellbeing

This app recognizes that **travel** is more than tourism—it's therapeutic. By embedding wellness check-ins, we:

- **Normalize Emotional Expression**: Travel memories naturally include feelings
- **Create Safe Spaces**: Anonymous sharing option included
- **Build Community**: Wisdom of Crowds shows "you're not alone"
- **Provide Resources**: Every flagged entry links to **Umang helpline: 0317-4288665**

### If You Need Support
- **Pakistan**: Umang (AASRA) — 0317-4288665
- **Internationally**: See [findahelpline.com](https://findahelpline.com)

---

## 📊 Sample Data

The app ships with 8 real Pakistani travel memories across 5 cities:

**Islamabad** (4 entries)
- Shah Allah Ditta Caves
- Lal Masjid
- Faisal Mosque & Pakistan Monument

**Lahore** (2 entries)
- Shahi Qila, Minar-e-Pakistan, Joyland

**Taxila** (1 entry)
- Taxila Buddhist Site (Julian)

**Rawalpindi** (2 entries)
- Army Museum
- Daewoo Terminal

**Murree** (1 entry)
- Mall Road & Chair Lift

All entries use real dates and authentic emotions.

---

## 🔮 Future Roadmap

### V2 — Multi-Page Website ✅ COMPLETE
- Multi-page HTML website with city guides
- OpenStreetMap integration
- Light/Dark mode toggle
- Professional responsive design

### V3 — Media & Videos ✅ COMPLETE
- YouTube video embeds
- Newspaper & magazine feature links
- City-based organization
- Professional tourism layout

### V4 — Community Features
- Web version with backend sync
- Real multi-user entries
- Discomfort alerts for regions
- Travel recommendation engine

### V5 — University Integration
- Deployed on campus networks
- Peer support features
- Integration with counseling services
- Anonymous journaling modes

---

## 🛠️ Development Notes

### Why Decentralized?
- **Privacy**: No central server storing personal data
- **Resilience**: Works offline; sync when convenient
- **Trust**: Users control their own data
- **Scalability**: Each node (laptop/phone) acts independently

### Why SQLite?
- Zero configuration
- File-based (easy to backup/share)
- Rich query support for Wisdom of Crowds
- Lightweight for desktop

### Why SFML?
- Cross-platform
- Simple graphics API
- Keyboard-first (perfect for terminal-like apps)
- Lightweight for modern UIs

### Why OpenStreetMap?
- Free forever (no API key)
- No payment required
- Excellent map data
- Privacy-respecting

---

## 📸 Screenshots

See `docs/` folder for:
- Desktop app with 8 entries
- Detail view with photos
- Add entry form with mood selection
- Wellness questionnaire screens
- HTML export single-page report
- Multi-page website with city guides
- Interactive maps on city pages
- Light/Dark mode toggle working

---

## 🤝 Contributing

This is a portfolio project built in 1 week as proof-of-concept. Future contributors welcome!

### Code Style
- C++17 conventions
- Clear naming (no abbreviations except `m_` for members)
- Comments for complex logic
- Consistent indentation (4 spaces)

---

## 📄 License

MIT License — See LICENSE file (free to use, modify, share)

---

## ��‍💻 Author

**Muhammad Maaz Gul**
- Student, Data Science, FAST-NUCES Islamabad
- GitHub: [@MaazGul-2006](https://github.com/MaazGul-2006)
- Portfolio: [My Pakistan Travel Diary](https://github.com/MaazGul-2006/MyPakistanDiary)
- Live Website: [Travel Website](https://MaazGul-2006.github.io/MyPakistanDiary-Website/)

---

## 🙏 Acknowledgments

- FAST-NUCES for mentorship & infrastructure
- Inspired by *Wisdom of Crowds* (James Surowiecki)
- SFML, SQLite, OpenStreetMap & Leaflet communities
- Mental health advocates in Pakistan
- Real Pakistani travelers who shared their stories

---

**Built with ❤️ for a healthier, more connected Pakistan.**
